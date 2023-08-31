#ifndef PAINTBARHELPER_H
#define PAINTBARHELPER_H

#include <QObject>
#include <QDomElement>
#include <QString>
#include <QToolButton>
#include <QGridLayout>
#include <QPointer>
#include "common/absbtnsctrl.h"
#include "horspacerline.h"
#include "verspacerline.h"

#define PROPERTY_PAINT_TYPR "PaintType"
#define ICON_SIZE 48 // 32

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

    PT_undo,
    PT_redo,

    PT_save,
    PT_cancel,
    PT_finish
};
Q_DECLARE_METATYPE(PaintType)                     // 可以被 QVariant 类型存储
Q_DECLARE_FLAGS(PaintTypeFlass, PaintType)        // 枚举 PaintType 生成宏 PaintTypeFlass
//Q_DECLARE_OPERATORS_FOR_FLAGS(PaintTypeFlass)   // 重载宏 PaintTypeFlass 的 |() 函数

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


#endif // PAINTBARHELPER_H
