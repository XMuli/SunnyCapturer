// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef TRAY_H
#define TRAY_H

#include <QObject>
#include <QHotkey>
#include <QPointer>
#include <QSystemTrayIcon>
#include "screenshot.h"

class QSystemTrayIcon;
class QAction;
class QMenu;
class QHotkey;
class PrefManage;

class Tray : public QObject
{
    Q_OBJECT
public:
	static Tray& instance();
    std::vector<std::tuple<QHotkey*, QString, QString, CaptureHelper::CaptureType>> getVHotKeys() const;

private:
    explicit Tray(QObject* parent = nullptr);
    virtual ~Tray();
	void init();
    void initGlobalHotKeys();

public slots:
    void onScrnShot();
    void onPreference(bool checked);
    void onKeySequenceChanged(const QKeySequence& keySequence);

    void onNotificQuickSave(const bool bSaveOk, const QString& pathName);
    void onNotificHotkeyRegisteredFail(std::map<const QString, const QString> map);

    void onActivated(QSystemTrayIcon::ActivationReason reason);
    // tabGeneral
    //void onLanuageChange(const QString& language);
    //void onSelfStart(int sta);
    //void onAsAdmin(int sta);
    //void onAutoCheck(int sta);
    //void onUpdate();

private:
    QPointer<ScreenShot> m_scrnShot;               // 前台截图 UI
    QPointer<PrefManage> m_prefManage;              // 偏好设置 UI
    QPointer<QMenu>      m_trayMenu;
    QPointer<QSystemTrayIcon> m_trayIcon;

    // hkPtr, "F6", tr("Active Window"), objectName/tupe: SST_ActionWindow
    std::vector<std::tuple<QHotkey*,QString, QString, CaptureHelper::CaptureType>> m_vHotKeys;
};

#endif // TARY_H
