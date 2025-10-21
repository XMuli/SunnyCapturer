// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QPointer>
#include <QWidget>
#include "qhotkey.h"
#include "../../data/configmanager.h"
#include "../../data/configjson.h"
#include "../../paint_bar/toolbar_level/paintbarhelper.h"
#include "json.hpp"
using json = nlohmann::json;

#define COMM Communication::instance()

enum class HotKeyType{
    HKT_capture,                    // 正常的立刻截图，不会有任何预设
    HKT_delay_capture,              // 延迟    截图: 计时器 + HKT_capture 截图效果   （会受 custom_size_enable & custom_dealy 影响， [会无视] topleft_enable size_enable 的状态）
    HKT_custom_capture,             // 自定义  截图: 计时器 + 预设的窗口等大小 截图  （会受 custom_size_enable & custom_dealy & topleft_enable & size_enable 的影响）
    HKT_ocr_capture,                // OCR    截图:
    HKT_image_transltae_capture     // 图片翻译截图:
};


QString hotKeyTypeToString(const HotKeyType& hotKeyType);

class Communication : public QObject
{
    Q_OBJECT

public:
    static Communication& instance(); // signle
    void init();
    bool resetShortcut(const QKeySequence &keySequence, const HotKeyType& type);
    bool shortcutStatus(const HotKeyType& type) const ;
    void setAppFont(const QString& font);
    void loadTranslation(const QString& language);
    void loadCustomQss(const QString &path);
    void showBuildInfoWidget();            // 显示提供信息窗口
    QString toLocaleName(const QString& language);

signals:
    void sigLanguageChange(const QString qmFile);
    void sigShowSystemMessagebox(const QString &title, const QString &msg, const int& msecs); // 系统消息通知
    void sigOCRImageGenerateFinsh(const QSize& size, const QString& path);                    // OCR 图片保存成功
    void sigOCRTextGenerateFinsh(const QByteArray& obj, const OcrData& ocrTextData);                                                     // OCR Text 保存成功


    void sigImgTranslateCtrlHide();                                                     // OCR 翻译 隐藏

private:

    explicit Communication(QObject *parent = nullptr);
    virtual ~Communication() = default;
    Communication(const Communication&) = delete;
    Communication& operator=(const Communication&) = delete;
    Communication(Communication&&) = delete;
    Communication& operator=(Communication&&) = delete;

private:
    QHotkey* m_hkCapture;
    QHotkey* m_hkDelayCapture;
    QHotkey* m_hkCustiomCapture;
    QHotkey* m_hkOcrCapture;
    QHotkey* m_hkImgTransCapture;
};

std::map<QString, QString> languageMap();
std::map<QString, QString> languageMapCodeYouDao();
std::map<QString, QString> languageMapCodeBaiDu();
std::map<const QString, const bool> themesMap();
QString findKeyByValue(const std::map<QString, QString>& myMap, const QString& value);
QStringList getQSSFileNames(const QString& path = qApp->applicationDirPath() + "/resources/qss");


const QScreen* cursorScrn(const QPoint &pos);      // 当前光标所在的屏幕，不存在则返回默主屏幕
double cursorScrnScale(const QScreen *screen);     // 当前光标所在的屏幕的缩放比
double cursorScrnScale(const bool& onlyPrimary);   //
double cursorScrnDpr(const bool& onlyPrimary);
double appGuiDpr();


#endif // COMMUNICATION_H
