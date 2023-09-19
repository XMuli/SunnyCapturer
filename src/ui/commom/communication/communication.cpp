#include "communication.h"
#include <QApplication>
#include <QKeySequence>
#include <QDebug>
#include "../../data/configmanager.h"
#include "../../ui/screenshot/tray.h"


Communication::Communication(QObject *parent)
    : QObject(parent)
    , m_hkCapture(new QHotkey(CONF_MANAGE.property("XHotkeys_capture").toString(), true, qApp))   //The hotkey will be automatically registered
    , m_hkDelayCapture(new QHotkey(CONF_MANAGE.property("XHotkeys_delay_capture").toString(), true, qApp))
    , m_hkCustiomCapture(new QHotkey(CONF_MANAGE.property("custiom_capture").toString(), true, qApp))
{
    init();
}

Communication &Communication::instance()
{
    static Communication ins;
    return ins;
}

void Communication::init()
{
    qDebug() << "m_hkCapture Is segistered:" << m_hkCapture->isRegistered()
             << "m_hkDelayCapture Is segistered:" << m_hkDelayCapture->isRegistered()
             << "m_hkCustiomCapture Is segistered:" << m_hkCustiomCapture->isRegistered();



    connect(m_hkCapture, &QHotkey::activated, this, [](){ TRAY.capture(HotKeyType::HKT_capture); });
    connect(m_hkDelayCapture, &QHotkey::activated, this, [](){ TRAY.capture(HotKeyType::HKT_delay_capture); });
    connect(m_hkCustiomCapture, &QHotkey::activated, this, [](){ TRAY.capture(HotKeyType::HKT_custiom_capture); });
}

bool Communication::resetShortcut(const QKeySequence &keySequence, const HotKeyType &type)
{
    bool ret = false;
    QHotkey* hk = nullptr;
    if (type == HotKeyType::HKT_capture) {
        hk = m_hkCapture;
    } else if (type == HotKeyType::HKT_delay_capture) {
        hk = m_hkDelayCapture;
    } else if (type == HotKeyType::HKT_custiom_capture) {
        hk = m_hkCustiomCapture;
    } else {
        qDebug() << QString("type %1 resetShortcut is empty!").arg(hotKeyTypeToString(type));
        return ret;
    }

    hk->resetShortcut();
    hk->setShortcut(keySequence, true);
    ret = hk->isRegistered();

    qDebug() << "type:" << hotKeyTypeToString(type) << "keySequence:" << keySequence.toString() << "hk->isRegistered():" << ret;

    return ret;
}

bool Communication::shortcutStatus(const HotKeyType &type) const
{
    bool ret = false;
    QHotkey* hk = nullptr;
    if (type == HotKeyType::HKT_capture) {
        hk = m_hkCapture;
    } else if (type == HotKeyType::HKT_delay_capture) {
        hk = m_hkDelayCapture;
    } else if (type == HotKeyType::HKT_custiom_capture) {
        hk = m_hkCustiomCapture;
    } else {
        qDebug() << QString("type %1 find shortcutStatus is empty!").arg(hotKeyTypeToString(type));
        return ret;
    }

    return hk->isRegistered();
}

QString hotKeyTypeToString(const HotKeyType &hotKeyType)
{
    switch (hotKeyType) {
    case HotKeyType::HKT_capture:
        return "HKT_capture";
    case HotKeyType::HKT_delay_capture:
        return "HKT_delay_capture";
    case HotKeyType::HKT_custiom_capture:
        return "HKT_custiom_capture";
    default:
        return "Unknown";
    }
}
