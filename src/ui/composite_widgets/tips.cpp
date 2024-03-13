// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "tips.h"
#include <QDebug>
#include <QRect>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QString>
#include <QtGlobal>

Tips::Tips(const QString& text, const TipsType& type, QWidget *parent)
    : QWidget(parent)
    , m_text(text)
    , m_type(type)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    QFont font = this->font();
    if (m_type == TipsType::TT_picked_rect_tips) {
        font.setPointSize(12);
    } else if (m_type == TipsType::TT_point_changed_tips) {
        font.setPointSize(50);
    } else if (m_type == TipsType::TT_countdown_tips) {
        font.setPointSize(40);
    } else if (m_type == TipsType::TT_keyboard_operation_tips) {
        font.setPointSize(12);
    }
    setFont(font);
}

void Tips::paintEvent(QPaintEvent *event)
{
    QPainter pa(this);
    pa.setRenderHint(QPainter::Antialiasing);

    pa.save();
    const int penWidth = 1;
    pa.setPen(QPen(QColor(255, 255, 255, 0.4 * 255), penWidth));
    pa.setBrush(QColor(0, 0, 0, 0.4 * 255));
    pa.drawRect(rect().adjusted(1, 1, -1, -1));

    pa.setPen(QPen(QColor(255, 255, 255, 0.8 * 255), penWidth));
    if (m_type == TipsType::TT_picked_rect_tips) {

        QRect rt1 = textRect("px");
        QRect rt2 = textRect(m_text);
        resize(rt1.size().width() + rt2.size().width(), qMax(rt1.size().height(), rt2.size().height()));
        const QPoint& p1 = rect().topRight() - QPoint(rt1.width(), 0);
        const QPoint& p2 = rect().bottomRight() - QPoint(rt1.width(), 0);

        pa.setPen(QPen(QColor(255, 255, 255, 0.4 * 255), penWidth));
        pa.setBrush(Qt::NoBrush);
        pa.drawLine(p1, p2);
        pa.setPen(QPen(QColor(255, 255, 255, 0.8 * 255), penWidth));
        pa.drawText(QRect(p1, rect().bottomRight()), Qt::AlignCenter, "px");
        pa.drawText(QRect(rect().topLeft(), p2), Qt::AlignCenter, m_text);
    } else if (m_type == TipsType::TT_point_changed_tips) {

        const QRect& rt = textRect(m_text);
        const int& maxWidth = qMax(rt.width(), rt.height());
        resize(maxWidth, maxWidth);
        pa.setBrush(Qt::NoBrush);
        pa.drawText(rect(), Qt::AlignCenter, m_text);
    } else if (m_type == TipsType::TT_countdown_tips) {

        const QRect& rt = textRect(m_text);
        const int& maxWidth = qMax(rt.width(), rt.height());
        resize(maxWidth * 4 / 3, maxWidth);
        pa.setBrush(Qt::NoBrush);
        pa.drawText(rect(), Qt::AlignCenter, m_text);
    } else if (m_type == TipsType::TT_keyboard_operation_tips) {
        pa.drawText(rect().center(), m_text);
    }

    pa.restore();
}

void Tips::setType(TipsType newType)
{
    m_type = newType;
}

QRect Tips::textRect(const QString &text)
{
    QFontMetrics fontMetrics(font());
    QRect textRect = fontMetrics.boundingRect(text);
    textRect.moveCenter(rect().center());

    int margin = 40;
    if (m_type == TipsType::TT_picked_rect_tips) {
        margin = 6;
    } else if (m_type == TipsType::TT_point_changed_tips) {
        margin = 40;
    } else if (m_type == TipsType::TT_keyboard_operation_tips) {
        margin = 10;
    }

    return textRect.adjusted(-margin, -margin, margin, margin);
}

void Tips::setText(const QString &newText)
{
    m_text = newText;

    if (m_type == TipsType::TT_countdown_tips) m_text += "s";  // 加上单位秒
}

void Tips::setColor(const QColor &newColor)
{
    m_color = newColor;
}
