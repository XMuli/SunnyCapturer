#include "configmanager.h"
#include <QApplication>
#include <QPen>
#include <QBrush>

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

// 属性对象改变时  ---snyc----> member对象的值也会改变;但反之 member对象的值改变不会同步属性对象改;
// 故此宏专门用于初次从初始化 .ini 文件，对 "属性对象 + member对象" 同时赋值; 然后在外面则，都是通过属性来修改其数值，从而发生值改变的信号
const int width = 6;
#define SET_PROPERTY_AND_MEMBER_VALUE(root, key, defVal) \
    GET_VALUE_PROPERTY(key) = READ_INI(root, key, defVal); \
    setProperty(key, GET_VALUE_PROPERTY(key)); \
qDebug() << "[" << root << "/" << key << "]: " << GET_VALUE_PROPERTY(key); // << "defVal:" << defVal;

//    connect(this, SIGNAL(sig ## key()), this, SLOT(onSyncToFile()));   // 读写 IO 过于频繁，改为窗口关闭才写入文件

//#define CONNECT_VALUE_PROPERTY(name, val) \
//    connect(this, SIGNAL(sig ## name()), this, SLOT(onSyncToFile()));

void ConfigManager::readFromFile()
{
    // General
    SET_PROPERTY_AND_MEMBER_VALUE(XGeneral, XGeneral_language, "English");
    SET_PROPERTY_AND_MEMBER_VALUE(XGeneral, XGeneral_themes, "default");
    SET_PROPERTY_AND_MEMBER_VALUE(XGeneral, XGeneral_log_level, "Debug");
    SET_PROPERTY_AND_MEMBER_VALUE(XGeneral, XGeneral_font, "Microsoft YaHei,9");
    SET_PROPERTY_AND_MEMBER_VALUE(XGeneral, XGeneral_autostart, false);
    // Interface
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_style, "Sunny");
    SET_PROPERTY_AND_MEMBER_VALUE(XInterface, XInterface_orientation, "Horizontal");
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
    // XOtherControl
    QStringList highlight;
    highlight << "#DF4187" << "#FF5D00" << "#F8CB00" << "#23C400"
              << "#00A48A" << "#0081FF" << "#3C02FF" << "#8C00D4" << "#4D4D4D";
    QStringList crosshair;
    crosshair << "#000000" << "#7f7f7f" << "#880015" << "#ed1c24" << "#ff7f27"
           << "#fff200" << "#22b14c" << "#00a2e8" << "#3f48cc" << "#a349a4"
           << "#ffffff" << "#c3c3c3" << "#b97a57" << "#ffaec9" << "#ffc90e"
           << "#efe4b0" << "#b5e61d" << "#99d9ea" << "#7092be" << "#c8bfe7";
    SET_PROPERTY_AND_MEMBER_VALUE(XOtherControl, XOtherControl_blur_effect_adius, 20);
    SET_PROPERTY_AND_MEMBER_VALUE(XOtherControl, XOtherControl_highlight_iridescence, highlight);
    SET_PROPERTY_AND_MEMBER_VALUE(XOtherControl, XOtherControl_crosshair_iridescence, crosshair);
    SET_PROPERTY_AND_MEMBER_VALUE(XOtherControl, XOtherControl_show_develop_ui_log, false);
    // XOtherData
    SET_PROPERTY_AND_MEMBER_VALUE(XOtherData, XOtherData_manual_save_image_dir, QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first());
    SET_PROPERTY_AND_MEMBER_VALUE(XOtherData, XOtherData_detection_min_windows_level_depth, true);
    // XPaintBarStatus
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_rect, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_ellipse, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_arrow, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_penciler, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_marker_pen, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_mosaic, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_serial, false);

    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_rectType, -1);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_ellipseType, -1);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_arrowType, -1);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_marker_penType, -1);

    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_mosaicType, -1);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_pixelatedMosaic, -1);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_smoothMosaic, -1);

    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_textBold, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_textItalic, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_textOutline, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_textStrikeout, false);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_textUnderline, false);

    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_fontFamily, "Microsoft YaHei");
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_fontSize, 16);

    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_serialType, -1);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_serialNumber, -1);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_serialLetter, QChar());
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_pointType, -1);
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_paPen, QPen());
    SET_PROPERTY_AND_MEMBER_VALUE(XPaintBarStatus, XPaintBarStatus_paBrush, QBrush());
}

