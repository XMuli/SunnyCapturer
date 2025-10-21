// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef APPELLATION_H
#define APPELLATION_H

#include <QString>

// Description: 统一存放 class PrefManage 中新建的对象的 objectName
 // perference UI config
//const QString INIT_GENERAL("General");                   // 初始化 常规
//const QString INIT_INTERFACE("Interface");               // 初始化 界面
//const QString INIT_OUTPUT("Output");                     // 初始化 输出
//const QString INIT_PIN("Pin");                           // 初始化 贴图
//const QString INIT_HOTKEYS("Hotkeys");                   // 初始化 快捷键

namespace XC {

    //tabGeneral      常规
    const QString tgFirstRun("firstRun");
    const QString tgLanuage("lanuage");
    const QString tgFont("font");
    const QString tgAutoRun("autoRun");
    const QString tgAsAdmin("asAdmin");
    const QString tgLogLevel("logLevel");
    const QString tgAutoCheckUpdate("autoCheck");  // TODO 带 _ 都是没修改的
    const QString tgAutoUpdate("autoUpdate");
    const QString tgReset("tg_reset"); // 单独添加 tg, 下同

    //tabInterface()  界面
    const QString tiBorderStyle("borderStyle");
    const QString tiBorderColor("borderColor");
    const QString tiBorderWidth("borderWidth");
    const QString tiCrosshairColor("crosshairColor");
    const QString tiCrosshairWidth("crosshairWidth");
    const QString tiSmartWindow("smartWindow");
    const QString tiCrosshair("crosshair");
    const QString tiShowCursor("showCursor");
    const QString tiAutoCopy2Clipboard("autoCopy2Clipboard");
    const QString tiReset("ti_reset");

    //tabOutput()     输出
    const QString toImageQuailty("imageQuailty");
    const QString toFileName("fileName");
    const QString toConfigPath("configPath");
    const QString toQuickSave("quickSave");
    const QString toAutoSave("autoSave");
    const QString toQuickSavePath("quickSavePath");
    const QString toAutoSavePath("autoSavePath");
    const QString toReset("to_reset");

    //tabPin()        贴图
    const QString tpWindowShadow("windowShadow");
    const QString tpOpacity("opacity");
    const QString tpMaxSize("maxSize");
    const QString tpReset("tp_reset");

    //tabHotkeys()    快捷键
    const QString thScrnCapture("scrnCapture");
    //const QString thScrollingWindow("scrolling_window");
    const QString thDelayCapture("delayCapture");
    const QString thFullScreen("fullScrnCapture");
    //const QString thFixdSize_Region("fixd_size_region");
    //const QString thPaste("paste");
    //const QString thHide_ShowAllImages("hide_show_all_images");
    //const QString thSwitchCurGroup("switch_current_group");
    const QString thReset("th_reset");

    //tabAbout()      关于
}

using namespace XC;

// 对象名、类型、命名的对象名称
#define NEW_OBJECT(_object, _type, _objectName) \
    auto _object = new _type(this); \
    if (_object) \
        _object->setObjectName(_objectName);

// 对象名、类型、命名的对象名称、可能需要显示的文本
#define NEW_OBJECT_AND_TEXT(_object, _type, _objectName, _text) \
    auto _object = new _type(_text, this); \
    if (_object) \
        _object->setObjectName(_objectName);

//// 写入配置文件 .ini 的内容
//#define WRITE_CONFIG_INI(group, key, value) \
//    SETTINGINI->beginGroup(group); \
//    SETTINGINI->setValue(key, value); \
//    SETTINGINI->endGroup();

//// 读取配置文件 .ini 的内容
//#define READ_CONFIG_INI(group, key, defValue) \
//    SETTINGINI->value("/" + group + "/" + key, defValue)

#endif // APPELLATION_H
