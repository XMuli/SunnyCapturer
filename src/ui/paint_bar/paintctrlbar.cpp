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
    , m_colorPicker(new ColorPicker(QSize(12, 12), orie == Qt::Horizontal ? ColorPickerType::CT_grid_horizontal : ColorPickerType::CT_grid_vertical, this))
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
}

void PaintCtrlBar::initUI()
{
    if (m_orie == Qt::Horizontal) {
        m_layout = new QHBoxLayout(this);
//        m_colorPicker->setColorPickerType(ColorPickerType::CT_grid_horizontal);
    } else if (m_orie == Qt::Vertical) {
        m_layout = new QVBoxLayout(this);
//        m_colorPicker->setColorPickerType(ColorPickerType::CT_grid_horizontal);
    }

//    m_colorPicker->setSize(QSize(100, 100));
//    m_colorPicker->adjustSize();
    setContentsMargins(0, 0, 0, 0);
    m_layout->setContentsMargins(0, 0, 0 ,0);
    setLayout(m_layout);

    initBtns();
}

/************************************************控件布局************************************************

********************************************************************************************************
*                                                                                                      *
*  ************************         ************************        ************************           *
*  * btn  btn  弹簧/分割线 *   弹簧   * btn  btn  弹簧/分割线  *   弹簧  * btn  btn  弹簧/分割线 *   弹簧     *
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

    m_layout->addWidget(m_rectCtrl, 0, Qt::AlignCenter);
    addSpacerLine(m_layout, m_orie);
    m_layout->addWidget(m_ellipseCtrl, 0, Qt::AlignCenter);
    addSpacerLine(m_layout, m_orie);
    m_layout->addWidget(m_arrowCtrl, 0, Qt::AlignCenter);
    addSpacerLine(m_layout, m_orie);
    m_layout->addWidget(m_mosaicCtrl, 0, Qt::AlignCenter);
    addSpacerLine(m_layout, m_orie);
    m_layout->addWidget(m_textCtrl, 0, Qt::AlignCenter);
    addSpacerLine(m_layout, m_orie);
    m_layout->addWidget(m_serialCtrl, 0, Qt::AlignCenter);
    addSpacerLine(m_layout, m_orie);
    m_layout->addWidget(m_colorPicker, 0, Qt::AlignCenter);

    addSpacerItem(m_layout, m_orie); // 实际是有效果的，被子组合控件的弹簧所影响了
}

void PaintCtrlBar::onIdReleased(int idx)
{
    qDebug() << "----sender（）:" << sender() << "   idx:" << idx ;
}
