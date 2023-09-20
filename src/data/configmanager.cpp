#include "configmanager.h"
#include <QApplication>

// 属性对象改变时  ---snyc----> member对象的值也会改变;但反之 member对象的值改变不会同步属性对象改;
// 故此宏专门用于初次从初始化 .ini 文件，对 "属性对象 + member对象" 同时赋值; 然后在外面则，都是通过属性来修改其数值，从而发生值改变的信号
#define SET_PROPERTY_AND_MEMBER_VALUE(root, key, defVal) \
    GET_VALUE_PROPERTY(key) = READ_INI(root, key, defVal); \
    setProperty(key, GET_VALUE_PROPERTY(key));

void ConfigManager::readFromFile()
{
    // General
    SET_PROPERTY_AND_MEMBER_VALUE(XGeneral, XGeneral_language, "English");
    SET_PROPERTY_AND_MEMBER_VALUE(XGeneral, XGeneral_log_level, "Debug");
    SET_PROPERTY_AND_MEMBER_VALUE(XGeneral, XGeneral_font, "Microsoft YaHei,9");
    SET_PROPERTY_AND_MEMBER_VALUE(XGeneral, XGeneral_autostart, false);
    // Interface
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_style, "Sunny");
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_highlight, "#0081ff");
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_border_width, 3);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_crosshair, "#df4187");
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_crosshair_width, 2);

    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_custom_size_enable, true);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_topleft_enable, true);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_size_enable, true);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_delay_enable, true);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_custom_rect_left, 100);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_custom_rect_top, 100);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_custom_rect_width, 640);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_custom_rect_height, 480);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_custom_dealy, 6.00);

    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_acrylic_effect, true);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_auto_detect_windows, true);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_auto_copy_to_clipbaoard, true);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_crosshair_show, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_bulrr_effect_adius, 20);

    // Output
    SET_PROPERTY_AND_MEMBER_VALUE(XOutput, XOutput_image_quailty, -1);
    SET_PROPERTY_AND_MEMBER_VALUE(XOutput, XOutput_flie_name, QString("%1_$yyyyMMdd_hhmmss$.png").arg(XPROJECT_NAME));
    SET_PROPERTY_AND_MEMBER_VALUE(XOutput, XOutput_config_path, qApp->applicationDirPath());
    SET_PROPERTY_AND_MEMBER_VALUE(XOutput, XOutput_quick_save_enable, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XOutput, XOutput_quick_save_path, QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first());
    SET_PROPERTY_AND_MEMBER_VALUE(XOutput, XOutput_auto_save_enable, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XOutput, XOutput_auto_save_path, QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first());
    // Pin
    SET_PROPERTY_AND_MEMBER_VALUE(XPin, XPin_opacity, 100);
    SET_PROPERTY_AND_MEMBER_VALUE(XPin, XPin_maximum_size, 10000);
    // Hotkeys
    SET_PROPERTY_AND_MEMBER_VALUE(XHotkeys, XHotkeys_capture, "F6");
    SET_PROPERTY_AND_MEMBER_VALUE(XHotkeys, XHotkeys_delay_capture, "Ctrl+F6");
    SET_PROPERTY_AND_MEMBER_VALUE(XHotkeys, XHotkeys_custiom_capture, "Shift+F6");

    qDebug() << GET_VALUE_PROPERTY(XGeneral_language) << GET_VALUE_PROPERTY(XGeneral_log_level)  << GET_VALUE_PROPERTY(XGeneral_font) << GET_VALUE_PROPERTY(XGeneral_autostart);
    qDebug() << GET_VALUE_PROPERTY(XInterface_style) << GET_VALUE_PROPERTY(XInterface_highlight) << GET_VALUE_PROPERTY(XInterface_border_width) << GET_VALUE_PROPERTY(XInterface_crosshair) << GET_VALUE_PROPERTY(XInterface_crosshair_width)
             << GET_VALUE_PROPERTY(XInterface_custom_size_enable) << GET_VALUE_PROPERTY(XInterface_topleft_enable) << GET_VALUE_PROPERTY(XInterface_size_enable) << GET_VALUE_PROPERTY(XInterface_delay_enable)
             << GET_VALUE_PROPERTY(XInterface_custom_rect_left) << GET_VALUE_PROPERTY(XInterface_custom_rect_top) << GET_VALUE_PROPERTY(XInterface_custom_rect_width) << GET_VALUE_PROPERTY(XInterface_custom_rect_height) << GET_VALUE_PROPERTY(XInterface_custom_dealy)
             << GET_VALUE_PROPERTY(XInterface_acrylic_effect) << GET_VALUE_PROPERTY(XInterface_auto_detect_windows) << GET_VALUE_PROPERTY(XInterface_auto_copy_to_clipbaoard) << GET_VALUE_PROPERTY(XInterface_crosshair_show) << GET_VALUE_PROPERTY(XInterface_bulrr_effect_adius);;
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

    WRITE_INI(XInterface, XInterface_custom_size_enable, GET_VALUE_PROPERTY(XInterface_custom_size_enable));
    WRITE_INI(XInterface, XInterface_topleft_enable, GET_VALUE_PROPERTY(XInterface_topleft_enable));
    WRITE_INI(XInterface, XInterface_size_enable, GET_VALUE_PROPERTY(XInterface_size_enable));
    WRITE_INI(XInterface, XInterface_delay_enable, GET_VALUE_PROPERTY(XInterface_delay_enable));
    WRITE_INI(XInterface, XInterface_custom_rect_left, GET_VALUE_PROPERTY(XInterface_custom_rect_left));
    WRITE_INI(XInterface, XInterface_custom_rect_top, GET_VALUE_PROPERTY(XInterface_custom_rect_top));
    WRITE_INI(XInterface, XInterface_custom_rect_width, GET_VALUE_PROPERTY(XInterface_custom_rect_width));
    WRITE_INI(XInterface, XInterface_custom_rect_height, GET_VALUE_PROPERTY(XInterface_custom_rect_height));
    WRITE_INI(XInterface, XInterface_custom_dealy, GET_VALUE_PROPERTY(XInterface_custom_dealy));
    WRITE_INI(XInterface, XInterface_acrylic_effect, GET_VALUE_PROPERTY(XInterface_acrylic_effect));
    WRITE_INI(XInterface, XInterface_auto_detect_windows, GET_VALUE_PROPERTY(XInterface_auto_detect_windows));
    WRITE_INI(XInterface, XInterface_auto_copy_to_clipbaoard, GET_VALUE_PROPERTY(XInterface_auto_copy_to_clipbaoard));
    WRITE_INI(XInterface, XInterface_crosshair_show, GET_VALUE_PROPERTY(XInterface_crosshair_show));

    WRITE_INI(XInterface, XInterface_bulrr_effect_adius, GET_VALUE_PROPERTY(XInterface_bulrr_effect_adius));
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

    CONNECT_VALUE_PROPERTY(XInterface_custom_size_enable, true);
    CONNECT_VALUE_PROPERTY(XInterface_topleft_enable, true);
    CONNECT_VALUE_PROPERTY(XInterface_size_enable, true);
    CONNECT_VALUE_PROPERTY(XInterface_delay_enable, true);
    CONNECT_VALUE_PROPERTY(XInterface_custom_rect_left, 100);
    CONNECT_VALUE_PROPERTY(XInterface_custom_rect_top, 100);
    CONNECT_VALUE_PROPERTY(XInterface_custom_rect_width, 640);
    CONNECT_VALUE_PROPERTY(XInterface_custom_rect_height, 480);
    CONNECT_VALUE_PROPERTY(XInterface_custom_dealy, 6);

    CONNECT_VALUE_PROPERTY(XInterface_acrylic_effect, true);
    CONNECT_VALUE_PROPERTY(XInterface_auto_detect_windows, true);
    CONNECT_VALUE_PROPERTY(XInterface_auto_copy_to_clipbaoard, true);
    CONNECT_VALUE_PROPERTY(XInterface_crosshair_show, false);
    CONNECT_VALUE_PROPERTY(XInterface_bulrr_effect_adius, 20);
    // Output
    CONNECT_VALUE_PROPERTY(XOutput_image_quailty, -1);
    CONNECT_VALUE_PROPERTY(XOutput_flie_name, QString("%1_$yyyyMMdd_hhmmss$.png").arg(XPROJECT_NAME));
    CONNECT_VALUE_PROPERTY(XOutput_config_path, qApp->applicationDirPath());
    CONNECT_VALUE_PROPERTY(XOutput_quick_save_enable, false);
    CONNECT_VALUE_PROPERTY(XOutput_quick_save_path, QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first());
    CONNECT_VALUE_PROPERTY(XOutput_auto_save_enable, false);
    CONNECT_VALUE_PROPERTY(XOutput_auto_save_path, QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first());
    // Pin
    CONNECT_VALUE_PROPERTY(XPin_opacity, 100);
    CONNECT_VALUE_PROPERTY(XPin_maximum_size, 10000);
    // Hotkeys
    CONNECT_VALUE_PROPERTY(XHotkeys_capture, "F6");
    CONNECT_VALUE_PROPERTY(XHotkeys_delay_capture, "Ctrl+F6");
    CONNECT_VALUE_PROPERTY(XHotkeys_custiom_capture, "Shift+F6");
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
