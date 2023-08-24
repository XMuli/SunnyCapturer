#include "xcolorbutton.h"

#include <QPainter>
#include <QConicalGradient>

XColorButton::XColorButton(QWidget *parent)
    : QToolButton(parent)
    , m_color("#DB000F")
    , m_bRainbow(false)
{
}

void XColorButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    // 矩形和内切圆此部分不用绘画 避免出现 拖曳时刻出现残影和交叉部分显示黑色

    if (m_bRainbow) {
        setConicalGradientColor(pa);
    } else {
        QColor colPen("#6B6B6B");
        colPen.setAlphaF(0.5);
        pa.setPen(colPen);
        pa.setBrush(m_color);
        pa.drawRect(rect());
    }

    if (isChecked()) {
        int centerX = width() / 2;
        int centerY = height() / 2;
        const int side = rect().size().width() * 2 / 3;
        const QRect rt(centerX - side / 2, centerY - side / 2, side, side);

        pa.setPen(QPen(QColor("#FFFFFF"), 2));
        pa.setBrush(Qt::NoBrush);
        pa.drawRect(rt);
    }
}

bool XColorButton::bRainbow() const
{
    return m_bRainbow;
}

void XColorButton::setBRainbow(bool newBRainbow)
{
    m_bRainbow = newBRainbow;
}

QColor XColorButton::color() const
{
    return m_color;
}

void XColorButton::setColor(const QColor &newColor)
{
    m_color = newColor;
}

void XColorButton::setConicalGradientColor(QPainter &pa)
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
