// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "paintbarhelper.h"
#include <QFile>
#include <QIODevice>
#include <QByteArray>
#include <QSvgRenderer>
#include <QDomDocument>
#include <QPixmap>
#include <QPainter>
#include <QIcon>
#include <QDebug>
#include <QButtonGroup>
#include <QString>
#include <QSpacerItem>
#include <QDateTime>
#include "horspacerline.h"
#include "verspacerline.h"
#include "xtoolbutton.h"
#include "../../../data/configjson.h"

void setAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval)
{
    // if it has the tagname then overwritte desired attribute
    if (elem.tagName().compare(strtagname) == 0)
        elem.setAttribute(strattr, strattrval);
    // loop all children
    for (int i = 0; i < elem.childNodes().count(); i++) {
        if (!elem.childNodes().at(i).isElement()) continue;
        QDomElement t = elem.childNodes().at(i).toElement();
        setAttrRecur(t, strtagname, strattr, strattrval);
    }
}

QIcon changedSVGColor(QString path, QColor color, QSize size)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();

    QDomDocument doc;
    doc.setContent(baData);
    QDomElement elem = doc.documentElement(); // const 和 值传递
    setAttrRecur(elem, "path", "fill", color.name());  // rect

    QSvgRenderer svgRenderer(doc.toByteArray());
    // create pixmap target (could be a QImage)
    // QPixmap pix(svgRenderer.defaultSize());
    QPixmap pix(svgRenderer.defaultSize().scaled(size.width(), size.height(), Qt::KeepAspectRatio));  // fix: 修改 svg 颜色后略有模糊
    pix.fill(Qt::transparent);
    // create painter to act over pixmap
    QPainter pixPainter(&pix);
    // use renderer to render over painter which paints on pixmap
    svgRenderer.render(&pixPainter);
    QIcon myicon(pix);
    return myicon;
}

void addSpacerLine(QGridLayout *layout, const Qt::Orientation &orie, const int &length, const int &row, const int &col)
{
    if (!layout) return;
    const int count = countItemsformLayout(layout, orie);
    if (orie == Qt::Horizontal) layout->addWidget(new VerSpacerLine(length), row, count, Qt::AlignVCenter);
    else if (orie == Qt::Vertical) layout->addWidget(new HorSpacerLine(length + 5), count, col, Qt::AlignHCenter);
}

void addSpacerLine(QBoxLayout *layout, const Qt::Orientation &orie, const int &length)
{
    if (!layout) return;
    const int& count = layout->count();
    if (orie == Qt::Horizontal) layout->addWidget(new VerSpacerLine(length), 0, Qt::AlignCenter);
    else if (orie == Qt::Vertical) layout->addWidget(new HorSpacerLine(length + 5), 0, Qt::AlignCenter);
}

int countItemsformLayout(const QGridLayout *layout, const Qt::Orientation &orie, const int &idx)
{
    int count = 0;
    if (orie == Qt::Horizontal) {  // Count the widgets in the idx's row
        int columns = layout->columnCount();
        for (int i = 0; i < columns; ++i) {
            if (layout->itemAtPosition(idx, i) != nullptr) count++;
        }
    } else if (orie == Qt::Vertical) {
        int rows = layout->rowCount();
        for (int i = 0; i < rows; ++i) {
            if (layout->itemAtPosition(i, idx) != nullptr) count++;
        }
    } else {
        qDebug() << "aligment is empty!";
    }

    qDebug() << "---->count:" <<  count;
    return count;
}



QButtonGroup* creatorAbsBtnsCtrl(const Qt::Orientation& orie, QPointer<AbsBtnsCtrl>& absBtnsCtrl, const QString& dir, const QStringList& items
                                 , const QStringList& defaultChecked, const bool& bLastAddSpacer, const bool exclusive)
{
    if (!absBtnsCtrl) absBtnsCtrl = new AbsBtnsCtrl(orie);

    QButtonGroup* group = new QButtonGroup(absBtnsCtrl);
    group->setExclusive(exclusive);
    for (int i = 0; i < items.size(); ++i) {
        XToolButton* btn = new XToolButton();
        btn->setStyleSheet(szIconBtnCSS);
        const double& scal = dpiScale();
        const QSize size(ICON_SIZE * scal, ICON_SIZE * scal);
        btn->setIconSize(size);
        btn->setFixedSize(size);
        btn->setObjectName(items.at(i));
        const QString& path = dir + items.at(i) + ".svg";
        btn->setIcon(QIcon(path));
        btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        btn->setAutoRaise(true);
        btn->setCheckable(true);
        btn->setChecked(false);

        for (const auto& it : defaultChecked) {
            if (i == it.toInt()) {
                btn->setChecked(true);
                const QIcon icon(changedSVGColor(path, highlightColor(), btn->iconSize()));
                btn->setIcon(icon);
            }
        }

        group->addButton(btn, i);
        if (i == items.size() - 1) {
            absBtnsCtrl->addWidget(btn, bLastAddSpacer);
        } else {
            absBtnsCtrl->addWidget(btn, false);
        }

        QObject::connect(btn, &XToolButton::toggled, [btn]() {

            const QString path = ":/resources/icons/paint_tool_bar/paint_ctrl_btn/" + btn->objectName() + ".svg";
            const QIcon origIcon(path);
            const QIcon newIcon(changedSVGColor(path, highlightColor(), btn->iconSize()));
            btn->setIcon(btn->isChecked() ? newIcon : origIcon);
        });

    }

    return group;
}

void addSpacerItem(QBoxLayout *layout, const Qt::Orientation &orie, const int &length)
{
    QSpacerItem *spacer = nullptr;
    if (orie == Qt::Horizontal)  {
        spacer = new QSpacerItem(length, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    } else if (orie == Qt::Vertical) {
        spacer = new QSpacerItem(1, length, QSizePolicy::Minimum, QSizePolicy::Expanding);
    }

    layout->addSpacerItem(spacer);
}

// 默认返回主屏幕的缩放比
double dpiScale(const QScreen *scrn)
{
    double scal = scrn ? scrn->logicalDotsPerInch() / double(96) : 1;
    qDebug() << "scrn:" << scrn << "DPIScale:" << scal;
    return scal;
}

QColor highlightColor(const double& alphaf)
{
    QColor color = QColor(CJ_GET_QSTR("interface.highlight")); //Qt::green;
    color.setAlphaF(alphaf);
    return color;
}

QString crosshairColor(const bool enable)
{
    const QColor& color = enable ? QColor(CJ_GET_QSTR("interface.crosshair")) : Qt::green;
    return color.name();
}

int borderWidth()
{
    return CJ_GET("interface.border_width").get<int>();
}

int crosshairWidth()
{
    return CJ_GET("interface.crosshair_width").get<int>();
}

bool acrylicEffectEnable()
{
    return CJ_GET("interface.acrylic_effect").get<bool>();
}

QString formatToFileName(const QString &name)
{
    auto fileName = name.trimmed();
    auto first = fileName.indexOf("$");
    auto second = fileName.lastIndexOf("$");
    auto nameTime = fileName.mid(first + 1, second - first - 1);
    auto finalyName = fileName.replace(first, second - first + 1, QDateTime::currentDateTime().toString(nameTime));

    return finalyName;
}
