#include "paintctrlbar.h"
#include <QDebug>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSize>
#include "paintbarhelper.h"

PaintCtrlBar::PaintCtrlBar(const Qt::Orientation &orie, QWidget *parent)
    : QWidget(parent)
    , m_layout(nullptr)
    , m_orie(orie)
    , m_rectCtrl(nullptr)
    , m_ellipseCtrl(nullptr)
    , m_arrowCtrl(nullptr)
    , m_mosaicCtrl(nullptr)
    , m_textCtrl(nullptr)
    , m_serialCtrl(nullptr)
    , m_lienWidthBar(nullptr)
    , m_colorPicker(new ColorPicker(QSize(16, 16), orie == Qt::Horizontal ? ColorPickerType::CT_grid_horizontal : ColorPickerType::CT_grid_vertical, this))
    , m_fontFamily(new QFontComboBox(this))
    , m_fontScale(new QComboBox(this))
    , m_scrollBar(new QSlider(m_orie, this))
{
    initUI();
}

PaintCtrlBar::~PaintCtrlBar()
{
    m_rectCtrl->deleteLater();
    m_ellipseCtrl->deleteLater();
    m_arrowCtrl->deleteLater();
    m_mosaicCtrl->deleteLater();
    m_textCtrl->deleteLater();
    m_serialCtrl->deleteLater();
    m_lienWidthBar->deleteLater();
    m_colorPicker->deleteLater();

    m_fontFamily->deleteLater();
    m_fontScale->deleteLater();
    m_scrollBar->deleteLater();
}

void PaintCtrlBar::initUI()
{
    if (m_orie == Qt::Horizontal) {
        m_layout = new QHBoxLayout(this);
    } else if (m_orie == Qt::Vertical) {
        m_layout = new QVBoxLayout(this);
    }

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_fontFamily->setMaximumWidth(170);
    m_scrollBar->setTickPosition(QSlider::NoTicks);
    m_colorPicker->hide();
    m_fontFamily->hide();
    m_fontScale->hide();
    m_scrollBar->hide();

    setContentsMargins(0, 0, 0, 0);
    m_layout->setContentsMargins(0, 0, 0 ,0);
    setLayout(m_layout);
    initBtns();
}


/************************************************控件布局************************************************

********************************************************************************************************
*                                                                                                      *
*  ************************         ************************        ************************           *
*  * btn  btn  弹簧/分割线 *          * btn  btn  弹簧/分割线  *       * btn  btn  弹簧/分割线  *   弹簧     *
*  ************************         ************************        ************************           *
*                                                                                                      *
********************************************************************************************************

*******************************************************************************************************/
void PaintCtrlBar::initBtns()
{
    const QString& dir(":/resources/screenshot_ui/paint_tool_bar/paint_ctrl_btn/");
    connect(creatorAbsBtnsCtrl(m_orie, m_rectCtrl, dir, QStringList() << "rectangle" << "rectangle_fill"), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_ellipseCtrl, dir, QStringList() << "ellipse" << "ellipse_fill"), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_arrowCtrl, dir, QStringList() << "arrow" << "line" << "arrow_bloken"), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_mosaicCtrl, dir, QStringList() << "mosaic" << "blur"), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_textCtrl, dir, QStringList() << "bold" << "italic" << "outline" << "strikeout_line" << "underline", false, false), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_serialCtrl, dir, QStringList() << "serial_rectangle" << "serial_ellipse"), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
//    connect(creatorAbsBtnsCtrl(m_orie, m_lienWidthBar, dir, QStringList() << "lienWidth1" << "lienWidth2" << "lienWidth3"), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);

//    addWidget(m_rectCtrl);
//    addWidget(m_ellipseCtrl);
//    addWidget(m_arrowCtrl);
//    addWidget(m_mosaicCtrl);
//    addWidget(m_scrollBar);

//    addWidget(m_textCtrl);
//    addWidget(m_fontFamily);
//    addWidget(m_fontScale);
//    addWidget(m_colorPicker);
//    addSpacerItem(m_layout, m_orie);
}

