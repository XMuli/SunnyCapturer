// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "xcolorbutton.h"
#include <QDebug>

XColorButton::XColorButton(const QColor &color, const ColorPickerType &type, QWidget *parent)
    : QToolButton(parent)
    , m_color(color)
    , m_pickType(type)
{
}

QColor XColorButton::color() const
{
    return m_color;
}

void XColorButton::setColor(const QColor &newColor)
{
    m_color = newColor;
}

void XColorButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor wihte = QColor(255, 255, 255, 0.4 * 255);
    QColor black = QColor(0, 0, 0, 0.7 * 255);

    int margin = 2;
    if (underMouse()) {
        // 如果鼠标悬停在按钮上，绘制升起效果
//        painter.setPen(QPen(wihte, 1));
//        painter.setBrush(Qt::NoBrush);
//        margin = 0;
//        painter.drawRect(rect().adjusted(margin, margin, -margin, -margin));

        painter.setPen(QPen(black, 1));
        painter.setBrush(m_color);
        margin -= 1;
        painter.drawRect(rect().adjusted(margin, margin, -margin, -margin));
    } else {
        // 在边框上绘制1px的黑色线
        painter.setPen(QPen(black, 1));
        painter.setBrush(m_color);
        painter.drawRect(rect().adjusted(margin, margin, -margin, -margin));
    }

    if (m_pickType == ColorPickerType::CT_horizontal) {
        if (isChecked()) {
            painter.setPen(QPen(black, 1));
            painter.setBrush(Qt::white);
            const int margin = rect().width() / 3;
            const QRect& rt = rect().adjusted(margin, margin, -margin, -margin);
            painter.drawRect(rt);
        }
    }

}
