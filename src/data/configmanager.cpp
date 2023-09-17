#include "configmanager.h"
#include <QApplication>

void ConfigManager::readFromFile()
{
    // General
    GET_VALUE_PROPERTY(XGeneral_language)                  = READ_INI(XGeneral, XGeneral_language, "English");
    GET_VALUE_PROPERTY(XGeneral_log_level)                 = READ_INI(XGeneral, XGeneral_log_level, "Debug");
    GET_VALUE_PROPERTY(XGeneral_font)                      = READ_INI(XGeneral, XGeneral_font, "Microsoft YaHei,9");
    GET_VALUE_PROPERTY(XGeneral_autostart)                 = READ_INI(XGeneral, XGeneral_autostart, false).toBool();
    // Interface
    GET_VALUE_PROPERTY(XInterface_style)                   = READ_INI(XInterface, XInterface_style, "Sunny");
    GET_VALUE_PROPERTY(XInterface_highlight)               = READ_INI(XInterface, XInterface_highlight, "#ed1c24");
    GET_VALUE_PROPERTY(XInterface_border_width)            = READ_INI(XInterface, XInterface_border_width, 3).toInt();
    GET_VALUE_PROPERTY(XInterface_crosshair)               = READ_INI(XInterface, XInterface_crosshair, "#ed1c24");
    GET_VALUE_PROPERTY(XInterface_crosshair_width)         = READ_INI(XInterface, XInterface_crosshair_width, 2).toInt();
    GET_VALUE_PROPERTY(XInterface_acrylic_effect)          = READ_INI(XInterface, XInterface_acrylic_effect, true).toBool();
    GET_VALUE_PROPERTY(XInterface_auto_detect_windows)     = READ_INI(XInterface, XInterface_auto_detect_windows, true).toBool();
    GET_VALUE_PROPERTY(XInterface_auto_copy_to_clipbaoard) = READ_INI(XInterface, XInterface_auto_copy_to_clipbaoard, true).toBool();
    GET_VALUE_PROPERTY(XInterface_crosshair_show)          = READ_INI(XInterface, XInterface_crosshair_show, false).toBool();
    // Output
    GET_VALUE_PROPERTY(XOutput_image_quailty)              = READ_INI(XOutput, XOutput_image_quailty, -1).toInt();
    GET_VALUE_PROPERTY(XOutput_flie_name)                  = READ_INI(XOutput, XOutput_flie_name, "Sunny_$yyyyMMdd_hhmmss$.png");
    GET_VALUE_PROPERTY(XOutput_config_path)                = READ_INI(XOutput, XOutput_config_path, qApp->applicationDirPath());
    GET_VALUE_PROPERTY(XOutput_quick_save_enable)          = READ_INI(XOutput, XOutput_quick_save_enable, false).toBool();
    GET_VALUE_PROPERTY(XOutput_quick_save_path)            = READ_INI(XOutput, XOutput_quick_save_path, QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first());
    GET_VALUE_PROPERTY(XOutput_auto_save_enable)           = READ_INI(XOutput, XOutput_auto_save_enable, false).toBool();
    GET_VALUE_PROPERTY(XOutput_auto_save_path)             = READ_INI(XOutput, XOutput_auto_save_path, QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first());
    // Pin
    GET_VALUE_PROPERTY(XPin_opacity)                       = READ_INI(XPin, XPin_opacity, 100).toInt();
    GET_VALUE_PROPERTY(XPin_maximum_size)                  = READ_INI(XPin, XPin_maximum_size, 10000).toInt();
    // Hotkeys
    GET_VALUE_PROPERTY(XHotkeys_capture)                   = READ_INI(XHotkeys, XHotkeys_capture, "F3");
    GET_VALUE_PROPERTY(XHotkeys_delay_capture)             = READ_INI(XHotkeys, XHotkeys_delay_capture, "F7");
    GET_VALUE_PROPERTY(XHotkeys_custiom_capture)           = READ_INI(XHotkeys, XHotkeys_custiom_capture, "F8");

    qDebug() << GET_VALUE_PROPERTY(XGeneral_language) << GET_VALUE_PROPERTY(XGeneral_log_level)  << GET_VALUE_PROPERTY(XGeneral_font) << GET_VALUE_PROPERTY(XGeneral_autostart);
    qDebug() << GET_VALUE_PROPERTY(XInterface_style) << GET_VALUE_PROPERTY(XInterface_highlight) << GET_VALUE_PROPERTY(XInterface_border_width) << GET_VALUE_PROPERTY(XInterface_crosshair) << GET_VALUE_PROPERTY(XInterface_crosshair_width)
             << GET_VALUE_PROPERTY(XInterface_acrylic_effect) << GET_VALUE_PROPERTY(XInterface_auto_detect_windows) << GET_VALUE_PROPERTY(XInterface_auto_copy_to_clipbaoard) << GET_VALUE_PROPERTY(XInterface_crosshair_show);
    qDebug() << GET_VALUE_PROPERTY(XOutput_image_quailty) << GET_VALUE_PROPERTY(XOutput_flie_name)  << GET_VALUE_PROPERTY(XOutput_config_path)
             << GET_VALUE_PROPERTY(XOutput_quick_save_enable) << GET_VALUE_PROPERTY(XOutput_quick_save_path)  << GET_VALUE_PROPERTY(XOutput_auto_save_enable) << GET_VALUE_PROPERTY(XOutput_auto_save_path);
    qDebug() << GET_VALUE_PROPERTY(XPin_opacity) << GET_VALUE_PROPERTY(XPin_maximum_size);
    qDebug() << GET_VALUE_PROPERTY(XHotkeys_capture) << GET_VALUE_PROPERTY(XHotkeys_delay_capture)  << GET_VALUE_PROPERTY(XHotkeys_custiom_capture);
}

