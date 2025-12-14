// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "xframewidget.h"
#include <QColor>
#include <QPainter>
#include <QGraphicsEffect>
#include <QPen>
#include "../xglobal.h"

XFrameWidget::XFrameWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);      // 去掉标题栏
    setAttribute(Qt::WA_TranslucentBackground);   // 设置透明，自绘画为圆角矩形
    //setWindowOpacity(0.7);
    //PerformanceHint = 0x00,
    //    QualityHint = 0x01,
    //    AnimationHint = 0x02

    //QGraphicsBlurEffect* blureffect = new QGraphicsBlurEffect(this);
    //blureffect->setBlurRadius(1);	//数值越大，越模糊
    //blureffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
    //this->setGraphicsEffect(blureffect);

    //setWindowFlags(Qt::FramelessWindowHint);

    //HWND hMoudle = (HWND)(winId());
    //HMODULE hDLL = LoadLibrary(L"Acrylic");

    //using fun = void (*)(HWND hWnd);

    //fun pSetBlur = (fun)GetProcAddress(hDLL, "setBlur");
    //pSetBlur((HWND)(winId()));


// 仅支持 windows 7 的毛玻璃  磨砂 透明  Aero Glass 效果
//#ifdef Q_OS_WIN
//    // 添加 winextras
//   #include <QtWin>
//    this->setAttribute(Qt::WA_TranslucentBackground);
//    QtWin::enableBlurBehindWindow(this);
//    QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
//#endif


}

void XFrameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QColor colBrush("#131313");
    colBrush.setAlphaF(0.6);
    pa.setBrush(colBrush);
    QColor colPen("#FFFFFF");
    colPen.setAlphaF(0.1);
    QPen pen(colPen, 1);
    pa.setPen(pen);

    const int margin1 = 1;
    pa.drawRoundedRect(contentsRect().adjusted(margin1, margin1, -margin1, -margin1), B_RADIRS, B_RADIRS);

    colPen.setNamedColor("#000000");
    colPen.setAlphaF(0.1);
    pa.setPen(colPen);
    pa.setBrush(Qt::NoBrush);
    const int margin2 = 0;
    pa.drawRoundedRect(contentsRect().adjusted(margin2, margin2, -margin2, -margin2), B_RADIRS, B_RADIRS);
}
