#include "xblureffect.h"
#include <QApplication>
#include <QPainter>
#include <QScreen>

QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

XBlurEffect::XBlurEffect(QWidget *parent)
    : QWidget(parent)
{
}

void XBlurEffect::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (!m_pixmap.isNull()) pa.drawPixmap(rect(), m_pixmap);

    const int& margin = 1;
    pa.setPen(QPen(QColor(255, 255, 255, 0.4 * 255), margin));
    pa.setBrush(QColor(255, 255, 255, 0.4 * 255));
    pa.drawRect(contentsRect().adjusted(margin, margin, -margin, -margin));

    pa.setPen(QColor(255, 255, 255, 0.8 * 255));
    pa.setBrush(Qt::NoBrush);
    pa.drawRect(contentsRect());
}

// 不事先调用这个函数，则是直接的透明效果，若是加上这个，则是自定义皮肤的效果
void XBlurEffect::setPixmap(const QPixmap &pix, int radius)
{
    if (pix.isNull()) return;

#if HIDE_TOOL_BAR_BLUR_EFFECT
    m_pixmap = pix;
#else
    QImage img = pix.toImage();
    QPixmap origPixmap(pix.size());
    origPixmap.setDevicePixelRatio(qApp->primaryScreen()->devicePixelRatio());
    QPainter painter(&origPixmap);
    qt_blurImage(&painter, img, radius, true, false);
    m_pixmap = std::move(origPixmap);
#endif
}
