#include "tray.h"

#include <QAction>
#include <QCoreApplication>
#include <QIcon>
#include <QKeySequence>
#include <QShortcut>

void Tray::init()
{
    QAction* srnShot = new QAction(tr("Capture"), this);
    QAction* setting = new QAction(tr("Setting"), this);
    QAction* quit = new QAction(tr("Quit"), this);

    m_trayMenu->addAction(srnShot);
    m_trayMenu->addAction(setting);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(quit);

    m_trayIcon->setIcon(QIcon(":/resources/screenshot_ui/logo/logo.svg"));
//    m_trayIcon->setToolTip("");
    m_trayIcon->setContextMenu(m_trayMenu);


    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &Tray::onTrayIcon);
    connect(srnShot, &QAction::triggered, this, &Tray::onCapture);
    connect(setting, &QAction::triggered, this, &Tray::onSetting);
    connect(quit, &QAction::triggered, [](){ qApp->quit(); });

#ifdef Q_OS_WIN
    // Ensure proper removal of tray icon when program quits on Windows.
    connect(qApp, &QCoreApplication::aboutToQuit, m_trayIcon, &QSystemTrayIcon::hide);
#endif

    m_trayIcon->show();
}

void Tray::onCapture()
{
    if (!m_scrnShot) m_scrnShot = new ScreenShot();
    m_scrnShot->capture();

    if(!m_scrnShot->isActiveWindow())
        m_scrnShot->activateWindow();
}

void Tray::onSetting()
{
    if (!m_setting) m_setting = new SideSettingUI();
    m_setting->show();
}

void Tray::onTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    if (QSystemTrayIcon::Trigger == reason) {  //  鼠标单击
        if (!m_scrnShot) m_scrnShot = new ScreenShot();
        m_scrnShot->capture();
    }
}

Tray::Tray(QObject *parent)
    : QObject(parent)
    , m_scrnShot(nullptr)
    , m_setting(nullptr)
    , m_trayMenu(new QMenu())
    , m_trayIcon(new QSystemTrayIcon(this))
{
    init();
}

Tray::~Tray()
{
    if (m_scrnShot) m_scrnShot->deleteLater();
    if (m_setting) m_setting->deleteLater();
    if (m_trayMenu) m_trayMenu->deleteLater();
    if (m_trayIcon) m_trayIcon->deleteLater();
}
