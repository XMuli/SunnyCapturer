#include "paintbar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <QPainter>
#include <QScreen>
#include <QPaintEvent>

QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

PaintBar::PaintBar(const Qt::Orientation &orie, QWidget *parent)
    : QWidget{parent}
    , m_layout(nullptr)
    , m_orie(orie)
    , m_paintToolBar(new PaintToolBar(orie, this))
    , m_paintCtrlBar(new PaintCtrlBar(ICON_SIZE, orie, this))

{
    initUI();
    initConnect();
}

PaintBar::~PaintBar()
{
    if (m_blurPixmap.isNull()) m_blurPixmap = QPixmap();
    if (m_paintToolBar) m_paintToolBar->deleteLater();
    if (m_paintCtrlBar) m_paintCtrlBar->deleteLater();
}

void PaintBar::transposePaintBar(const bool &bTranspose)
{
    if (!m_layout) return;

    m_layout->removeWidget(m_paintToolBar);
    m_layout->removeWidget(m_paintCtrlBar);

    if (bTranspose) {
        m_layout->addWidget(m_paintCtrlBar);
        m_layout->addWidget(m_paintToolBar);
    } else {
        m_layout->addWidget(m_paintToolBar);
        m_layout->addWidget(m_paintCtrlBar);
    }
}

bool PaintBar::hadDrawBtnsChecked() const
{
    return m_paintToolBar->hadDrawBtnsChecked();
}

void PaintBar::initUI()
{
    //    【Qt bug，】 水平布局下，属于 Qt 的bug，去掉 Qt::FramelessWindowHint 属性，就完美了; 且可以使用如下 来查看验证； 反之带上，则需要手动拖曳一下，才会发现界面被刷新是正常的。
    //    qDebug() << "-----@3---->rowCount:" << m_layout->rowCount()  << "columnCount:" << m_layout->columnCount() << "" << m_layout->count();
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags());  // | Qt::WindowStaysOnTopHint
    if (m_orie == Qt::Horizontal) m_layout =  new QVBoxLayout(this);
    else if (m_orie == Qt::Vertical) m_layout = new QHBoxLayout(this);

    const int& margin = 3;
    m_layout->setMargin(margin);
    m_layout->setSpacing(0);
    setLayout(m_layout);
    m_layout->addWidget(m_paintToolBar);
    m_layout->addWidget(m_paintCtrlBar);


}

void PaintBar::initConnect()
{
    connect(m_paintToolBar, &PaintToolBar::sigPaintToolBtnsRelease, this, &PaintBar::onPaintToolBtnsRelease);

    // 统一接口，由 PaintBar 转发出去
    connect(m_paintCtrlBar, &PaintCtrlBar::sigPaintCtrlIdReleased, this, &PaintBar::sigPaintCtrlIdReleased);
    connect(m_paintCtrlBar, &PaintCtrlBar::sigTextFontFamilyChanged, this, &PaintBar::sigTextFontFamilyChanged);
    connect(m_paintCtrlBar, &PaintCtrlBar::sigTextFontSizeChanged, this, &PaintBar::sigTextFontSizeChanged);
    connect(m_paintCtrlBar, &PaintCtrlBar::sigMosaicSliderValueChanged, this, &PaintBar::sigMosaicSliderValueChanged);
    connect(m_paintCtrlBar, &PaintCtrlBar::sigTextCtrlToggled, this, &PaintBar::sigTextCtrlToggled);
    connect(m_paintCtrlBar, &PaintCtrlBar::sigPointCtrlReleased, this, &PaintBar::sigPointCtrlReleased);
    connect(m_paintCtrlBar, &PaintCtrlBar::sigPickedColor, this, &PaintBar::sigPickedColor);
    connect(m_paintCtrlBar, &PaintCtrlBar::sigOCRTranslateCtrlIdReleased, this, &PaintBar::sigOCRTranslateCtrlIdReleased);
    connect(m_paintCtrlBar, &PaintCtrlBar::sigOCRTextCtrlIdReleased, this, &PaintBar::sigOCRTextCtrlIdReleased);
    connect(this, &PaintBar::sigSetTextFontSizeComboBoxValue, m_paintCtrlBar, &PaintCtrlBar::onSetTextFontSizeComboBoxValue);
    connect(this, &PaintBar::sigAutoDisableUndoAndRedo, m_paintToolBar, &PaintToolBar::onAutoDisableUndoAndRedo);
}

void PaintBar::setLowerBlurEffect(const QPixmap &pix, int radius)
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
    m_blurPixmap = std::move(origPixmap);
#endif
}

void PaintBar::disableBlurEffect()
{
    m_blurPixmap = QPixmap();
}

void PaintBar::onPaintToolBtnsRelease(const PaintType &type, const bool &isCheckable)
{
    m_paintCtrlBar->onPaintBtnRelease(type, isCheckable);

    int space;
    if (hadDrawBtnsChecked()) {
        space = 3;
        m_paintCtrlBar->show();
    } else {
        space = 0;
        m_paintCtrlBar->hide();
    }

    m_layout->setSpacing(space);
    QTimer::singleShot(10, this, [this](){ adjustSize(); }); // fix: 点击 text 时， 时间太短 pickColor 控件还没有 adjustSize() 调整好

    emit sigPaintToolBtnsRelease(type, isCheckable);
}

void PaintBar::resizeEvent(QResizeEvent *e)
{
    qDebug() << QString() << "-----------resizeEvent---parent():" << parent();
    if (!m_blurPixmap.isNull())
        emit sigUpdatePaintBarBlurPixmap();
}

void PaintBar::enterEvent(QEvent *e)
{
    setCursor(Qt::ArrowCursor);
    QWidget::enterEvent(e);
}

void PaintBar::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    int margin = 0;
    if (m_blurPixmap.isNull()) { // 关闭特效
        pa.setPen(Qt::NoPen);
        pa.setBrush(QColor(242, 242, 242));
        pa.drawRect(rect().adjusted(margin, margin, -margin, -margin));
    } else {  // 开启模糊特效
        pa.drawPixmap(rect().adjusted(margin, margin, -margin, -margin), m_blurPixmap);
    }

    margin = 1;
    pa.setPen(QPen(QColor(246, 246, 246, 1 * 255), margin));
    pa.setBrush(QColor(255, 255, 255, 0.4 * 255));
    pa.drawRect(contentsRect().adjusted(margin, margin, -margin, -margin));

    margin = 0;
    pa.setPen(QColor(70, 70, 70, 1 * 255));
    pa.setBrush(Qt::NoBrush);
    pa.drawRect(contentsRect().adjusted(margin, margin, -margin, -margin));
}
