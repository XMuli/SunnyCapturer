#include "paintbar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

PaintBar::PaintBar(const Qt::Orientation &orie, QWidget *parent)
    : QWidget{parent}
    , m_layout(nullptr)
    , m_orie(orie)
    , m_blurEffect(new XBlurEffect(this))
    , m_paintToolBar(new PaintToolBar(orie, this))
    , m_paintCtrlBar(new PaintCtrlBar(orie, this))

{
    initUI();
    initConnect();
}

PaintBar::~PaintBar()
{
    if (m_blurEffect) m_blurEffect->deleteLater();
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

void PaintBar::setLowerBlurEffect(const QPixmap &pix, int radius)
{
    if (m_blurEffect) {
        m_blurEffect->setPixmap(pix, radius);
        m_blurEffect->lower();
    }
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
    connect(m_paintCtrlBar, &PaintCtrlBar::sigMosaicSliderValueChanged, this, &PaintBar::sigMosaicSliderValueChanged);
    connect(m_paintCtrlBar, &PaintCtrlBar::sigTextCtrlToggled, this, &PaintBar::sigTextCtrlToggled);
    connect(m_paintCtrlBar, &PaintCtrlBar::sigPointCtrlReleased, this, &PaintBar::sigPointCtrlReleased);
    connect(m_paintCtrlBar, &PaintCtrlBar::sigPickedColor, this, &PaintBar::sigPickedColor);
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
    adjustSize();
    emit sigPaintToolBtnsRelease(type, isCheckable);
}

void PaintBar::resizeEvent(QResizeEvent *e)
{
    qDebug() << QString() << "-----------resizeEvent---parent():" << parent();
    if (m_blurEffect) {
        emit sigUpdatePaintBarBlurPixmap();
        m_blurEffect->setGeometry(0, 0, width(), height());
    }
}

void PaintBar::enterEvent(QEvent *e)
{
    setCursor(Qt::ArrowCursor);
    QWidget::enterEvent(e);
}

