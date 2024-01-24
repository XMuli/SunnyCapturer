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
#include "../../data/configmanager.h"

void Tray::init()
{
    setAppFont("");
    QAction* capture = new QAction(this);
    QAction* setting = new QAction(this);
    QAction* restart = new QAction(this);
    QAction* quit = new QAction(this);
    capture->setObjectName("actCapture");
    setting->setObjectName("actSetting");
    restart->setObjectName("actRestart");
    quit->setObjectName("actQuit");

    m_trayMenu->addAction(capture);
    m_trayMenu->addAction(setting);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(restart);
    m_trayMenu->addAction(quit);

    m_trayIcon->setIcon(QIcon(":/resources/logo/logo.svg"));
    const auto& text = QString("%1 Screenshot\n"
                               "Capture: %2\n"
                               "Delay Capture: %3\n"
                               "Custom Capture: %4\n")
                           .arg(XPROJECT_NAME)
                           .arg(CONF_MANAGE.property("XHotkeys_delay_capture").toString())
                           .arg(CONF_MANAGE.property("XHotkeys_delay_capture").toString())
                           .arg(CONF_MANAGE.property("custom_capture").toString());
    m_trayIcon->setToolTip(text);
    m_trayIcon->setContextMenu(m_trayMenu);

    onLanguageChange("");
    m_trayIcon->show();
    m_timerDelay->start(1000); // 1秒触发一次定时器
    m_timerDelay->stop();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &Tray::onTrayIcon);
    connect(capture, &QAction::triggered, this, &Tray::onCapture);
    connect(setting, &QAction::triggered, this, &Tray::onSetting);
    connect(restart, &QAction::triggered, this, &Tray::onRestart);
    connect(quit, &QAction::triggered, [](){ qApp->quit(); });
    connect(m_timerDelay, &QTimer::timeout, this, &Tray::onCountdownTips);
    connect(&COMM, &Communication::sigLanguageChange, this, [this]() { onLanguageChange("");});
    connect(&COMM, &Communication::sigShowSystemMessagebox, this, &Tray::onShowSystemMessagebox);



#ifdef Q_OS_WIN  // Ensure proper removal of tray icon when program quits on Windows.
    connect(qApp, &QCoreApplication::aboutToQuit, m_trayIcon, &QSystemTrayIcon::hide);
#endif
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
    QStringList list = tFont.isEmpty() ? CONF_MANAGE.property("XGeneral_font").toString().split(",") : tFont.split(",");
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

    const QString szOrie = CONF_MANAGE.property("XInterface_orientation").toString();
    Qt::Orientation orie = Qt::Horizontal;
    if (szOrie == "Horizontal") orie = Qt::Horizontal;
    else if (szOrie == "Vertical") orie = Qt::Vertical;
    if (!m_scrnShot) m_scrnShot = new ScreenShot(type, orie);
    const auto& customSizeEnable = CONF_MANAGE.property("XInterface_custom_size_enable").toBool();
    const auto& delayEnable = CONF_MANAGE.property("XInterface_delay_enable").toBool();
    const double& s = CONF_MANAGE.property("XInterface_custom_dealy").toDouble();
    qDebug() << "------------>customSizeEnable:" << customSizeEnable << "delayEnable:" << delayEnable << "s:" << s;

    auto delayAndCustomFunction = [this, &customSizeEnable, &delayEnable, &s]() {

        if (customSizeEnable && delayEnable) {
            QScreen* scrn = qGuiApp->screenAt(QCursor::pos());
            if (!scrn) scrn = qGuiApp->primaryScreen();
            m_remainingSeconds = CONF_MANAGE.property("XInterface_custom_dealy").toDouble();
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

    if (type == HotKeyType::HKT_capture) {
        m_scrnShot->capture();
    } else if (type == HotKeyType::HKT_delay_capture || type == HotKeyType::HKT_custiom_capture) {
        delayAndCustomFunction();
    } else {
        qDebug() << "type is empty!";
    }

    if(!m_scrnShot->isActiveWindow())
        m_scrnShot->activateWindow();
}

void Tray::onCapture()
{
    capture(); // 套一层，方便兼容 QAction 发射的信号
}

void Tray::onSetting()
{
    if (!m_setting) m_setting = new SideSettingUI();
    m_setting->show();
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
    QAction * actRestart = findChild<QAction*>("actRestart");
    QAction * actQuit = findChild<QAction*>("actQuit");

    if (actCapture) actCapture->setText(tr("Capture"));
    if (actSetting) actSetting->setText(tr("Setting"));
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
    CONF_MANAGE.onSyncToFile();
}
