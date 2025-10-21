// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "blurwidget.h"
#include "../../xglobal.h"
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPixmap>
#include <QApplication>
#include <QScreen>
#include <QDebug>

QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

BlurWidget::BlurWidget(QWidget *parent)
    : QWidget(parent)
    , m_pix()
//    , m_pBlurEffect(std::make_unique<QGraphicsBlurEffect>())
{
//    m_pBlurEffect->setBlurRadius(5);
//    m_pBlurEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
//    setGraphicsEffect(m_pBlurEffect.get());
//    setWindowFlags(Qt::FramelessWindowHint);
}

void BlurWidget::setPixmap(const QPixmap &pix, int blurRadius)
{
    if (!pix.isNull()) {
//        m_pix = std::move(pix);
        QImage img = pix.toImage();
        QPixmap tPix(pix.size());
        tPix.setDevicePixelRatio(qApp->primaryScreen()->devicePixelRatio());  // fix: macos dpi is 2
        QPainter painter(&tPix);
        qt_blurImage(&painter, img, blurRadius, true, false);
        m_pix = std::move(tPix);
    }


//    if (m_pBlurEffect)
//        m_pBlurEffect->setBlurRadius(blurRadius);
}

void BlurWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    const int margin = 1;
    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (!m_pix.isNull()) {
        QPainterPath clipPath;
        clipPath.addRoundedRect(contentsRect(), B_RADIRS, B_RADIRS);
        pa.setClipPath(clipPath);
        pa.drawPixmap(rect(), m_pix);
    }

    pa.setRenderHints(QPainter::Antialiasing);
    QColor colBrush(19, 19, 19, 0.6 * 255);
    QColor colPen(255, 255, 255, 0.1 * 255);
    pa.setBrush(colBrush);
    QPen pen(colPen, 1);
    pa.setPen(pen);
    pa.drawRoundedRect(contentsRect().adjusted(margin, margin, -margin, -margin), B_RADIRS, B_RADIRS);

    colPen = QColor(0, 0, 0, 0.1 * 255);
    pa.setPen(colPen);
    pa.setBrush(Qt::NoBrush);
    pa.drawRoundedRect(contentsRect(), B_RADIRS, B_RADIRS);

//    QPainterPath path;
//    path.addRect(contentsRect());
//    path.addRoundedRect(contentsRect().adjusted(margin, margin, -margin, -margin), B_RADIRS, B_RADIRS);
//    path.setFillRule(Qt::OddEvenFill);
//    pa.setPen(colPen);
//    pa.setBrush(colPen);
//    pa.drawPath(path);
}
