#include "communication.h"
#include <QApplication>
#include <QKeySequence>
#include <QLocale>
#include <QDebug>
#include <QTranslator>
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

void Communication::setAppFont(const QString &font)
{
    TRAY.setAppFont(font);
}

void Communication::loadTranslation(const QString &language)
{
    QString temp = language.isEmpty() ? CONF_MANAGE.property("XGeneral_language").toString() : language; //language.isEmpty() ? QLocale::system().name() : t;

    // 创建 QTranslator 对象
    static QTranslator* translator = nullptr;
    if (!translator) translator = new QTranslator(this);
    QString qmDir = qGuiApp->applicationDirPath() + "/translations/";
    #if defined(Q_OS_LINUX)
        qmDir = QString("/usr/local/%1/translations/").arg(XPROJECT_NAME);
    #endif

    const QString& qmName = QString("%1_%2.qm").arg(XPROJECT_NAME).arg(toLocaleName(temp));
    const QString& qmPath = qmDir + qmName;
    qDebug() << "loadTranslation qmPath:" << qmPath;
    if (translator->load(qmPath)) {  // 加载翻译文件
        qApp->installTranslator(translator);
        emit COMM.sigLanguageChange(qmName);

        CONF_MANAGE.setProperty("XGeneral_language", temp);
    }
}

void Communication::loadCustomQss(const QString &path)
{
    TRAY.loadCustomQss(path);
}

QString Communication::toLocaleName(const QString &language)
{
    const auto& map = languageMap();
    auto it = map.find(language);
    return it != map.cend() ? it->second : "";
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

std::map<QString, QString> languageMap()
{
    static  std::map<QString, QString> map = {  {"English", "en_US"}
                                              , {"简体中文", "zh_CN"}
                                              , {"繁体中文", "zh_TW"}};
    return map;
}


#include <QDir>
#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QStyleFactory>

// 获取主题的名称
std::map<const QString, const bool> themesMap()
{
    static std::map<const QString, const bool>  map;
    map.insert(std::make_pair("default", true));
    for(const auto& it : QStyleFactory::keys()) map.insert(std::make_pair(it, true));
    for (const auto& it : getQSSFileNames())  map.insert(std::make_pair(it, false));
    return map;
}

QStringList getQSSFileNames(const QString &path)
{
    QDir dir(path);
    QStringList filters;
    filters << "*.qss";
    dir.setNameFilters(filters);

    QStringList qssFileNames;
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach (const QFileInfo& fileInfo, fileInfoList) {
        QString qssFileName = fileInfo.baseName();
        qssFileNames.append(qssFileName);
    }

    return qssFileNames;
}

