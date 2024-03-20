// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef PAINTBARHELPER_H
#define PAINTBARHELPER_H

#include <QObject>
#include <QDomElement>
#include <QString>
#include <QToolButton>
#include <QGridLayout>
#include <QPointer>
#include <QScreen>
#include <QGuiApplication>
#include "common/absbtnsctrl.h"

#define PROPERTY_PAINT_TYPR "PaintType"
#define ICON_SIZE 28     // 32 48   在 100% 缩放上 32px 比较合适，然后实际大小乘以主屏幕的屏幕缩放比
#define ICON_PADDING 4          //
const QString szIconBtnCSS = QString("border-style:none; padding: %1px").arg(ICON_PADDING);
//#define ICON_PADDING_TOP 0      //
//#define ICON_PADDING_BOTTOM 0   //

//const QString PaintToolBarIconBtnCSS = QString("border-style:none; padding-left: %1px; padding-top: %1px; padding-right: %1px; padding-bottom: %2px;")
//                                           .arg(ICON_PADDING).arg(ICON_PADDING_BOTTOM);
//const QString PaintCtrlBarIconBtnCSS = QString("border-style:none; padding-left: %1px; padding-top: %2px; padding-right: %1px; padding-bottom: %1px;")
//                                           .arg(ICON_PADDING).arg(ICON_PADDING_TOP);


enum class PaintType
{
    PT_rectangle,
    PT_ellipse,
    PT_arrow,
    PT_pencil,
    PT_marker_pen,
    PT_mosaic,
    PT_text,
    PT_serial,
    PT_pin,
    PT_point,

    PT_ocr_translate,   // OCR 翻译
    PT_ocr_text,        // OCR 生成文本

    PT_undo,
    PT_redo,

    PT_save,
    PT_cancel,
    PT_finish
};
Q_DECLARE_METATYPE(PaintType)                     // 可以被 QVariant 类型存储
Q_DECLARE_FLAGS(PaintTypeFlass, PaintType)        // 枚举 PaintType 生成宏 PaintTypeFlass
//Q_DECLARE_OPERATORS_FOR_FLAGS(PaintTypeFlass)   // 重载宏 PaintTypeFlass 的 |() 函数


enum class TextFlag
{
    TF_blod = 1 << 0,        // 1     // 粗体
    TF_italic = 1 << 1,      // 2     // 倾斜
    TF_outline = 1 << 2,     // 4     // 描边
    TF_strikeout = 1 << 3,   // 8     // 删除线
    TF_underline = 1 << 4    // 16     // 下划线
};
Q_DECLARE_METATYPE(TextFlag)                     // 可以被 QVariant 类型存储
Q_DECLARE_FLAGS(TextFlags, TextFlag)            // 枚举 TextFlag 生成宏 TextFlags
Q_DECLARE_OPERATORS_FOR_FLAGS(TextFlags)

// ImgTranslateData 都是调用 YouDao API
struct ImgTranslateData
{
    ImgTranslateData() {}

    // 自定义
    bool bTranslate = true;
    // YouDao API Tranlstates 有用接口
    QString from = "auto";
    QString to = "zh-CHS";
    QString render = "1";   // 是否需要服务端返回渲染的图片，0-否； 1-是
};
Q_DECLARE_METATYPE(ImgTranslateData)            // 可以被 QVariant 类型存储


// OcrChannel + OcrData 都是调用 BaiDu API
enum class OcrChannel
{
    OCR_high_precision,                // 通用文字识别（高精度版）         1000 次/month
    OCR_high_precision_location,       // 通用文字识别（高精度含位置版）     500 次/month
    OCR_standard,                      // 通用文字识别（标准版）           1000 次/month
    OCR_standard_location,             // 通用文字识别（标准含位置版）      1000 次/month
};
Q_DECLARE_METATYPE(OcrChannel)                     // 可以被 QVariant 类型存储


enum class OcrTextOperate
{
    OTO_empty,
    OTO_is_allow_edit,
    OTO_text_copy,
    OTO_update          // 尝试换一个接口进行更新
};

struct OcrData
{
    // 自定义
    OcrTextOperate operate = OcrTextOperate::OTO_empty;                  // 是哪一个按钮被按下
    OcrChannel pipeline =  OcrChannel::OCR_high_precision;
    bool allowWrite = false;
    bool bTranslate = false;

    // BaiDu API Image Tranlstates 有用接口
    QString fromLang = "zh";
    QString toLang = "en";

};
Q_DECLARE_METATYPE(OcrData)                     // 可以被 QVariant 类型存储


QButtonGroup *creatorAbsBtnsCtrl(const Qt::Orientation& orie, QPointer<AbsBtnsCtrl>& absBtnsCtrl, const QString& dir, const QStringList& items
                                 , const QStringList& defaultChecked, const bool& bLastAddSpacer = false, const bool exclusive = true);

struct PaintBtn
{
    PaintBtn() {}
    PaintBtn(QToolButton* tBtn, const PaintType& tType, const QString& tName, const QString& tTooltip
             , const bool& tBCheckable  = true, const bool& tBAddSpacer = false)
        : btn(tBtn)
        , type(tType)
        , name(tName)
        , tooltip(tTooltip)
        , bCheckable(tBCheckable)
        , bAddSpacer(tBAddSpacer)
    {}

    QToolButton* btn;
    PaintType    type;         // 对应枚举
    QString      name;         // objectName
    QString      tooltip;
    bool         bCheckable;
    bool         bAddSpacer;
};

void setAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval);
QIcon changedSVGColor(QString path, QString color, QSize size);
void addSpacerLine(QGridLayout *layout, const Qt::Orientation &orie, const int &length = ICON_SIZE - 2 * ICON_PADDING, const int &row = 0, const int &col = 0);
void addSpacerLine(QBoxLayout *layout, const Qt::Orientation &orie, const int &length = ICON_SIZE - 2 * ICON_PADDING);
void addSpacerItem(QBoxLayout *layout, const Qt::Orientation &orie, const int &length = 0);    // 添加弹簧
int countItemsformLayout(const QGridLayout* layout, const Qt::Orientation& orie, const int& idx = 0);
double dpiScale(const QScreen* scrn = qGuiApp->primaryScreen());

QString highlightColor(const bool enable = false); // 高亮色，活动色，强调色
QString crosshairColor(const bool enable = true);
int borderWidth();
int crosshairWidth();
bool acrylicEffectEnable();

QString formatToFileName(const QString& name);



#endif // PAINTBARHELPER_H