void ConfigManager::writeToFile()
{
    // General
    WRITE_INI(XGeneral, XGeneral_language, GET_VALUE_PROPERTY(XGeneral_language));  // 使用类成员变量进行赋值到 .ini 文件
    WRITE_INI(XGeneral, XGeneral_themes, GET_VALUE_PROPERTY(XGeneral_themes));
    WRITE_INI(XGeneral, XGeneral_log_level, GET_VALUE_PROPERTY(XGeneral_log_level));
    WRITE_INI(XGeneral, XGeneral_font, GET_VALUE_PROPERTY(XGeneral_font));
    WRITE_INI(XGeneral, XGeneral_autostart, GET_VALUE_PROPERTY(XGeneral_autostart));
    // Interface
    WRITE_INI(XInterface, XInterface_style, GET_VALUE_PROPERTY(XInterface_style));
    WRITE_INI(XInterface, XInterface_orientation, GET_VALUE_PROPERTY(XInterface_orientation));
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
    // XOtherControl
    WRITE_INI(XOtherControl, XOtherControl_blur_effect_adius, GET_VALUE_PROPERTY(XOtherControl_blur_effect_adius));
    WRITE_INI(XOtherControl, XOtherControl_highlight_iridescence, GET_VALUE_PROPERTY(XOtherControl_highlight_iridescence));
    WRITE_INI(XOtherControl, XOtherControl_crosshair_iridescence, GET_VALUE_PROPERTY(XOtherControl_crosshair_iridescence));
    WRITE_INI(XOtherControl, XOtherControl_show_develop_ui_log, GET_VALUE_PROPERTY(XOtherControl_show_develop_ui_log));
    // XOtherData
    WRITE_INI(XOtherData, XOtherData_manual_save_image_dir, GET_VALUE_PROPERTY(XOtherData_manual_save_image_dir));
    WRITE_INI(XOtherData, XOtherData_detection_min_windows_level_depth, GET_VALUE_PROPERTY(XOtherData_detection_min_windows_level_depth));
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
    , m_settings(nullptr)
{
    QString xconfigDir = qApp->applicationDirPath();
#if defined(Q_OS_LINUX)
    xconfigDir = QString("/usr/local/%1").arg(XPROJECT_NAME);
#endif

    m_settings = new QSettings(xconfigDir + "/xconfig.ini", QSettings::IniFormat, this);
    m_settings->setIniCodec("UTF-8"); // 禁用自动转义
    readFromFile();
}


void initPaintBarStatus(PaintBarStatus &pbs)
{
    // 一级工具栏的状态
    pbs.rect = CONF_GET_PROPERTY(XPaintBarStatus_rect).toBool();
    pbs.ellipse = CONF_GET_PROPERTY(XPaintBarStatus_ellipse).toBool();
    pbs.arrow = CONF_GET_PROPERTY(XPaintBarStatus_arrow).toBool();
    pbs.penciler = CONF_GET_PROPERTY(XPaintBarStatus_penciler).toBool();
    pbs.marker_pen = CONF_GET_PROPERTY(XPaintBarStatus_marker_pen).toBool();
    pbs.mosaic = CONF_GET_PROPERTY(XPaintBarStatus_mosaic).toBool();
    pbs.text = CONF_GET_PROPERTY(XPaintBarStatus_text).toBool();
    pbs.serial = CONF_GET_PROPERTY(XPaintBarStatus_serial).toBool();

    // 二级工具栏的状态
    pbs.rectintType = CONF_GET_PROPERTY(XPaintBarStatus_rectType).toInt();
    pbs.ellipseType = CONF_GET_PROPERTY(XPaintBarStatus_ellipseType).toInt();
    pbs.arrowType = CONF_GET_PROPERTY(XPaintBarStatus_arrowType).toInt();
    pbs.marker_penType = CONF_GET_PROPERTY(XPaintBarStatus_marker_penType).toInt();

    pbs.mosaicType = CONF_GET_PROPERTY(XPaintBarStatus_mosaicType).toInt();
    pbs.pixelatedMosaic = CONF_GET_PROPERTY(XPaintBarStatus_pixelatedMosaic).toInt();
    pbs.smoothMosaic = CONF_GET_PROPERTY(XPaintBarStatus_smoothMosaic).toInt();

    pbs.textBold = CONF_GET_PROPERTY(XPaintBarStatus_textBold).toBool();
    pbs.textItalic = CONF_GET_PROPERTY(XPaintBarStatus_textItalic).toBool();
    pbs.textOutline = CONF_GET_PROPERTY(XPaintBarStatus_textOutline).toBool();
    pbs.textStrikeout = CONF_GET_PROPERTY(XPaintBarStatus_textStrikeout).toBool();
    pbs.textUnderline = CONF_GET_PROPERTY(XPaintBarStatus_textUnderline).toBool();
    pbs.fontFamily = CONF_GET_PROPERTY(XPaintBarStatus_fontFamily).toString();
    pbs.fontSize = CONF_GET_PROPERTY(XPaintBarStatus_fontSize).toInt();

    pbs.serialType = CONF_GET_PROPERTY(XPaintBarStatus_serialType).toInt();
    pbs.serialNumber = CONF_GET_PROPERTY(XPaintBarStatus_serialNumber).toInt();
    pbs.serialLetter = CONF_GET_PROPERTY(XPaintBarStatus_serialLetter).toChar();
    pbs.pointType = CONF_GET_PROPERTY(XPaintBarStatus_pointType).toInt();

    pbs.paPen = CONF_GET_PROPERTY(XPaintBarStatus_paPen).value<QPen>();
    pbs.paBrush = CONF_GET_PROPERTY(XPaintBarStatus_paBrush).value<QBrush>();
}

