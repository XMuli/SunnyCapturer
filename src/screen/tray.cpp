// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "tray.h"
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfoList>
#include <QKeySequence>
#include <QHotkey>
#include <QFont>
#include <QString>
#include <QMetaEnum>
#include "../preference/prefmanage.h"
#include "../widget/xkeysequenceedit.h"
#include "../screen/drawhelper.h"
#include "../screen/datamaid.h"

/*!
 * \brief WinFullScreen::instance 单例的实现
 * \return 返回单例的引用
 * \note 问：类的static变量在什么时候初始化？函数的static变量在什么时候初始化？
 * \li 答：类的静态成员变量在类实例化之前就已经存在了，并且分配了内存。函数的static变量在执行此函数时进行初始化。
 */
Tray& Tray::instance()
{
    static Tray tray;
    return tray;
}

std::vector<std::tuple<QHotkey*, QString, QString, CaptureHelper::CaptureType>> Tray::getVHotKeys() const
{
    return m_vHotKeys;
}

Tray::Tray(QObject *parent)
    : QObject(parent)
    , m_scrnShot(nullptr)
    , m_prefManage(nullptr)
    , m_trayMenu(new QMenu())
    , m_trayIcon(new QSystemTrayIcon(this))
{
    init();
    initGlobalHotKeys();
    DATAMAID->setAutoRun();
}

Tray::~Tray()
{
    if (m_scrnShot) m_scrnShot->deleteLater();
    if (m_prefManage) m_prefManage->deleteLater();

    delete m_trayMenu;
    m_trayMenu = nullptr;
}

void Tray::init()
{
    const auto& tFont = DATAMAID->paraValue("font").value<QString>().split(',');
    QString fontFamily = "Microsoft YaHei";
    int fontPointSize = 9;
    if (tFont.size() == 2) {
        fontFamily = tFont.at(0);
        fontPointSize = tFont.at(1).toInt();
    }

    QFont font(fontFamily, fontPointSize);
    qGuiApp->setFont(font);
    m_trayMenu->setFont(font);

    QAction* srnShot = new QAction(tr("ScreenShot"), this);
    QAction* preference = new QAction(tr("Preference"), this);
    QAction* quit = new QAction(tr("Quit"), this);

    m_trayMenu->addAction(srnShot);
    m_trayMenu->addAction(preference);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(quit);

    m_trayIcon->setIcon(QIcon(":/resources/logo/logo.png"));
    m_trayIcon->setToolTip(tr(_PROJECT_NAME));
    m_trayIcon->setContextMenu(m_trayMenu);


    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &Tray::onActivated);
    connect(srnShot, &QAction::triggered, this, &Tray::onScrnShot);
    connect(preference, &QAction::triggered, this, &Tray::onPreference);
	connect(quit, &QAction::triggered, [](){qApp->quit();});

#ifdef Q_OS_WIN
    // Ensure proper removal of tray icon when program quits on Windows.
    connect(qApp, &QCoreApplication::aboutToQuit, m_trayIcon, &QSystemTrayIcon::hide);
#endif

    m_trayIcon->show();
}


void Tray::initGlobalHotKeys()
{
    m_vHotKeys = {
        std::make_tuple(nullptr, DATAMAID->paraValue(thScrnCapture).toString(), tr("Scrn Capture"), CaptureHelper::SST_ScrnCapture),
        //std::make_tuple(nullptr, "", tr("Scrolling Window"), ScrnShotType::SST_ScrollingWindow),
        std::make_tuple(nullptr, DATAMAID->paraValue(thFullScreen).toString(), tr("Full Screen"), CaptureHelper::SST_FullScrnCapture),
        std::make_tuple(nullptr, DATAMAID->paraValue(thDelayCapture).toString(), tr("Delay Capture"), CaptureHelper::SST_DelayCapture)
//        std::make_tuple(nullptr, "", tr("Fixd-Size Region"), ScrnShotType::SST_FixdSizeRegion),
//        std::make_tuple(nullptr, "", tr("Paste"), ScrnShotType::SST_Paste),
//        std::make_tuple(nullptr, "", tr("Hide/Show all images"), ScrnShotType::SST_HideShowAllImages),
//        std::make_tuple(nullptr, "", tr("Switch current group"), ScrnShotType::SST_SwitchCurrentGroup)
    };

    std::map<const QString, const QString> hotkeyRegFail;
    for (auto& it : m_vHotKeys) {
        auto& pHK = std::get<0>(it);    // QHotkey*& 指针的引用类型
        QString& hotkey = std::get<1>(it);
        QString& describe = std::get<2>(it);
        const CaptureHelper::CaptureType sst = std::get<3>(it);
        const QString strHotKey = DATAMAID->paraValue(describe).toString();  // 没有则返回 hotkey
        if (!strHotKey.isEmpty())
            hotkey = strHotKey;

        pHK =  new QHotkey(QKeySequence(hotkey), true, qApp);
        QMetaEnum enumSst = QMetaEnum::fromType<CaptureHelper::CaptureType>();
        pHK->setObjectName(enumSst.valueToKey(sst));
        connect(pHK, &QHotkey::activated, this, &Tray::onScrnShot);

        if (!pHK->isRegistered())
            hotkeyRegFail.insert(std::make_pair(describe, hotkey));
        
        //qDebug() << "pHK: " << pHK << Qt::endl
        //    << "std::get<0>(it):" << std::get<0>(it) << Qt::endl
        //    << "hotkey:" << hotkey << "    hk Is Registered:" << pHK->isRegistered() << Qt::endl;
    }
    onNotificHotkeyRegisteredFail(hotkeyRegFail);
}

