#include "tray.h"

#include <QAction>
#include <QCoreApplication>
#include <QIcon>
#include <QKeySequence>
#include <QShortcut>
#include <QHotkey>
#include <QTimer>
#include "../../data/configmanager.h"

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
    m_timerDelay->start(1000); // 1秒触发一次定时器
    m_timerDelay->stop();
    connect(m_timerDelay, &QTimer::timeout, this, &Tray::onCountdownTips);
}

void Tray::capture(const HotKeyType &type)
{

    qDebug() << "capture type:" << hotKeyTypeToString(type);
    if (!m_scrnShot) m_scrnShot = new ScreenShot(type);
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
}
