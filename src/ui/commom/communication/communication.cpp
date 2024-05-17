// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "communication.h"
#include <QApplication>
#include <QKeySequence>
#include <QLocale>
#include <QDebug>
#include <QTranslator>
#include "aboutinfo.h"
#include "../../data/configmanager.h"
#include "../../ui/screenshot/tray.h"

Communication::Communication(QObject *parent)
    : QObject(parent)
    , m_hkCapture(new QHotkey(CJ_GET_QSTR("hotkeys.capture"), true, qApp))   //The hotkey will be automatically registered
    , m_hkDelayCapture(new QHotkey(CJ_GET_QSTR("hotkeys.delay_capture"), true, qApp))
    , m_hkCustiomCapture(new QHotkey(CJ_GET_QSTR("hotkeys.custom_capture"), true, qApp))
    , m_hkOcrCapture(new QHotkey(CJ_GET_QSTR("hotkeys.ocr_capture"), true, qApp))
    , m_hkImgTransCapture(new QHotkey(CJ_GET_QSTR("hotkeys.image_transltae_capture"), true, qApp))
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
             << "m_hkCustiomCapture Is segistered:" << m_hkCustiomCapture->isRegistered()
             << "m_hkOcrCapture Is segistered:" << m_hkOcrCapture->isRegistered()
             << "m_hkImgTransCapture Is segistered:" << m_hkImgTransCapture->isRegistered();

    connect(m_hkCapture, &QHotkey::activated, this, [](){ TRAY.capture(HotKeyType::HKT_capture); });
    connect(m_hkDelayCapture, &QHotkey::activated, this, [](){ TRAY.capture(HotKeyType::HKT_delay_capture); });
    connect(m_hkCustiomCapture, &QHotkey::activated, this, [](){ TRAY.capture(HotKeyType::HKT_custom_capture); });
    connect(m_hkOcrCapture, &QHotkey::activated, this, [](){ TRAY.capture(HotKeyType::HKT_ocr_capture); });
    connect(m_hkImgTransCapture, &QHotkey::activated, this, [](){ TRAY.capture(HotKeyType::HKT_image_transltae_capture); });
}

bool Communication::resetShortcut(const QKeySequence &keySequence, const HotKeyType &type)
{
    bool ret = false;
    QHotkey* hk = nullptr;
    if (type == HotKeyType::HKT_capture) {
        hk = m_hkCapture;
    } else if (type == HotKeyType::HKT_delay_capture) {
        hk = m_hkDelayCapture;
    } else if (type == HotKeyType::HKT_custom_capture) {
        hk = m_hkCustiomCapture;
    } else if (type == HotKeyType::HKT_ocr_capture) {
        hk = m_hkOcrCapture;
    } else if (type == HotKeyType::HKT_image_transltae_capture) {
        hk = m_hkImgTransCapture;
    } else {
        qDebug() << QString("type %1 resetShortcut is empty!").arg(hotKeyTypeToString(type));
        return ret;
    }

    hk->resetShortcut();
    if (keySequence.isEmpty())  // delete 按键，会将之前也热键取消掉
        return true;

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
    } else if (type == HotKeyType::HKT_custom_capture) {
        hk = m_hkCustiomCapture;
    } else if (type == HotKeyType::HKT_ocr_capture) {
        hk = m_hkOcrCapture;
    } else if (type == HotKeyType::HKT_image_transltae_capture) {
        hk = m_hkImgTransCapture;
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
    QString temp = language.isEmpty() ? CJ_GET_QSTR("general.language") : language; //language.isEmpty() ? QLocale::system().name() : t;

    // 创建 QTranslator 对象
    static QTranslator* translator = nullptr;
    if (!translator) translator = new QTranslator(this);
    QString qmDir = qGuiApp->applicationDirPath() + "/translations/";
    #if defined(Q_OS_LINUX)
        qmDir = QString("/usr/local/%1/translations/").arg(XPROJECT_NAME);
    #endif

    const QString& qmName = QString("%1.qm").arg(toLocaleName(temp));
    const QString& qmPath = qmDir + qmName;
    qDebug() << "loadTranslation qmPath:" << qmPath;
    if (translator->load(qmPath)) {  // 加载翻译文件
        qApp->installTranslator(translator);
        emit COMM.sigLanguageChange(qmName);
        CJ_SET("general.language", temp.toStdString());
    }
}

void Communication::loadCustomQss(const QString &path)
{
    TRAY.loadCustomQss(path);
}

void Communication::showBuildInfoWidget()
{
    static QPointer<AboutInfo> info = nullptr;
    if (!info) {
        info = new AboutInfo(nullptr);
        if (!info->isVisible()) info->show();
    }
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
    case HotKeyType::HKT_custom_capture:
        return "HKT_custom_capture";
    case HotKeyType::HKT_ocr_capture:
        return "HKT_ocr_capture";
    case HotKeyType::HKT_image_transltae_capture:
        return "HKT_image_transltae_capture";
    default:
        return "Unknown";
    }
}

