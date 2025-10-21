// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef TRAY_H
#define TRAY_H

#include <QMenu>
#include <QObject>
#include <QPointer>
#include <QSystemTrayIcon>
#include "communication.h"
#include "screenshot.h"
#include "tips.h"
#include "../setting/sidesettingui.h"
#include "../../common/data_analytics/dbanalytics.h"

#define TRAY Tray::instance()

class Tray : public QObject
{
    Q_OBJECT
public:
    void capture(const HotKeyType& type = HotKeyType::HKT_capture);  // 实际的调用函数
    void setAppFont(const QString& tFont);
    void loadCustomQss(const QString &path);

public slots:
    void onCapture();
    void onSetting();
    void onAbout();
    void onRestart();
    void onTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void onCountdownTips();
    void onLanguageChange(const QString qmName);
    void onShowSystemMessagebox(const QString &title, const QString &msg, const int &msecs = 6000); // 系统消息通知

private:
    void init();
    const bool isSendUserData();

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
    DbAnalytics                      m_dbAnalytics;
};

#endif // TRAY_H