void ConfigManager::writeToFile()
{
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
    WRITE_INI(XInterface, XInterface_acrylic_effect, GET_VALUE_PROPERTY(XInterface_acrylic_effect));
    WRITE_INI(XInterface, XInterface_auto_detect_windows, GET_VALUE_PROPERTY(XInterface_auto_detect_windows));
    WRITE_INI(XInterface, XInterface_auto_copy_to_clipbaoard, GET_VALUE_PROPERTY(XInterface_auto_copy_to_clipbaoard));
    WRITE_INI(XInterface, XInterface_crosshair_show, GET_VALUE_PROPERTY(XInterface_crosshair_show));
    // Output
    WRITE_INI(XOutput, XOutput_image_quailty, GET_VALUE_PROPERTY(XOutput_image_quailty));
    WRITE_INI(XOutput, XOutput_flie_name, GET_VALUE_PROPERTY(XOutput_flie_name));
    WRITE_INI(XOutput, XOutput_config_path, GET_VALUE_PROPERTY(XOutput_config_path));
    WRITE_INI(XOutput, XOutput_quick_save_enable, GET_VALUE_PROPERTY(XOutput_quick_save_enable));
    WRITE_INI(XOutput, XOutput_quick_save_path, GET_VALUE_PROPERTY(XOutput_quick_save_path));
    WRITE_INI(XOutput, XOutput_auto_save_enable, GET_VALUE_PROPERTY(XOutput_auto_save_enable));
    WRITE_INI(XOutput, XOutput_auto_save_path, GET_VALUE_PROPERTY(XOutput_auto_save_path));
    // Pin
    WRITE_INI(XPin, XPin_opacity, GET_VALUE_PROPERTY(XPin_opacity));
    WRITE_INI(XPin, XPin_maximum_size, GET_VALUE_PROPERTY(XPin_maximum_size));
    // Hotkeys
    WRITE_INI(XHotkeys, XHotkeys_capture, GET_VALUE_PROPERTY(XHotkeys_capture));
    WRITE_INI(XHotkeys, XHotkeys_delay_capture, GET_VALUE_PROPERTY(XHotkeys_delay_capture));
    WRITE_INI(XHotkeys, XHotkeys_custiom_capture, GET_VALUE_PROPERTY(XHotkeys_custiom_capture));
}

void ConfigManager::init()
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
    CONNECT_VALUE_PROPERTY(XInterface_acrylic_effect, true);
    CONNECT_VALUE_PROPERTY(XInterface_auto_detect_windows, true);
    CONNECT_VALUE_PROPERTY(XInterface_auto_copy_to_clipbaoard, true);
    CONNECT_VALUE_PROPERTY(XInterface_crosshair_show, false);
    // Output
    CONNECT_VALUE_PROPERTY(XOutput_image_quailty, -1);
    CONNECT_VALUE_PROPERTY(XOutput_flie_name, "Sunny_$yyyyMMdd_hhmmss$.png");
    CONNECT_VALUE_PROPERTY(XOutput_config_path, qApp->applicationDirPath());
    CONNECT_VALUE_PROPERTY(XOutput_quick_save_enable, false);
    CONNECT_VALUE_PROPERTY(XOutput_quick_save_path, QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first());
    CONNECT_VALUE_PROPERTY(XOutput_auto_save_enable, false);
    CONNECT_VALUE_PROPERTY(XOutput_auto_save_path, QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first());
    // Pin
    CONNECT_VALUE_PROPERTY(XPin_opacity, 100);
    CONNECT_VALUE_PROPERTY(XPin_maximum_size, 10000);
    // Hotkeys
    CONNECT_VALUE_PROPERTY(XHotkeys_capture, "F3");
    CONNECT_VALUE_PROPERTY(XHotkeys_delay_capture, "F7");
    CONNECT_VALUE_PROPERTY(XHotkeys_custiom_capture, "F8");
}

void ConfigManager::setIniValue(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
    m_settings->sync(); // 将更改立即写入到配置文件
}

QVariant ConfigManager::getIniValue(const QString &key, const QVariant &defaultValue) const
{
    return m_settings->value(key, defaultValue);
}

void ConfigManager::onSyncToFile()
{
    qDebug() << "run onSyncToFile().";
    writeToFile();
}

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
    , m_settings(new QSettings(qApp->applicationDirPath() + "/xconfig.ini", QSettings::IniFormat, this))
{
    m_settings->setIniCodec("UTF-8"); // 禁用自动转义
    readFromFile();
    init();
}