std::map<QString, QString> languageMap()
{
    static  std::map<QString, QString> map = {  {"English", "en_us"}
                                              , {"简体中文", "zh_cn"}
                                              , {"繁体中文", "zh_tw"}
                                              , {"Português Brasil", "pt_br"}     // Portuguese-Brazil 葡萄牙语-巴西
    };
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



// https://ai.youdao.com/DOCSIRMA/html/trans/api/tpfy/index.html
std::map<QString, QString> languageMapCodeYouDao()
{
    static std::map<QString, QString> map = { {"auto", "auto"},
        {"中文简体", "zh-CHS"}, {"中文繁体", "zh-CHT"}, {"English", "en"}, {"日语", "ja"}, {"韩语", "ko"},
        {"俄语", "ru"}, {"德语", "de"}, {"西班牙语", "es"}, {"法语", "fr"}, {"阿拉伯语", "ar"},
        {"葡萄牙语", "pt"}, {"意大利语", "it"}, {"印度尼西亚语", "id"}, {"荷兰语", "nl"}, {"泰语", "th"},
        {"越南语", "vi"}, {"泰米尔语", "ta"}, {"土耳其语", "tr"}, {"瑞典语", "sv"}, {"挪威语", "no"},
        {"芬兰语", "fi"}, {"丹麦语", "da"}, {"希腊语", "el"}, {"捷克语", "cs"}, {"波兰语", "pl"},
        {"匈牙利语", "hu"}, {"罗马尼亚语", "ro"}, {"乌克兰语", "uk"}, {"马来语", "ms"}, {"斯洛伐克语", "sk"},
        {"克罗地亚语", "hr"}, {"爱沙尼亚语", "et"}, {"立陶宛语", "lt"}, {"斯洛文尼亚语", "sl"}, {"拉脱维亚语", "lv"},
        {"波斯尼亚语", "bs"}, {"塞尔维亚语(拉丁文)", "sr-Latn"}, {"塞尔维亚语(西里尔文)", "sr-Cyrl"}, {"阿尔巴尼亚语", "sq"},
        {"马其顿语", "mk"}, {"马耳他语", "mt"}, {"海地克里奥尔语", "ht"}, {"尼泊尔语", "ne"}, {"孟加拉语", "bn"},
        {"尼泊尔语", "ne"}, {"乌尔都语", "ur"}, {"斯瓦希里语", "sw"}, {"哈萨克语", "kk"}, {"吉尔吉斯语", "ky"},
        {"塔吉克语", "tg"}, {"土库曼语", "tk"}, {"乌兹别克语", "uz"}, {"阿塞拜疆语", "az"}, {"亚美尼亚语", "hy"},
        {"格鲁吉亚语", "ka"}, {"波斯语", "fa"}, {"希伯来语", "he"}, {"阿姆哈拉语", "am"}, {"海威语", "haw"},
        {"斐济语", "fj"}, {"塞索托语", "st"}, {"索托语", "nso"}, {"斯瓦蒂语", "ss"}, {"南非祖鲁语", "zu"},
        {"阿维斯扎纳语", "awa"}, {"巴斯克语", "eu"}, {"瓦鲁语", "war"}, {"依地语", "yi"}, {"哈萨克语", "kk"}
    };

    return map;

}

// https://ai.baidu.com/ai-doc/MT/mki483xpu
std::map<QString, QString> languageMapCodeBaiDu()
{
    static  std::map<QString, QString> map = { {"auto", "auto"}
        , {"English", "en"}
        , {"中文", "zh"}
        , {"日语", "jp"}
        , {"韩语", "kor"}
        , {"葡萄牙语", "pt"}
        , {"法语", "fra"}
        , {"德语", "de"}
        , {"意大利语", "it"}
        , {"西班牙", "ru"}
        , {"泰语", "spa"}
        , {"俄语", "ru"}
        , {"荷兰语", "nl"}
        , {"马来语", "may"}
        , {"丹麦语", "dan"}
        , {"瑞典语", "swe"}
        , {"印尼语", "id"}
        , {"波兰语", "pl"}
        , {"罗马尼亚语", "rom"}
        , {"土耳其语", "tr"}
        , {"希腊语", "el"}
        , {"匈牙利语", "hu"}
    };
    return map;
}

QString findKeyByValue(const std::map<QString, QString> &myMap, const QString &value)
{
    for (const auto& pair : myMap) {
        if (pair.second == value)
            return pair.first; // 返回匹配的键
    }
    return ""; // 如果没有找到匹配的值，则返回空字符串
}
