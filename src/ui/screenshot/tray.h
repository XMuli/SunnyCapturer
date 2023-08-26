#ifndef TRAY_H
#define TRAY_H

#include <QMenu>
#include <QObject>
#include <QPointer>
#include <QSystemTrayIcon>
#include "screenshot.h"
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
    virtual ~Tray() = default;

private:
    QPointer<ScreenShot>             m_scrnShot;
    QPointer<SideSettingUI>          m_setting;
    QPointer<QMenu>                  m_trayMenu;
    QPointer<QSystemTrayIcon>        m_trayIcon;
};

#endif // TRAY_H
