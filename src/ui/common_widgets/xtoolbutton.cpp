#include "xtoolbutton.h"
#include <QPainter>

XToolButton::XToolButton(QWidget *parent)
    : QToolButton(parent)
{
}

void XToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    QPainter pa(this);
    pa.setRenderHint(QPainter::Antialiasing);

    if (underMouse()) {  // 鼠标悬浮时绘制红色透明效果
        pa.fillRect(rect(), QColor(0, 0, 0, 0.1 * 255));
    }
}
