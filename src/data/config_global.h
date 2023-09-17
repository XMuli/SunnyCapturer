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
constexpr const char* XGeneral_log_level("log_level");
constexpr const char* XGeneral_font("font");
constexpr const char* XGeneral_autostart("autostart");

// Interface
constexpr const char* XInterface_style("style");
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
constexpr const char* XHotkeys_custiom_capture("custiom_capture");

#endif // CONFIG_GLOBAL_H
