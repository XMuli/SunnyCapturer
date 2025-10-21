// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include <QApplication>
#include <QDir>
#include <QPushButton>
#include <QDebug>
#include <QtGlobal>
#include <QObject>
#include <QStyleFactory>
#include <QLockFile>
#include <QMessageBox>

// test
#include <iostream>
#include <QPixmap>
#include <QTranslator>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>
#include <QGuiApplication>
#include <QSystemTrayIcon>
#include <QSystemTrayIcon>
#include "./screen/tray.h"
#include "preference/prefmanage.h"
#include "qmetaobject.h"
#include "tool/pin/pinwidget.h"
#include "widget/xroundwidget.h"
#include "tool/parameterbar.h"
#include "tool/selectbar.h"
#include "core/xlog.h"
#include "tool/base/blurwidget.h"
#include "screen/datamaid.h"

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(CustomOutputMessage);
    qInstallMessageHandler(nullptr);

    // 高分屏四种方案 https://blog.csdn.net/qq_33154343/article/details/108905279
#if(QT_VERSION > QT_VERSION_CHECK(5,6,0))
//    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);  // 2K、4K 2@ 倍；获取的分辨率 4K 下实际为 /2 后。 此行需在 QApplication a(argc,argv);前面
//    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);    // 控制图片缩放质量，svg 的图片不会模糊在 4K 上。   此行无需在 QApplication a(argc,argv);前面
    qApp->setAttribute(Qt::AA_UseHighDpiPixmaps);  // 上面一行没有生效，但是此行可以生效
#endif

    // 因多处使用 QSettings，故声明组织等信息
    QCoreApplication::setOrganizationName(QStringLiteral("XMuli"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("github.com/XMuli"));
    QCoreApplication::setApplicationName(QStringLiteral(_PROJECT_NAME));
    QCoreApplication::setApplicationVersion(_PROJECT_VERSION);

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);

    QString filePath = QApplication::applicationDirPath() + "/Flipped.temp.lock";
    QLockFile* lockFile = new QLockFile(filePath);
    bool isLock = lockFile->isLocked();
    qDebug() << "filePath:" << filePath << "  isLock:" << isLock;

    if (!lockFile->tryLock(100)) {
        QMessageBox msgBox(QMessageBox::Warning, QObject::tr("Warning"), QObject::tr("The application is already running.\nAllowed to run only one instance of the application."));
        msgBox.exec();
        return 0;
    }

    if (lockFile)
        delete lockFile;

    DATAMAID->setRunLanguage();

    Tray::instance();


//    QStringList listStyle = QStyleFactory::keys();
//    foreach(QString val, listStyle)
//      qDebug()<<val<<"  ";

//#ifdef Q_OS_MAC
//    const QString value = "macintosh";  // macintosh Windows Fusion
//    QApplication::setStyle(value);
//#else
//#endif


//    SelectBar* t1 = new SelectBar(Qt::Horizontal);
//    t1->setWindowFlags(Qt::FramelessWindowHint);
////    t1 ->setBlurBackground(QPixmap("D:/projects/Flipped/src/p1.jpg"), 4);
//    t1->move(200, 100);
//    t1->show();

//    ParameterBar* t2 = new ParameterBar(Qt::Horizontal);
////    t2->setBlurBackground(QPixmap("D:/projects/Flipped/src/p2.jpg"), 5);
//    t2->move(200, 200);
//    t2->show();

//    ColorParaBar* t3 = new ColorParaBar(ColorParaBarMode::CPB_ParaBar, Qt::Horizontal);
////    t3->resize(400, 300);
//    t3->move(200, 300);
//    t3->show();

//    ColorParaBar* t3_1 = new ColorParaBar(ColorParaBarMode::CPB_HighLight, Qt::Horizontal);
//    //    t3->resize(400, 300);
//    t3_1->move(200, 400);
//    t3_1->show();

////    ExWidget* t4 = new ExWidget();
////    //    t3->resize(400, 300);
////    t4->move(100, 200);
////    t4->show();

//    PinWidget* w5 = new PinWidget(QPixmap("C:/Users/xmuli/Desktop/Snipaste_2022-07-16_02-13-39.png"), QRect(100, 100, 400, 400));
//    w5->show();

    return QApplication::exec();
}
