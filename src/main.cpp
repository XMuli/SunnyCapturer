#include <QApplication>
#include "ui/screenshot/tray.h"

// test
#include <QWidget>
#include "colorpicker.h"
#include "ui/setting/sidesettingui.h"

#include "ui/include/colorpicker.h"
#include "ui/screenshot/screenshot.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false); // fix: 默认情况下，当关闭最后一个窗口时，Qt 应用程序会自动退出
    TRAY; // 启动托盘

//    SideSettingUI w;
//    w.show();
//    ColorPicker colorPicker;
//    colorPicker.setColorPickerType(ColorPickerType::Grid);
//    colorPicker.show();
//    AbsSettingUI absW;
//    absW.show();
//    ResetUI resetUI;
//    resetUI.show();
    return a.exec();
}
