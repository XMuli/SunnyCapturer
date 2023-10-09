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
#include "../common/absbtnsctrl.h"

#define PROPERTY_PAINT_TYPR "PaintType"
#define ICON_SIZE 32 // 32 48   在 100% 缩放上 32px 比较合适，然后实际大小乘以主屏幕的屏幕缩放比

enum class PaintType
{
    PT_rectangle,
    PT_ellipse,
    PT_arrow,
    PT_pencil,
    PT_mosaic,
    PT_marker_pen,
    PT_text,
    PT_serial,
    PT_pin,
    PT_point,

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

QButtonGroup *creatorAbsBtnsCtrl(const Qt::Orientation& orie, QPointer<AbsBtnsCtrl>& absBtnsCtrl, const QString& dir, const QStringList& items
                                 , const bool& bLastAddSpacer = false, const bool exclusive = true, const int defaultChecked = 0);

struct PaintBtn
{
    PaintBtn() {}
    PaintBtn(QToolButton* tBtn, const PaintType& tType, const QString& tName, const QString& tTooltip, const bool& tBCheckable  = true, const bool& tBAddSpacer = false)
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
void addSpacerLine(QGridLayout *layout, const Qt::Orientation &orie, const int &length = 30, const int &row = 0, const int &col = 0);
void addSpacerLine(QBoxLayout *layout, const Qt::Orientation &orie, const int &length = 30);
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
