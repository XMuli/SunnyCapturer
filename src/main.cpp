// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

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
#include "data/configjson.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QUrlQuery>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QDebug>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QUrl>

#include "aboutinfo.h"
#include "xmonitorlabel.h"


//#include "easylogging++.h"
//INITIALIZE_EASYLOGGINGPP

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


    // AboutInfo* monitorInfo = new AboutInfo();
    // monitorInfo->show();


    // XMonitorLabel* lab = new XMonitorLabel();
    // lab->renderMonitorToPixmap();
    // lab->resize(lab->fixSize());
    // lab->show();
//     // 从文件加载配置文件
//     const QString& path = qApp->applicationDirPath() + "/log.conf";
//     el::Configurations conf(path.toStdString().c_str());

//     el::Loggers::reconfigureLogger("default", conf);    // 为单独一个级别的Logger配置
//     el::Loggers::reconfigureAllLoggers(conf);           // 为全部的Logger配置
//     el::Loggers::setDefaultConfigurations(conf);        // Now all the loggers will use configuration from file
//     // 这样后注册的所有Logger，都是默认配置，而无需重复配置

//     el::Loggers::addFlag(el::LoggingFlag::AutoSpacing);
//     el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
//     el::Loggers::addFlag(el::LoggingFlag::ImmediateFlush);
// //    el::Loggers::setLoggingLevel(el::Level::Debug);

//     START_EASYLOGGINGPP(argc, argv);  //

//     QRect rect(0, 1, 2, 3);

//     LOG(INFO) << "My first info log using default logger" << "other";
// //    std::cout.flush(); // 刷新输出缓冲区
// //    el::Loggers::flushAll();
//     LOG(INFO) << uniqueKey;
//     LOG(INFO) << rect;
// //    el::Loggers::flushAll();
//     qDebug() << " qDebug - My first info log using default logger" << rect;


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

    // 创建系统信号量, 再尝试获取系统信号量，如果已经被其他实例持有，程序就退出, 判断是为了确保在多个进程同时启动时，只有一个进程能够继续执行。QSystemSemaphore用于创建系统信号量，如果系统信号量已经被其他实例持有（比如由于上一次程序异常退出导致信号量未被释放），则acquire函数会返回false，
    QSystemSemaphore systemSemaphore(uniqueKey, 1, QSystemSemaphore::Open);
    if (!systemSemaphore.acquire()) {
        qDebug() << "There is already an instance of the application running (by QSystemSemaphore)!";
        return 1;
    }

#if defined (Q_OS_LINUX)
    CJ.initJsonToFile();          // fitst time init write to linux "~/.config/Sunny"
#endif

    CJ.readFromFile();
    CJ.initOutputDefaulValue();   // 完整的 m_j = 读取 .josn + 填充为空 path 的一些路径
    CJ.writeToFile();             // 写入完整的 m_j
    // CJ.m_cd.cdReadFromFile();  // 恢复上次一二级菜单栏的状态
    // cdWritToFile(CJ.m_cd);     // 写入配置文件中


    COMM.loadTranslation("");
    TRAY; // 启动托盘

    // 释放系统信号量
    systemSemaphore.release();
    return a.exec();
}
