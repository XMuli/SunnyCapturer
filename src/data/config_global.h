// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef CONFIG_GLOBAL_H
#define CONFIG_GLOBAL_H

#include <cstring>
#include <QString>

constexpr const char* XGeneral = "General";
constexpr const char* XInterface = "Interface";
constexpr const char* XOutput = "Output";
constexpr const char* XPin = "Pin";
constexpr const char* XHotkeys = "Hotkeys";
constexpr const char* XTokens = "Tokens";
constexpr const char* XAbout = "About";

// General
constexpr const char* XGeneral_language("language");
constexpr const char* XGeneral_themes("themes");
constexpr const char* XGeneral_log_level("log_level");
constexpr const char* XGeneral_font("font");
constexpr const char* XGeneral_autostart("autostart");

// Interface
constexpr const char* XInterface_style("style");
constexpr const char* XInterface_orientation("orientation");
constexpr const char* XInterface_highlight("highlight");
constexpr const char* XInterface_border_width("border_width");
constexpr const char* XInterface_crosshair("crosshair");
constexpr const char* XInterface_crosshair_width("crosshair_width");

constexpr const char* XInterface_custom_size_enable("custom_size_enable");
constexpr const char* XInterface_topleft_enable("topleft_enable");
constexpr const char* XInterface_size_enable("size_enable");
constexpr const char* XInterface_delay_enable("delay_enable");
constexpr const char* XInterface_custom_rect_left("custom_rect_left");
constexpr const char* XInterface_custom_rect_top("custom_rect_top");
constexpr const char* XInterface_custom_rect_width("custom_rect_width");
constexpr const char* XInterface_custom_rect_height("custom_rect_height");
constexpr const char* XInterface_custom_dealy("custom_dealy");

constexpr const char* XInterface_acrylic_effect("acrylic_effect");
constexpr const char* XInterface_auto_detect_windows("auto_detect_windows");
constexpr const char* XInterface_auto_copy_to_clipbaoard("auto_copy_to_clipbaoard");
constexpr const char* XInterface_crosshair_show("crosshair_show");

// Output
constexpr const char* XOutput_image_quailty("image_quailty");
constexpr const char* XOutput_flie_name("flie_name");
constexpr const char* XOutput_config_path("config_path");
constexpr const char* XOutput_quick_save_enable("quick_save_enable");
constexpr const char* XOutput_quick_save_path("quick_save_path");
constexpr const char* XOutput_auto_save_enable("auto_save_enable");
constexpr const char* XOutput_auto_save_path("auto_save_path");

// Pin
constexpr const char* XPin_opacity("opacity");
constexpr const char* XPin_maximum_size("maximum_size");

// Hotkeys
constexpr const char* XHotkeys_capture("capture");
constexpr const char* XHotkeys_delay_capture("delay_capture");
constexpr const char* XHotkeys_custom_capture("custom_capture");

// Tokens
constexpr const char* XTokens_youdao_app_id("youdao_app_id");
constexpr const char* XTokens_youdao_secret_key("youdao_secret_key");
constexpr const char* XTokens_iamge_translate_from_youdao("iamge_translate_from_youdao");
constexpr const char* XTokens_iamge_translate_to_youdao("iamge_translate_to_youdao");
constexpr const char* XTokens_iamge_translate_from_baidu("iamge_translate_from_baidu");
constexpr const char* XTokens_iamge_translate_to_baidu("iamge_translate_to_baidu");
constexpr const char* XTokens_baidu_api_key("baidu_api_key");
constexpr const char* XTokens_baidu_secret_key("baidu_secret_key");
constexpr const char* XTokens_ocr_channel("ocr_channel");
constexpr const char* XTokens_iamge_translate_channel("iamge_translate_channel");

// XOtherControl
constexpr const char* XOtherControl = "XOtherControl";
constexpr const char* XOtherControl_blur_effect_adius("blur_effect_adius");  // 仅提供配置文件修改
constexpr const char* XOtherControl_highlight_iridescence("highlight_iridescence");
constexpr const char* XOtherControl_crosshair_iridescence("crosshair_iridescence");
constexpr const char* XOtherControl_show_develop_ui_log("show_develop_ui_log");

// XOtherData
constexpr const char* XOtherData("XOtherData");
constexpr const char* XOtherData_manual_save_image_dir("manual_save_image_dir");
constexpr const char* XOtherData_detection_min_windows_level_depth("detection_min_windows_level_depth");  // 自动检测窗口的深度， flase-最大的窗口， true-其子窗口

// XPaintBarStatus
constexpr const char* XPaintBarStatus = "XPaintBarStatus";
constexpr const char* XPaintBarStatus_rect = "rect";
constexpr const char* XPaintBarStatus_ellipse = "ellipse";
constexpr const char* XPaintBarStatus_arrow = "arrow";
constexpr const char* XPaintBarStatus_penciler = "penciler";
constexpr const char* XPaintBarStatus_marker_pen = "marker_pen";
constexpr const char* XPaintBarStatus_mosaic = "mosaic";
constexpr const char* XPaintBarStatus_text = "text";
constexpr const char* XPaintBarStatus_serial = "serial";

constexpr const char* XPaintBarStatus_rectType = "rectintType";
constexpr const char* XPaintBarStatus_ellipseType = "ellipseType";
constexpr const char* XPaintBarStatus_arrowType = "arrowType";
constexpr const char* XPaintBarStatus_marker_penType = "marker_penType";
constexpr const char* XPaintBarStatus_mosaicType = "mosaicType";
constexpr const char* XPaintBarStatus_pixelatedMosaic = "pixelatedMosaic";
constexpr const char* XPaintBarStatus_smoothMosaic = "smoothMosaic";
constexpr const char* XPaintBarStatus_textBold = "textBold";
constexpr const char* XPaintBarStatus_textItalic = "textItalic";
constexpr const char* XPaintBarStatus_textOutline = "textOutline";
constexpr const char* XPaintBarStatus_textStrikeout = "textStrikeout";
constexpr const char* XPaintBarStatus_textUnderline = "textUnderline";
constexpr const char* XPaintBarStatus_fontFamily = "fontFamily";
constexpr const char* XPaintBarStatus_fontSize = "fontSize";
constexpr const char* XPaintBarStatus_serialType = "serialType";
constexpr const char* XPaintBarStatus_serialNumber = "serialNumber";
constexpr const char* XPaintBarStatus_serialLetter = "serialLetter";
constexpr const char* XPaintBarStatus_pointType = "point";
constexpr const char* XPaintBarStatus_paPen = "pen";
constexpr const char* XPaintBarStatus_paBrush = "brush";

#endif // CONFIG_GLOBAL_H
