// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "xroundwidget.h"
#include "../xglobal.h"
#include <QColor>
#include <QHBoxLayout>
#include <QPainter>

XRoundWidget::XRoundWidget(QWidget *parent)
    : QWidget(parent)
{
    initUI();
}

XRoundWidget::~XRoundWidget()
{
}

void XRoundWidget::initUI()
{
    setWindowFlags(Qt::FramelessWindowHint);      // 去掉标题栏
    setAttribute(Qt::WA_TranslucentBackground);   // 设置透明，自绘画为圆角矩形
}

void XRoundWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    pa.setPen(Qt::NoPen);
    pa.setBrush(QColor(255, 255, 255, 0.8 * 255));
    pa.drawRoundedRect(contentsRect(), B_RADIRS, B_RADIRS);
}
