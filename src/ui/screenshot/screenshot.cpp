#include "screenshot.h"
#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QDebug>

ScreenShot::ScreenShot(QWidget *parent)
    : QWidget(parent)
    , m_primaryScreen(qGuiApp->primaryScreen())
    , m_screens(qGuiApp->screens())
    , m_origPix(nullptr)
    , m_vdRect()
{
    initUI();
}

ScreenShot::~ScreenShot()
{
    preDestruction();
}

void ScreenShot::capture()
{
    originalPixmap();
}

void ScreenShot::initUI()
{
    m_vdRect = m_primaryScreen->virtualGeometry();
}

void ScreenShot::drawShadowOverlay(const QRect &fullRect, const QRect &pickedRect, QPainter& pa) const
{
    QPainterPath path;
    path.addRect(fullRect);
    path.addRect(pickedRect);
    path.setFillRule(Qt::OddEvenFill);
    pa.save();
    pa.setPen(Qt::NoPen);
    pa.setBrush(QColor(0, 0, 0, 0.5 * 255));
    pa.drawPath(path);
    pa.restore();
}

void ScreenShot::originalPixmap()
{
    if (!m_origPix)  // 调用一次 copy constructor (底部的 QPixmap::copy 实际是深拷贝)，还有优化空间
        m_origPix = new QPixmap(m_primaryScreen->grabWindow(qApp->desktop()->winId(), m_vdRect.x(), m_vdRect.y(), m_vdRect.width(), m_vdRect.height()));

    m_origPix->save("123456.png");
}

void ScreenShot::preDestruction()
{
    if (m_origPix) {
        delete m_origPix;
        m_origPix = nullptr;
    }
}

void ScreenShot::mousePressEvent(QMouseEvent *e)
{

}

void ScreenShot::mouseReleaseEvent(QMouseEvent *e)
{

}

void ScreenShot::mouseMoveEvent(QMouseEvent *e)
{

}

void ScreenShot::wheelEvent(QWheelEvent *e)
{

}

void ScreenShot::keyReleaseEvent(QKeyEvent *e)
{

}

void ScreenShot::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter pa(this);     // 始终从相对本窗口的坐标 (0, 0) 开始绘画
    pa.setBrush(Qt::NoBrush);
    pa.setPen(Qt::NoPen);

    qDebug() << "m_origPix:" << m_origPix;
    if (m_origPix) {
        pa.drawPixmap(QPoint(0, 0), *m_origPix);
    }

    drawShadowOverlay(m_vdRect, QRect(0, 0, 0, 0), pa);
}
