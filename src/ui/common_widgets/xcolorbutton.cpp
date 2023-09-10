#include "xcolorbutton.h"
#include <QDebug>

XColorButton::XColorButton(const QColor &color, QWidget *parent)
    : QToolButton(parent)
    , m_color(color)
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

    if (underMouse()) {
        // 如果鼠标悬停在按钮上，绘制升起效果
        painter.setPen(QPen(wihte, 1));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(rect().adjusted(1, 1, -1, -1));

        painter.setPen(QPen(black, 1));
        painter.setBrush(m_color);
        painter.drawRect(rect().adjusted(2, 2, -2, -2));
    } else {
        // 在边框上绘制1px的黑色线
        painter.setPen(QPen(black, 1));
        painter.setBrush(m_color);
        painter.drawRect(rect().adjusted(1, 1, -1, -1));
    }

}