void PaintCtrlBar::hideAllBtnsCtrl()
{
    // 遍历 m_layout 中的所有子对象
    for (int i = 0; i < m_layout->count(); ++i) {
        QLayoutItem *item = m_layout->itemAt(i);
        qDebug() << "-----@1-->i:" << i << item;
    }

    // 遍历 m_layout 中的所有子对象
    QLayoutItem *item;
    while ((item = m_layout->takeAt(0)) != nullptr) {
        QWidget *widget = item->widget();

        qDebug() << "widget:" << widget << "item:" << item;
        if (widget) {
            m_layout->removeWidget(widget);
            widget->setParent(nullptr);
            widget->hide();

            if (qobject_cast<VerSpacerLine*>(widget) || qobject_cast<HorSpacerLine*>(widget)) {
                widget->deleteLater(); // 释放内存
            }
        } else {
            QSpacerItem *spacer = item->spacerItem();
            if (spacer) {
                delete spacer; // 释放内存
                continue;
            }
        }

    }

    m_rectCtrl->hide();
    m_ellipseCtrl->hide();
    m_arrowCtrl->hide();
    m_mosaicCtrl->hide();
    m_textCtrl->hide();
    m_serialCtrl->hide();
//    m_lienWidthBar->hide();
    m_colorPicker->hide();
    m_fontFamily->hide();
    m_fontScale->hide();
    m_scrollBar->hide();

    for (int i = 0; i < m_layout->count(); ++i) {
        QLayoutItem *item = m_layout->itemAt(i);
        qDebug() << "-----@2-->i:" << i << item;
    }
}

void PaintCtrlBar::addWidget(QWidget *w, const bool &bAddSpaceLine, int stretch, Qt::Alignment alignment)
{
    m_layout->addWidget(w, stretch, alignment);
    if (bAddSpaceLine) addSpacerLine(m_layout, m_orie);
    w->show();
}

void PaintCtrlBar::onIdReleased(int idx)
{
    qDebug() << "----sender（）:" << sender() << "   idx:" << idx ;
}

void PaintCtrlBar::onPaintBtnRelease(const PaintType &type)
{    
    hideAllBtnsCtrl();
    bool bColorPickerShow = true;
    if (type == PaintType::PT_rectangle) {
        addWidget(m_rectCtrl);
    } else if (type == PaintType::PT_ellipse) {
        addWidget(m_ellipseCtrl);
    } else if (type == PaintType::PT_arrow) {
        addWidget(m_arrowCtrl);
    } else if (type == PaintType::PT_pencil) {
    } else if (type == PaintType::PT_mosaic) {
        addWidget(m_mosaicCtrl);
        addWidget(m_scrollBar);
        bColorPickerShow = false;
    } else if (type == PaintType::PT_marker_pen) {
    } else if (type == PaintType::PT_text) {
        addWidget(m_textCtrl);
        addWidget(m_fontFamily, false);
        addWidget(m_fontScale, false);
    } else if (type == PaintType::PT_serial) {
        addWidget(m_serialCtrl);
    } else if (type == PaintType::PT_pin) {

    } else if (type == PaintType::PT_undo) {
    } else if (type == PaintType::PT_redo) {

    } else if (type == PaintType::PT_save) {
    } else if (type == PaintType::PT_cancel) {
    } else if (type == PaintType::PT_finish) {
    } else {
        qDebug() << "type is unknow PaintType!";
    }

    if (bColorPickerShow)  {
        addWidget(m_colorPicker, false);
    } else {
        m_colorPicker->hide();
    }

    addSpacerItem(m_layout, m_orie); // 实际是有效果的，被子组合控件的弹簧所影响了

    for (int i = 0; i < m_layout->count(); ++i) {
        QLayoutItem *item = m_layout->itemAt(i);
        qDebug() << "-----@3-->i:" << i << item;
    }
}
