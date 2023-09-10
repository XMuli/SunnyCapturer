#include <QApplication>
#include "xlog.h"
#include "ui/screenshot/tray.h"

// test
#include "tips.h"
#include <QWidget>
#include "colorpicker.h"
#include "ui/setting/sidesettingui.h"

#include "ui/include/colorpicker.h"
#include "ui/screenshot/screenshot.h"
#include "ui/paint_bar/painttoolbar.h"
#include "ui/paint_bar/paintctrlbar.h"


int main(int argc, char *argv[])
{
//    qInstallMessageHandler(XMessageOutput);
#if(QT_VERSION > QT_VERSION_CHECK(5,6,0))
    qApp->setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    // 因多处使用 QSettings，故声明组织等信息
    QCoreApplication::setOrganizationName(QStringLiteral("XMuli"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("github.com/XMuli"));

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false); // fix: 默认情况下，当关闭最后一个窗口时，Qt 应用程序会自动退出
    TRAY; // 启动托盘


//    QString text = "27";
//    Tips tips(text, TipsType::TT_point_changed_tips);
//    tips.show();

//    Tips tips2("1200, 1000, 1600 * 900", TipsType::TT_picked_rect_tips);
//    tips2.show();


//    PaintToolBar paintToolBar(Qt::Horizontal); // Horizontal Vertical
//    paintToolBar.show();

//    PaintCtrlBar paintCtrlBar(Qt::Horizontal);
//    paintCtrlBar.show();

//    SideSettingUI w;
//    w.show();
//    ColorPicker colorPicker(QSize(100, 100), ColorPickerType::CT_grid_horizontal);
//    colorPicker.setColorPickerType(ColorPickerType::CT_grid_horizontal);
//    colorPicker.show();
//    AbsSettingUI absW;
//    absW.show();
//    ResetUI resetUI;
//    resetUI.show();
    return a.exec();
}
