// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

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
