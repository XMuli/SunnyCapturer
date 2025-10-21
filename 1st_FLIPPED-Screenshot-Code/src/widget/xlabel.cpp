// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>
#include "xlabel.h"
#include "../xglobal.h"
#include <QPainter>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QtGlobal>
#include <QMouseEvent>
#include <QPainterPath>

XLabel::XLabel(QWidget *parent, Qt::WindowFlags f)
    : QToolButton(parent)
{
    initUI();
}

void XLabel::setChecked(const bool &checked)
{
    m_bChecked = checked;
}

void XLabel::setColor(QColor c, double alpha)
{
    m_color = c;
    m_alpha = alpha;
}

// 构造一个锥向渐变(center, angle) https://blog.csdn.net/weixin_37818081/article/details/118879134
void XLabel::setConicalGradientColor(QPainter& pa)
{
    QConicalGradient conicalGradient(QPointF(contentsRect().center()), 300);

    conicalGradient.setColorAt(0, QColor("#EB004A"));
    conicalGradient.setColorAt(1 / 7.0, QColor("#FF0000"));
    conicalGradient.setColorAt(2 / 7.0, QColor("#F800FC"));
    conicalGradient.setColorAt(3 / 7.0, QColor("#4D00F3"));
    conicalGradient.setColorAt(4 / 7.0, QColor("#1BB8F2"));
    conicalGradient.setColorAt(5 / 7.0, QColor("#46EA48"));
    conicalGradient.setColorAt(6 / 7.0, QColor("#FFDB34"));
    conicalGradient.setColorAt(1, QColor("#EB004A"));

    pa.save();
    QColor colPen("#6B6B6B");
    colPen.setAlphaF(0.5);
    pa.setPen(colPen);
    pa.setBrush(conicalGradient);
    pa.drawRect(rect());
    pa.restore();
}

void XLabel::setRainbow(bool bRainbow)
{
    m_bRainbow = bRainbow;
}

void XLabel::initUI()
{
    m_color = "#DB000F";
    m_alpha = 1;
    m_bRainbow = false;
    m_bChecked = false;

    // 此两行也可以注释掉，亲测不影响，可能是由于后面有一个 ColorParaBar 作为父类
    //setWindowFlags(Qt::FramelessWindowHint);      // 去掉标题栏
    setAttribute(Qt::WA_TranslucentBackground);   // 设置透明，自绘画为圆角矩形
//    setAutoFillBackground(false);

    QColor col = this->palette().window().color();
}


void XLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    // 矩形和内切圆此部分不用绘画 避免出现 拖曳时刻出现残影和交叉部分显示黑色

    if (m_bRainbow) {
        setConicalGradientColor(pa);
    } else {
        QColor colBrush(m_color);
        colBrush.setAlphaF(m_alpha);
        QColor colPen("#6B6B6B");
        colPen.setAlphaF(0.5);
        pa.setPen(colPen);
        pa.setBrush(colBrush);
        pa.drawRect(rect());
    }

    if (m_bChecked) {
        int centerX = width() / 2;
        int centerY = height() / 2;
        const int side = rect().size().width() * 2 / 3;
        const QRect rt(centerX - side / 2, centerY - side / 2, side, side);

        pa.setPen(QPen(QColor("#FFFFFF"), 2));
        pa.setBrush(Qt::NoBrush);
        pa.drawRect(rt);
    }
}
