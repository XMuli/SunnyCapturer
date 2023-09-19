#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QStandardPaths>
#include "isingleton.h"
#include "config_global.h"

#define CONF_MANAGE ConfigManager::instance()

#if 1
    #define WRITE_INI(root, key, val) \
        setIniValue(QString("/") + root + "/" + key, val)
    #define READ_INI(root, key, defVal) \
        getIniValue(QString("/") + root + "/" + key, defVal)
#else
    #define WRITE_INI(root, key, val) \
        ConfigManager::instance().setIniValue(QString("/") + root + "/" + key, val)
    #define READ_INI(root, key, defVal) \
        ConfigManager::instance().getIniValue(QString("/") + root + "/" + key, defVal)
#endif

// 每新建一个 保存的变量 到配置文件，就分别在对应的三个位置调用如下的三个宏即可。
// 这里面保存的属性名字实际为 “常变量” 而不是 其 所对应的具体字符串 -> 为了外部的书写是统一的，且实际的配置的名字也是耦合考虑

#define SET_VALUE_PROPERTY(name) \
    Q_PROPERTY(QVariant name MEMBER m_ ## name NOTIFY sig ## name) \
    Q_SIGNALS: \
        void sig ## name(); \
    private: \
        QVariant m_ ## name;

#define GET_VALUE_PROPERTY(name) m_ ## name

#define CONNECT_VALUE_PROPERTY(name, val) \
    connect(this, SIGNAL(sig ## name()), this, SLOT(onSyncToFile()));
//    m_ ## name = val;
//    connect(this, SIGNAL(sig ## name()), this, [](){ });


//   宏展开之后的样子
//    Q_PROPERTY(QVariant language MEMBER m_language NOTIFY siglanguage);
//signals:
//    void siglanguage();
//private:
//    QVariant m_language;


/***************************************************************************************************************

*.ui  ⇆  ConfigManager::instance()  ⇆  PROPERTY name  ⇆  class property member   ⇆  QSetting    ⇆  xconfig.ini

***************************************************************************************************************/

class ConfigManager : public QObject, public ISingleton<ConfigManager>
{
    Q_OBJECT
    FRIEND_CREAT_SINGLETION(ConfigManager);

    // General
    SET_VALUE_PROPERTY(XGeneral_language) //生成对应的属性，信号，类成员变量
    SET_VALUE_PROPERTY(XGeneral_log_level)
    SET_VALUE_PROPERTY(XGeneral_font)
    SET_VALUE_PROPERTY(XGeneral_autostart)
    // Interface
    SET_VALUE_PROPERTY(XInterface_style)
    SET_VALUE_PROPERTY(XInterface_highlight)
    SET_VALUE_PROPERTY(XInterface_border_width)
    SET_VALUE_PROPERTY(XInterface_crosshair)
    SET_VALUE_PROPERTY(XInterface_crosshair_width)

    SET_VALUE_PROPERTY(XInterface_custom_size_enable);
    SET_VALUE_PROPERTY(XInterface_topleft_enable);
    SET_VALUE_PROPERTY(XInterface_size_enable);
    SET_VALUE_PROPERTY(XInterface_delay_enable);
    SET_VALUE_PROPERTY(XInterface_custom_rect_left);
    SET_VALUE_PROPERTY(XInterface_custom_rect_top);
    SET_VALUE_PROPERTY(XInterface_custom_rect_width);
    SET_VALUE_PROPERTY(XInterface_custom_rect_height);
    SET_VALUE_PROPERTY(XInterface_custom_dealy);

    SET_VALUE_PROPERTY(XInterface_acrylic_effect)
    SET_VALUE_PROPERTY(XInterface_auto_detect_windows)
    SET_VALUE_PROPERTY(XInterface_auto_copy_to_clipbaoard)
    SET_VALUE_PROPERTY(XInterface_crosshair_show)

    SET_VALUE_PROPERTY(XInterface_bulrr_effect_adius)  // only *.ini write and read

    // Output
    SET_VALUE_PROPERTY(XOutput_image_quailty)
    SET_VALUE_PROPERTY(XOutput_flie_name)
    SET_VALUE_PROPERTY(XOutput_config_path)
    SET_VALUE_PROPERTY(XOutput_quick_save_enable)
    SET_VALUE_PROPERTY(XOutput_quick_save_path)
    SET_VALUE_PROPERTY(XOutput_auto_save_enable)
    SET_VALUE_PROPERTY(XOutput_auto_save_path)
    // Pin
    SET_VALUE_PROPERTY(XPin_opacity);
    SET_VALUE_PROPERTY(XPin_maximum_size);
    // Hotkeys
    SET_VALUE_PROPERTY(XHotkeys_capture);
    SET_VALUE_PROPERTY(XHotkeys_delay_capture);
    SET_VALUE_PROPERTY(XHotkeys_custiom_capture);

public:
    void readFromFile();
    void writeToFile();

private:
    void init();   // 关连属性的 成员变量-的信号和槽
    void setIniValue(const QString& key, const QVariant& value);
    QVariant getIniValue(const QString& key, const QVariant& defaultValue = QVariant()) const;

private slots:
    void onSyncToFile();

private:
    explicit ConfigManager(QObject *parent = nullptr);
    virtual ~ConfigManager() = default;

private:
    QSettings* m_settings;
};

#endif // CONFIGMANAGER_H
