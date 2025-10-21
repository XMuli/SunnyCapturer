// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QSettings>
#include <QApplication>
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QStandardPaths>
#include "isingleton.h"
#include "config_global.h"


struct PaintBarStatus
{
    // 一级工具栏的状态
    bool rect = false;
    bool ellipse = false;
    bool arrow = false;
    bool penciler = false;
    bool marker_pen = false;
    bool mosaic = false;
    bool text = false;
    bool serial = false;

    // 二级工具栏的状态
    int rectintType = -1;
    int ellipseType = -1;
    int arrowType = -1;
    int marker_penType = -1;

    int mosaicType = -1;
    int pixelatedMosaic = -1;
    int smoothMosaic = -1;

    bool textBold = false;
    bool textItalic = false;
    bool textOutline = false;
    bool textStrikeout = false;
    bool textUnderline = false;
    QString fontFamily = "Microsoft YaHei";
    int fontSize = 16;

    int serialType = -1;
    int serialNumber = -1;
    QChar serialLetter = ' ';
    int pointType = -1;

    QPen pen = QPen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QBrush brush = QBrush(Qt::red, Qt::SolidPattern);
};

void initPaintBarStatus(PaintBarStatus& pbs);

//-------------------------------------------------------------

#define CONF_MANAGE ConfigManager::instance()
#define CONF_PBS_DATA ConfigManager::instance().m_paintBarStatus  // 获取 m_paintBarStatus

#define CONF_SET_PROPERTY(name, val) CONF_MANAGE.setProperty(#name, val)  // 直接设置单例里面的属性为新值
#define CONF_GET_PROPERTY(name) CONF_MANAGE.property(#name)               // 直接获取单例里面的属性

// 每新建一个 保存的变量 到配置文件，就分别在对应的三个位置调用如下的三个宏即可。
// 这里面保存的属性名字实际为 “常变量” 而不是 其 所对应的具体字符串 -> 为了外部的书写是统一的，且实际的配置的名字也是耦合考虑

#define SET_VALUE_PROPERTY(name) \
    Q_PROPERTY(QVariant name MEMBER m_ ## name NOTIFY sig ## name) \
    Q_SIGNALS: \
        void sig ## name(); \
    private: \
        QVariant m_ ## name;

#define GET_VALUE_PROPERTY(name) m_ ## name


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

public:
    void readFromFile();
    void writeToFile();    // 便于开发测试，才将在这两个设置为 public 的

    // 加密解密 API-Key
    QByteArray encryptString(const QString& input);
    QString decryptString(const QByteArray& input);

private:
    void setIniValue(const QString& key, const QVariant& value);
    QVariant getIniValue(const QString& key, const QVariant& defaultValue = QVariant()) const;

public:
    void onSyncToFile();

private:
    explicit ConfigManager(QObject *parent = nullptr);
    virtual ~ConfigManager() = default;

public:
    PaintBarStatus m_paintBarStatus;           // 初始工具栏的状态  -> 当关闭时刻，需要写入一边到文本保存？; 可写也可以不写。嘎

private:
    QSettings* m_settings;
    QString m_key;
    QString m_iv;

private:   // 为了便于查看，故属性声明放在最下面
    // General
    SET_VALUE_PROPERTY(XGeneral_language) //生成对应的属性，信号，类成员变量
    SET_VALUE_PROPERTY(XGeneral_log_level)
    SET_VALUE_PROPERTY(XGeneral_themes)
    SET_VALUE_PROPERTY(XGeneral_font)
    SET_VALUE_PROPERTY(XGeneral_autostart)
    // Interface
    SET_VALUE_PROPERTY(XInterface_style)
    SET_VALUE_PROPERTY(XInterface_orientation)
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
    SET_VALUE_PROPERTY(XHotkeys_custom_capture);
    // Tokens
    SET_VALUE_PROPERTY(XTokens_youdao_app_id);
    SET_VALUE_PROPERTY(XTokens_youdao_secret_key);
    SET_VALUE_PROPERTY(XTokens_iamge_translate_from_youdao);
    SET_VALUE_PROPERTY(XTokens_iamge_translate_to_youdao);
    SET_VALUE_PROPERTY(XTokens_iamge_translate_from_baidu);
    SET_VALUE_PROPERTY(XTokens_iamge_translate_to_baidu);
    SET_VALUE_PROPERTY(XTokens_baidu_api_key);
    SET_VALUE_PROPERTY(XTokens_baidu_secret_key);
    SET_VALUE_PROPERTY(XTokens_ocr_channel);
    SET_VALUE_PROPERTY(XTokens_iamge_translate_channel);
    // XOtherControl
    SET_VALUE_PROPERTY(XOtherControl_blur_effect_adius);
    SET_VALUE_PROPERTY(XOtherControl_highlight_iridescence);
    SET_VALUE_PROPERTY(XOtherControl_crosshair_iridescence);
    SET_VALUE_PROPERTY(XOtherControl_show_develop_ui_log);
    // XOtherData
    SET_VALUE_PROPERTY(XOtherData_manual_save_image_dir);
    SET_VALUE_PROPERTY(XOtherData_detection_min_windows_level_depth);
    // XPaintBarStatus
    SET_VALUE_PROPERTY(XPaintBarStatus_rect);
    SET_VALUE_PROPERTY(XPaintBarStatus_ellipse);
    SET_VALUE_PROPERTY(XPaintBarStatus_arrow);
    SET_VALUE_PROPERTY(XPaintBarStatus_penciler);
    SET_VALUE_PROPERTY(XPaintBarStatus_marker_pen);
    SET_VALUE_PROPERTY(XPaintBarStatus_mosaic);
    SET_VALUE_PROPERTY(XPaintBarStatus_text);
    SET_VALUE_PROPERTY(XPaintBarStatus_serial);

    SET_VALUE_PROPERTY(XPaintBarStatus_rectType);
    SET_VALUE_PROPERTY(XPaintBarStatus_ellipseType);
    SET_VALUE_PROPERTY(XPaintBarStatus_arrowType);
    SET_VALUE_PROPERTY(XPaintBarStatus_marker_penType);
    SET_VALUE_PROPERTY(XPaintBarStatus_mosaicType);
    SET_VALUE_PROPERTY(XPaintBarStatus_pixelatedMosaic);
    SET_VALUE_PROPERTY(XPaintBarStatus_smoothMosaic);
    SET_VALUE_PROPERTY(XPaintBarStatus_textBold);
    SET_VALUE_PROPERTY(XPaintBarStatus_textItalic);
    SET_VALUE_PROPERTY(XPaintBarStatus_textOutline);
    SET_VALUE_PROPERTY(XPaintBarStatus_textStrikeout);
    SET_VALUE_PROPERTY(XPaintBarStatus_textUnderline);
    SET_VALUE_PROPERTY(XPaintBarStatus_fontFamily);
    SET_VALUE_PROPERTY(XPaintBarStatus_fontSize);
    SET_VALUE_PROPERTY(XPaintBarStatus_serialType);
    SET_VALUE_PROPERTY(XPaintBarStatus_serialNumber);
    SET_VALUE_PROPERTY(XPaintBarStatus_serialLetter);
    SET_VALUE_PROPERTY(XPaintBarStatus_pointType);
    SET_VALUE_PROPERTY(XPaintBarStatus_paPen);
    SET_VALUE_PROPERTY(XPaintBarStatus_paBrush);
};



#endif // CONFIGMANAGER_H
