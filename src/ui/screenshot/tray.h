#ifndef TRAY_H
#define TRAY_H

#include <QMenu>
#include <QObject>
#include <QPointer>
#include <QSystemTrayIcon>
#include "screenshot.h"
#include "tips.h"
#include "../setting/sidesettingui.h"

#define TRAY Tray::instance()

class Tray : public QObject
{
    Q_OBJECT

private:
    void init();

public slots:
    void onCapture();
    void onSetting();
    void onTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void onCountdownTips();

public:
    static Tray &instance() {
        static Tray instance;
        return instance;
    };

    Tray(const Tray&) = delete;
    Tray& operator=(const Tray&) = delete;
    Tray(Tray&&) = delete;
    Tray& operator=(Tray&&) = delete;

protected:
    explicit Tray(QObject *parent = nullptr);
    virtual ~Tray();

private:
    QPointer<ScreenShot>             m_scrnShot;
    QPointer<SideSettingUI>          m_setting;
    QPointer<QMenu>                  m_trayMenu;
    QPointer<QSystemTrayIcon>        m_trayIcon;
    QPointer<Tips>                   m_countdownTips;  // 倒计时预览
    QPointer<QTimer>                 m_timerDelay;
    double                           m_remainingSeconds;
};

#endif // TRAY_H
