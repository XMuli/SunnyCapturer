#include <QApplication>
#include <QCoreApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QDebug>
#include "xapphelper.h"
#include "ui/commom/ocr/networkocr.h"

//#include "xlog.h"
#include "ui/screenshot/tray.h"
//#include "xapphelper.h"

// test
#include "data/configmanager.h"
//#include "xtextedit.h"
//#include "tips.h"
//#include <QWidget>
//#include "colorpicker.h"
//#include "ui/setting/sidesettingui.h"

//#include "ui/include/colorpicker.h"
//#include "ui/screenshot/screenshot.h"
//#include "ui/paint_bar/toolbar_level/painttoolbar.h"
//#include "ui/paint_bar/toolbar_level/paintctrlbar.h"
//#include "ui/paint_bar/paintbar.h"
//#include "ui/paint_bar/pin/pinwidget.h"

//#include <qt_windows.h>


int main(int argc, char *argv[])
{
//    qInstallMessageHandler(XMessageOutput);
//    qSetMessagePattern("[%{type}] [%{file} %{line}] %{message} ");
#if(QT_VERSION > QT_VERSION_CHECK(5,6,0))
    qApp->setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    // 因多处使用 QSettings，故声明组织等信息
    QCoreApplication::setOrganizationName(QStringLiteral("XMuli"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("github.com/XMuli"));
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false); // fix: 默认情况下，当关闭最后一个窗口时，Qt 应用程序会自动退出

#ifdef _MSC_VER
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);//注冊异常捕获函数
#endif


    QString uniqueKey = "SunnyUniqueKey"; // 使用唯一的标识符来创建共享内存和系统信号量
    QSharedMemory sharedMemory;
    sharedMemory.setKey(uniqueKey);

    // 尝试附加到现有的共享内存并分离
    if (sharedMemory.attach()) {
        sharedMemory.detach();
    }

    // 尝试创建共享内存，如果已经存在，表示已经有一个实例在运行, 判断是为了确保在同一台计算机上只能运行一个相同实例的程序。
    if (!sharedMemory.create(1)) {
        qDebug() << "There is already an instance of the application running (by QSharedMemory)!";
        return 1;
    }

//    int* ptr = nullptr; // 尝试访问空指针
//    *ptr = 42; // 这将导致崩溃

//    QWidget* ptr = nullptr;
//    ptr->show();



    // 创建系统信号量, 再尝试获取系统信号量，如果已经被其他实例持有，程序就退出, 判断是为了确保在多个进程同时启动时，只有一个进程能够继续执行。QSystemSemaphore用于创建系统信号量，如果系统信号量已经被其他实例持有（比如由于上一次程序异常退出导致信号量未被释放），则acquire函数会返回false，
    QSystemSemaphore systemSemaphore(uniqueKey, 1, QSystemSemaphore::Open);
    if (!systemSemaphore.acquire()) {
        qDebug() << "There is already an instance of the application running (by QSystemSemaphore)!";
        return 1;
    }

    CONF_MANAGE; //.writeToFile();
    initPaintBarStatus(CONF_MANAGE.m_paintBarStatus);
    COMM.loadTranslation("");

    TRAY; // 启动托盘

    NetworkOCR networkOCR;
    networkOCR.sendBaiDuOcrTextRequest("C:/Users/Venn/Desktop/Sunny_20231030_114240.png");
//    networkOCR.sendYouDaoOcrTranslateRequest();



//    CONF_MANAGE.setProperty("XGeneral_language", "test");
//    qDebug() << "--->" << CONF_MANAGE.property("XGeneral_language").toString();
//    ConfigManager::instance().writeToFile();
//    connect(m_widget, &XTextEdit::textUpdated, this, &TextTool::updateText);

//    QString text = "27";
//    Tips tips(text, TipsType::TT_point_changed_tips);
//    tips.show();

//    Tips tips2("1200, 1000, 1600 * 900", TipsType::TT_picked_rect_tips);
//    tips2.show();


//    PaintToolBar paintToolBar(Qt::Vertical); // Horizontal Vertical
//    paintToolBar.show();

//    PaintCtrlBar paintCtrlBar(Qt::Vertical);
//    paintCtrlBar.show();

//    PaintBar paintbar(Qt::Horizontal); // Horizontal Vertical
//    paintbar.show();
//    paintbar.transposePaintBar(false);

//    PinWidget pinWidget;
//    pinWidget.show();

//    SideSettingUI w;
//    w.show();
//    ColorPicker colorPicker(QSize(100, 100), ColorPickerType::CT_grid_horizontal);
//    colorPicker.setColorPickerType(ColorPickerType::CT_grid_horizontal);
//    colorPicker.show();
//    AbsSettingUI absW;
//    absW.show();
//    ResetUI resetUI;
//    resetUI.show();

    // 释放系统信号量
    systemSemaphore.release();
    return a.exec();
}
