// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "tray.h"

#include <QAction>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QIcon>
#include <QKeySequence>
#include <QShortcut>
#include <QHotkey>
#include <QTimer>
#include <QFont>
#include <QProcess>
#include "communication.h"
#include "aboutinfo.h"

void Tray::init()
{
    qApp->setStyleSheet(QString("QToolTip { background-color: %1; border: 1px solid black; }").arg(QPalette().color(QPalette::Window).name()));
    setAppFont("");
    QAction* capture = new QAction(this);
    QAction* setting = new QAction(this);
    QAction* about   = new QAction(this);
    QAction* restart = new QAction(this);
    QAction* quit    = new QAction(this);
    capture->setObjectName("actCapture");
    setting->setObjectName("actSetting");
    about->setObjectName("actAbout");
    restart->setObjectName("actRestart");
    quit->setObjectName("actQuit");

    m_trayMenu->addAction(capture);
    m_trayMenu->addAction(setting);
    m_trayMenu->addAction(about);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(restart);
    m_trayMenu->addAction(quit);

    QString suffix = ".svg";
#ifdef Q_OS_LINUX
    suffix = ".png";
#endif

    m_trayIcon->setIcon(QIcon(":/resources/logo/logo" + suffix));

    m_trayIcon->setContextMenu(m_trayMenu);

    onLanguageChange("");
    m_trayIcon->show();
    m_timerDelay->start(1000); // 1秒触发一次定时器
    m_timerDelay->stop();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &Tray::onTrayIcon);
    connect(capture, &QAction::triggered, this, &Tray::onCapture);
    connect(setting, &QAction::triggered, this, &Tray::onSetting);
    connect(about, &QAction::triggered, this, &Tray::onAbout);
    connect(restart, &QAction::triggered, this, &Tray::onRestart);
    connect(quit, &QAction::triggered, [](){ qApp->quit(); });
    connect(m_timerDelay, &QTimer::timeout, this, &Tray::onCountdownTips);
    connect(&COMM, &Communication::sigLanguageChange, this, [this]() { onLanguageChange("");});
    connect(&COMM, &Communication::sigShowSystemMessagebox, this, &Tray::onShowSystemMessagebox);

#ifdef Q_OS_WIN  // Ensure proper removal of tray icon when program quits on Windows.
    connect(qApp, &QCoreApplication::aboutToQuit, m_trayIcon, &QSystemTrayIcon::hide);
#endif

#ifdef QT_DEBUG
#else
    m_dbAnalytics.sendData("start");
#endif

}

// // 是否满足发送时间： 时间至少属于第二天就行
const bool Tray::isSendUserData()
{
    QString format = "yyyy-MM-dd HH:mm:ss";
    QString lastTimeStr = CJ_GET_QSTR("advanced.non_ui_user_experience.time");
    if (lastTimeStr.isEmpty())  // 如果 lastTimeStr 为空，直接返回 true
        return true;

    // 获取当前时间的QDateTime对象
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDateTime lastDateTime = QDateTime::fromString(lastTimeStr, format);

    // 检查解析是否成功
    if (!lastDateTime.isValid()) {
        qWarning() << "Invalid date format.";
        return false;
    }

    // 计算日期差异
    QDate lastDate = lastDateTime.date();
    QDate currentDate = currentDateTime.date();

    // 检查当前时间是否已经跨过了第二天的00:00
    if (currentDate > lastDate)
        return true;

    return false;
}

void Tray::loadCustomQss(const QString &path)
{
    QString css = path.isEmpty() ? "" : path;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        css = in.readAll();
    } else {
        qDebug() << "custom qss is open fail, file path:" << path;
    }

    if (m_setting) m_setting->setStyleSheet(css);
    if (m_trayMenu) m_trayMenu->setStyleSheet(css);
    if (m_countdownTips) m_countdownTips->setStyleSheet(css);
//    qApp->setStyleSheet(css);

//    if (m_scrnShot)
//        m_scrnShot->setStyleSheet("");
}

void Tray::setAppFont(const QString &tFont)
{
    QStringList list = tFont.isEmpty() ? CJ_GET_QSTR("general.font").split(",") : tFont.split(",");
    if (list.size() < 2) {
        list .clear();

    #if defined(Q_OS_WIN)
        list << "Microsoft YaHei" << "9";
    #elif defined(Q_OS_LINUX)
        list << "WenQuanYi Micro Hei" << "9";
    #elif defined(Q_OS_MAC)
        list << "PingFang SC" << "11";
    #endif

    }
    const QFont font(list.at(0), list.at(1).toInt());
    qApp->setFont(font);
    m_trayMenu->setFont(font);
}

