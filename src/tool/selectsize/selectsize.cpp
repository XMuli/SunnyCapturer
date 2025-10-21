// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "selectsize.h"
#include <QPainter>
#include "../../xglobal.h"
#include "../../screen/datamaid.h"

SelectSize::SelectSize(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    initUI();
}

SelectSize::SelectSize(const QString& text, QWidget* parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{
    initUI();
}

void SelectSize::onTextChanged(QString text)
{
    text = this->text();
    QFontMetrics fm(font());
    QRect bound = fm.boundingRect(QRect(), Qt::AlignCenter, text);
    resize(bound.size() + QSize(2, 2)); // fix: 1080 和 2K 屏幕下，字体被遮挡
}

void SelectSize::initUI()
{
    setContentsMargins(SS_MARGIN_LEFT, SS_MARGIN_TOP, SS_MARGIN_RIGHT, SS_MARGIN_BOTTOM);
    connect(this, &SelectSize::sigTextChanged, this, &SelectSize::onTextChanged);
}

void SelectSize::paintEvent(QPaintEvent* e)
{
    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QColor col(DATAMAID->paraValue("borderColor").toString());
    pa.setPen(Qt::NoPen);
    pa.setBrush(col);
    pa.drawRoundedRect(rect(), SS_RADIRS, SS_RADIRS);

    pa.setPen("#FFFFFF");
    pa.setBrush(Qt::NoBrush);
    pa.drawText(contentsRect(), Qt::AlignCenter, text());
}

