#include <QApplication>
#include "colorpicker.h"
#include "ui/setting/sidesettingui.h"

#include "ui/include/colorpicker.h"
#include "ui/screenshot/screenshot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    SideSettingUI w;
//    w.show();

    ScreenShot screenShot;
    screenShot.capture();
    screenShot.show();
//    ColorPicker colorPicker;
//    colorPicker.setColorPickerType(ColorPickerType::Grid);
//    colorPicker.show();
//    AbsSettingUI absW;
//    absW.show();
//    ResetUI resetUI;
//    resetUI.show();
    return a.exec();
}
