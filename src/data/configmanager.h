#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QSettings>
#include <QDebug>
#include "isingleton.h"
#include "config_global.h"
#include "qcolor.h"


#define CONF_MANAGE ConfigManager::instance()
#define WRITE_INI(root, key, val) \
    ConfigManager::instance().setIniValue(QString("/") + root + "/" + key, val)
#define READ_INI(root, key, defVal) \
    ConfigManager::instance().getIniValue(QString("/") + root + "/" + key, defVal)

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
    m_ ## name = val; \
    connect(this, SIGNAL(sig ## name()), this, SLOT(onSyncIni()));



//   宏展开之后的样子
//    Q_PROPERTY(QVariant language MEMBER m_language NOTIFY siglanguage);
//signals:
//    void siglanguage();
//private:
//    QVariant m_language;

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
    SET_VALUE_PROPERTY(XGeneral_auto_detect_windows)
    SET_VALUE_PROPERTY(XGeneral_auto_copy_to_clipbaoard)
    SET_VALUE_PROPERTY(XGeneral_crosshair_show)

public:
    void initIniFile() {
        // General
        WRITE_INI(XGeneral, XGeneral_language, GET_VALUE_PROPERTY(XGeneral_language));  // 使用类成员变量进行赋值到 .ini 文件
        WRITE_INI(XGeneral, XGeneral_log_level, GET_VALUE_PROPERTY(XGeneral_log_level));
        WRITE_INI(XGeneral, XGeneral_font, GET_VALUE_PROPERTY(XGeneral_font));
        WRITE_INI(XGeneral, XGeneral_autostart, GET_VALUE_PROPERTY(XGeneral_autostart));
        // Interface
        WRITE_INI(XInterface, XInterface_style, GET_VALUE_PROPERTY(XInterface_style));
        WRITE_INI(XInterface, XInterface_highlight, GET_VALUE_PROPERTY(XInterface_highlight));
        WRITE_INI(XInterface, XInterface_border_width, GET_VALUE_PROPERTY(XInterface_border_width));
        WRITE_INI(XInterface, XInterface_crosshair, GET_VALUE_PROPERTY(XInterface_crosshair));
        WRITE_INI(XInterface, XInterface_crosshair_width, GET_VALUE_PROPERTY(XInterface_crosshair_width));
        WRITE_INI(XInterface, XGeneral_auto_detect_windows, GET_VALUE_PROPERTY(XGeneral_auto_detect_windows));
        WRITE_INI(XInterface, XGeneral_auto_copy_to_clipbaoard, GET_VALUE_PROPERTY(XGeneral_auto_copy_to_clipbaoard));
        WRITE_INI(XInterface, XGeneral_crosshair_show, GET_VALUE_PROPERTY(XGeneral_crosshair_show));
    }


private:
    void init()
    {
        // General
        CONNECT_VALUE_PROPERTY(XGeneral_language, "English");  // 连接信号和赋值初始化
        CONNECT_VALUE_PROPERTY(XGeneral_log_level, "debug");
        CONNECT_VALUE_PROPERTY(XGeneral_font, "Microsoft YaHei,9");
        CONNECT_VALUE_PROPERTY(XGeneral_autostart, true);
        // Interface
        CONNECT_VALUE_PROPERTY(XInterface_style, "Sunny");
        CONNECT_VALUE_PROPERTY(XInterface_highlight, "#ed1c24");
        CONNECT_VALUE_PROPERTY(XInterface_border_width, 3);
        CONNECT_VALUE_PROPERTY(XInterface_crosshair, "#ed1c24");
        CONNECT_VALUE_PROPERTY(XInterface_crosshair_width, 2);
        CONNECT_VALUE_PROPERTY(XGeneral_auto_detect_windows, true);
        CONNECT_VALUE_PROPERTY(XGeneral_auto_copy_to_clipbaoard, true);
        CONNECT_VALUE_PROPERTY(XGeneral_crosshair_show, false);
    }
    void setIniValue(const QString& key, const QVariant& value);
    QVariant getIniValue(const QString& key, const QVariant& defaultValue = QVariant()) const;


private slots:
    void onSyncIni() {
        qDebug() << "mac  onSyncIni";
        initIniFile();
    }

private:
    explicit ConfigManager(QObject *parent = nullptr);
    virtual ~ConfigManager() = default;

private:
    QSettings* m_settings;
};

#endif // CONFIGMANAGER_H
