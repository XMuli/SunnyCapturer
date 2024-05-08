#include "imageview.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QStyleOption>
#include <QPainter>
#include <QFileDialog>

ImageView::ImageView(QWidget *parent)
    : QWidget(parent)
    , m_ZoomValue(1.0)
    , m_XPtInterval(0)
    , m_YPtInterval(0)
    , m_OldPos(QPoint())
    , m_Pressed(false)
    , m_labZoom(new QLabel(this))
    , m_timerLabZoom(new QTimer(this))
{
    initUI();
}

ImageView::~ImageView()
{
}

void ImageView::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pos = event->pos();
    pos = this->mapToGlobal(pos);
    QMenu *menu = new QMenu(this);

    QAction *loadImage = new QAction(tr("Load Image"));
    QObject::connect(loadImage, &QAction::triggered, this, &ImageView::onLoadImage);
    menu->addAction(loadImage);
    menu->addSeparator();

    QAction *zoomInAction = new QAction(tr("Zoom In"));
    QObject::connect(zoomInAction, &QAction::triggered, this, &ImageView::onZoomInImage);
    menu->addAction(zoomInAction);

    QAction *zoomOutAction = new QAction(tr("Zoom Out"));
    QObject::connect(zoomOutAction, &QAction::triggered, this, &ImageView::onZoomOutImage);
    menu->addAction(zoomOutAction);

    QAction *presetAction = new QAction(tr("Preset"));
    QObject::connect(presetAction, &QAction::triggered, this, &ImageView::onPresetImage);
    menu->addAction(presetAction);

    menu->exec(pos);
}

void ImageView::paintEvent(QPaintEvent *event)
{
    // 绘制样式
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    // 设置渲染质量
    painter.setRenderHint(QPainter::Antialiasing , true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    if (m_pix.isNull())
        return QWidget::paintEvent(event);

    // 根据窗口计算应该显示的图片的大小
    int width = qMin(m_pix.width(), this->width());
    int height = width * 1.0 / (m_pix.width() * 1.0 / m_pix.height());
    height = qMin(height, this->height());
    width = height * 1.0 * (m_pix.width() * 1.0 / m_pix.height());

    // 平移
    painter.translate(this->width() / 2 + m_XPtInterval, this->height() / 2 + m_YPtInterval);

    // 缩放
    painter.scale(m_ZoomValue, m_ZoomValue);

    // 绘制图像
    QRect picRect(-width / 2, -height / 2, width, height);
    painter.drawPixmap(picRect, m_pix);
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    int value = event->delta();
    if (value > 0)
        onZoomInImage();
    else
        onZoomOutImage();

    this->update();
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    m_OldPos = event->pos();
    m_Pressed = true;
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_Pressed)
        return QWidget::mouseMoveEvent(event);

    this->setCursor(Qt::SizeAllCursor);
    QPoint pos = event->pos();
    int xPtInterval = pos.x() - m_OldPos.x();
    int yPtInterval = pos.y() - m_OldPos.y();

    m_XPtInterval += xPtInterval;
    m_YPtInterval += yPtInterval;

    m_OldPos = pos;
    this->update();
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    m_Pressed = false;
    this->setCursor(Qt::ArrowCursor);
}

void ImageView::setPixmap(const QPixmap &newPix)
{
    m_pix = newPix;
}

void ImageView::initUI()
{
    m_labZoom->setParent(this);
    m_labZoom->setStyleSheet("QLabel { background-color: white; border: 1px solid black; }");
    m_labZoom->raise();
    m_labZoom->hide();
    m_labZoom->move(10, 10);

    connect(m_timerLabZoom, &QTimer::timeout, [this]() { m_labZoom->hide();});
}

void ImageView::updateZoomLabel()
{
    m_labZoom->setText(QString(tr("Zoom: %1%")).arg(m_ZoomValue * 100));
    m_labZoom->show();
    m_labZoom->adjustSize();      // 自动调整大小以适应文本
    m_timerLabZoom->stop();       // 取消之前的定时器
    m_timerLabZoom->start(2000);  // 创建一个新的定时器，并在时间到达时隐藏标签
}

void ImageView::onLoadImage(void)
{
    QString imageFile = QFileDialog::getOpenFileName(this, "Open Image", "./", tr("Images (*.png *.xpm *.jpg)"));

    QFile file(imageFile);
    if (!file.exists())
        return;

    m_pix.load(imageFile);
}

void ImageView::onZoomInImage(void)
{
    m_ZoomValue += 0.1;
    updateZoomLabel();
    this->update();
}

void ImageView::onZoomOutImage(void)
{
    m_ZoomValue -= 0.1;
    m_ZoomValue = qMax(0.1, m_ZoomValue);
    updateZoomLabel();
    this->update();
}

void ImageView::onPresetImage(void)
{
    m_ZoomValue = 1.0;
    m_XPtInterval = 0;
    m_YPtInterval = 0;
    updateZoomLabel();
    this->update();
}