void Tray::onScrnShot()
{
    // 第二次就是野指针, 故使用 QPointer 解决 https://blog.csdn.net/luoyayun361/article/details/90199081
    if (!m_scrnShot) m_scrnShot = new ScreenShot();


    QHotkey* hk = qobject_cast<QHotkey*>(sender());
    const auto act = qobject_cast<QAction*>(sender());
    if (!hk && !act) return;

    int sst = CaptureHelper::SST_ScrnCapture;
    if (hk) {
        bool ok = false;
        QMetaEnum metaSst = QMetaEnum::fromType<CaptureHelper::CaptureType>();
        sst = metaSst.keyToValue(hk->objectName().toStdString().c_str(), &ok);
    }

    if (act || sst == CaptureHelper::SST_ScrnCapture
        || sst == CaptureHelper::SST_DelayCapture
        || sst == CaptureHelper::SST_FullScrnCapture)
        m_scrnShot->launchCapture(static_cast<CaptureHelper::CaptureType>(sst));
}

void Tray::onPreference(bool checked)
{
    Q_UNUSED(checked);

    if (!m_prefManage) m_prefManage = new PrefManage();

    m_prefManage->show();
    m_prefManage->resize(800, 450);
}

void Tray::onKeySequenceChanged(const QKeySequence& keySequence)
{
    XKeySequenceEdit* editor = qobject_cast<XKeySequenceEdit*>(sender());
    if (!editor)
        return;

    for (auto& it : m_vHotKeys) {
        auto& pHK = std::get<0>(it);                                          // QHotkey*& 指针的引用类型
//        QString& hotkey = std::get<1>(it);
        CaptureHelper::CaptureType& sst = std::get<3>(it);

        QMetaEnum enumSst = QMetaEnum::fromType<CaptureHelper::CaptureType>();
        QString objName = enumSst.valueToKey(sst);
        if (editor->objectName() == objName) {
            auto prev = pHK->shortcut();
            qDebug() << "----->prev:" << prev << "   keySequence:" << keySequence;
            if (prev == keySequence || keySequence.isEmpty()) return;
            pHK->setShortcut(keySequence, true);

            if (pHK->isRegistered()) {
                editor->setStyleSheet("background-color: #98fb98;");
            } else {
                editor->setStyleSheet("background-color: #ff7f50;");
            }

            QString paraName;
            if (sst == CaptureHelper::SST_ScrnCapture) {
                paraName = thScrnCapture;
            } else if (sst == CaptureHelper::SST_DelayCapture) {
                paraName = thDelayCapture;
            } else if (sst == CaptureHelper::SST_FullScrnCapture) {
                paraName = thFullScreen;
            }

            DATAMAID->setParaValue(paraName, keySequence.toString());
            qDebug() << "#editor->keySequence():" << editor->keySequence() << "  keySequence:" << keySequence
                << "  pHK:" << pHK << "  pHK->shortcut():" << pHK->shortcut();
        }
    }
}

void Tray::onNotificQuickSave(const bool bSaveOk, const QString& pathName)
{
    const QString& title = bSaveOk ? tr("Success") : tr("Fail");
    const QString& msg = tr("Image saved to %1").arg(pathName);
    m_trayIcon->showMessage(title, msg, QIcon(":/resources/logo/logo.png"), 6000);
}

void Tray::onNotificHotkeyRegisteredFail(std::map<const QString, const QString> map)
{
    if (map.size() <= 0)
        return;

    const QString& title = tr("Fail ShotKeys");
    QString msg;
    for (const auto& it: map) {
        msg += (it.first + ": " + it.second + "\n");
        auto t = msg.lastIndexOf('\n');
        msg.left(t);
    }

    m_trayIcon->showMessage(title, msg, QIcon(":/resources/logo/logo.png"), 6000);
}

void Tray::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (QSystemTrayIcon::Trigger == reason) {  //  鼠标单击
        if (!m_scrnShot)
            m_scrnShot = new ScreenShot();

            m_scrnShot->launchCapture(CaptureHelper::SST_ScrnCapture);
    } else {

    }
}