void Tray::capture(const HotKeyType &type)
{
    qDebug() << "capture type:" << hotKeyTypeToString(type);

    const QString szOrie = CJ_GET_QSTR("interface.orientation");
    Qt::Orientation orie = Qt::Horizontal;
    if (szOrie == "Horizontal") orie = Qt::Horizontal;
    else if (szOrie == "Vertical") orie = Qt::Vertical;
    if (!m_scrnShot) m_scrnShot = new ScreenShot(type, orie);
    const auto& customSizeEnable = CJ_GET("interface.custom_size_enable").get<bool>();
    const auto& delayEnable = CJ_GET("interface.delay_enable").get<bool>();
    const double& s = CJ_GET("interface.custom_dealy").get<double>();
    // qDebug() << "------------>customSizeEnable:" << customSizeEnable << "delayEnable:" << delayEnable << "s:" << s;

    auto delayAndCustomFunction = [this, &customSizeEnable, &delayEnable, &s]() {

        if (customSizeEnable && delayEnable) {
            QScreen* scrn = qGuiApp->screenAt(QCursor::pos());
            if (!scrn) scrn = qGuiApp->primaryScreen();
            m_remainingSeconds = CJ_GET("interface.custom_dealy").get<double>();
            m_countdownTips->setText(QString::number(m_remainingSeconds));
            m_countdownTips->move(scrn->geometry().center() - QPoint(m_countdownTips->width() / 2, m_countdownTips->height() / 2));
            m_countdownTips->show();
            m_timerDelay->stop();
            m_timerDelay->start();

            QTimer::singleShot(s * 1000, this, [this](){
                m_countdownTips->hide();
                m_scrnShot->capture();
            });
        }
    };

    // 进入的类型
    if (type == HotKeyType::HKT_capture  || type == HotKeyType::HKT_ocr_capture  || type == HotKeyType::HKT_image_transltae_capture) {
        m_scrnShot->capture();
    } else if (type == HotKeyType::HKT_delay_capture || type == HotKeyType::HKT_custom_capture) {
        delayAndCustomFunction();
    } else {
        qDebug() << "type is empty!";
    }

    if(!m_scrnShot->isActiveWindow())
        m_scrnShot->activateWindow();

    if (m_scrnShot->actionType() == ActionType::AT_picking_detection_windows_rect)
        m_scrnShot->startEnumWindowsRect();
    m_scrnShot->showMagnifyingGlass();

#ifdef QT_DEBUG
#else
    if (isSendUserData()) m_dbAnalytics.sendData(hotKeyTypeToString(type).toStdString().data());
#endif
}

void Tray::onCapture()
{
    capture(); // 套一层，方便兼容 QAction 发射的信号
}

void Tray::onSetting()
{
    if (!m_setting) m_setting = new SideSettingUI();
    // m_setting->adjustSize();
    m_setting->show();
}

void Tray::onAbout()
{
    COMM.showBuildInfoWidget();
}

void Tray::onRestart()
{
    const QString& path = qApp->applicationFilePath();
    QProcess::startDetached(path, QStringList());
    QCoreApplication::quit();
}

void Tray::onTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    if (QSystemTrayIcon::Trigger == reason) {  //  鼠标单击
        onCapture();
    }
}

void Tray::onCountdownTips()
{
    m_countdownTips->setText(QString::number(--m_remainingSeconds));
    m_countdownTips->update();
    if (m_remainingSeconds < 0) {
        m_timerDelay->stop();
        m_countdownTips->hide();
    }
}

void Tray::onLanguageChange(const QString qmName)
{
    Q_UNUSED(qmName)
    QAction * actCapture = findChild<QAction*>("actCapture");
    QAction * actSetting = findChild<QAction*>("actSetting");
    QAction * actAbout   = findChild<QAction*>("actAbout");
    QAction * actRestart = findChild<QAction*>("actRestart");
    QAction * actQuit = findChild<QAction*>("actQuit");

    const auto& text = QString("%1 %2\n"
                               "%3: %4\n"
                               "%5: %6\n"
                               "%7: %8\n"
                               "%9: %10\n"
                               "%11: %12\n")
                           .arg(XPROJECT_NAME)
                           .arg(tr("Screenshot"))
                           .arg(tr("Capture"))
                           .arg(CJ_GET_QSTR("hotkeys.capture"))
                           .arg(tr("Delay Capture"))
                           .arg(CJ_GET_QSTR("hotkeys.delay_capture"))
                           .arg(tr("Custom Capture"))
                           .arg(CJ_GET_QSTR("hotkeys.custom_capture"))
                           .arg(tr("OCR Capture"))
                           .arg(CJ_GET_QSTR("hotkeys.ocr_capture"))
                           .arg(tr("Image Translate Capture"))
                           .arg(CJ_GET_QSTR("hotkeys.image_transltae_capture"));

    m_trayIcon->setToolTip(text);
    if (actCapture) actCapture->setText(tr("Capture"));
    if (actSetting) actSetting->setText(tr("Setting"));
    if (actAbout)   actAbout->setText(tr("About"));
    if (actRestart) actRestart->setText(tr("Restart"));
    if (actQuit) actQuit->setText(tr("Quit"));
}

void Tray::onShowSystemMessagebox(const QString &title, const QString &msg, const int& msecs)
{
    m_trayIcon->showMessage(title, msg, QIcon(":/resources/logo/logo.svg"), msecs);
}

Tray::Tray(QObject *parent)
    : QObject(parent)
    , m_scrnShot(nullptr)
    , m_setting(nullptr)
    , m_trayMenu(new QMenu())
    , m_trayIcon(new QSystemTrayIcon(this))
    , m_countdownTips(new Tips("", TipsType::TT_countdown_tips, qobject_cast<QWidget*>(this)))
    , m_timerDelay(new QTimer(this))
    , m_remainingSeconds(0)
{
    m_countdownTips->hide();
    m_countdownTips->setWindowFlag(Qt::WindowStaysOnTopHint, true);
    init();
}

Tray::~Tray()
{
    if (m_scrnShot) m_scrnShot->deleteLater();
    if (m_setting) m_setting->deleteLater();
    if (m_trayMenu) m_trayMenu->deleteLater();
    if (m_trayIcon) m_trayIcon->deleteLater();
    CJ.onSyncToFile();
}
