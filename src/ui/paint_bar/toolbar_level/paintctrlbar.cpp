#include "paintctrlbar.h"
#include <QDebug>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSize>
#include <QButtonGroup>
#include <QLabel>
#include "paintbarhelper.h"
#include "communication.h"
#include "horspacerline.h"
#include "verspacerline.h"

#define PIXELATED_MOSAIC_VAL  "pixelatedMosaicValue"
#define SMOOTH_MOSAIC_VAL  "smoothMosaicValue"
#define MOSAIC_DEFAUTLE_VAL 10

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
    , m_pointCtrl(nullptr)
    , m_markerPenCtrl(nullptr)
    , m_colorPicker(new ColorPicker(QSize(14, 14) * dpiScale(), orie == Qt::Horizontal ? ColorPickerType::CT_grid_horizontal : ColorPickerType::CT_grid_vertical, this))
    , m_fontFamily(new QFontComboBox(this))
    , m_fontScale(new QComboBox(this))
    , m_mosaicSliderCtrl(initSliderCtrl())
{
    initUI();

    connect(m_colorPicker, &ColorPicker::sigPickedColor, this, &PaintCtrlBar::sigPickedColor);
}

PaintCtrlBar::~PaintCtrlBar()
{
    m_rectCtrl->deleteLater();
    m_ellipseCtrl->deleteLater();
    m_arrowCtrl->deleteLater();
    m_mosaicCtrl->deleteLater();
    m_textCtrl->deleteLater();
    m_serialCtrl->deleteLater();
    m_pointCtrl->deleteLater();
    m_markerPenCtrl->deleteLater();
    m_colorPicker->deleteLater();

    m_fontFamily->deleteLater();
    m_fontScale->deleteLater();
    m_mosaicSliderCtrl->deleteLater();
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
    m_colorPicker->hide();
    m_fontFamily->hide();
    m_fontScale->hide();
    m_mosaicSliderCtrl->hide();

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
    const QString& dir(":/resources/icons/paint_tool_bar/paint_ctrl_btn/");
    connect(creatorAbsBtnsCtrl(m_orie, m_rectCtrl, dir, QStringList() << "rectangle" << "rectangle_fill"), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_ellipseCtrl, dir, QStringList() << "ellipse" << "ellipse_fill"), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_arrowCtrl, dir, QStringList() << "arrow" << "line"), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_markerPenCtrl, dir, QStringList() << "ellipse_fill" << "rectangle_fill"), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_mosaicCtrl, dir, QStringList() << "mosaic" << "blur"), &QButtonGroup::idReleased, this, &PaintCtrlBar::onMosaicCtrlIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_textCtrl, dir, QStringList() << "bold" << "italic" << "outline" << "strikeout" << "underline", false, false, -1), &QButtonGroup::idToggled, this, &PaintCtrlBar::onTextCtrlToggled);
    connect(creatorAbsBtnsCtrl(m_orie, m_serialCtrl, dir, QStringList() << "serial_number" << "serial_letter"), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_pointCtrl, dir, QStringList() << "point_small" << "point_medium" << "point_large"), &QButtonGroup::idReleased, this, &PaintCtrlBar::sigPointCtrlReleased);

//    addWidget(m_rectCtrl);
//    addWidget(m_ellipseCtrl);
//    addWidget(m_arrowCtrl);
//    addWidget(m_mosaicCtrl);
//    addWidget(m_pointCtrl);
//    addWidget(m_mosaicSliderCtrl);
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
    m_markerPenCtrl->hide();
    m_colorPicker->hide();
    m_fontFamily->hide();
    m_fontScale->hide();
    m_mosaicSliderCtrl->hide();
    m_pointCtrl->hide();

    for (int i = 0; i < m_layout->count(); ++i) {
        QLayoutItem *item = m_layout->itemAt(i);
        qDebug() << "-----@2-->i:" << i << item;
    }
}

// 通过 absBtnsCtrl 的 group 来判断是哪一个 btn 处于开启状态
int PaintCtrlBar::btnIdIschecked(const PaintType& type, const bool &isCheckable)
{
    int ret = -1;
    QPointer<AbsBtnsCtrl> ctrl = nullptr;

    if (isCheckable) {
        if (type == PaintType::PT_rectangle) {
            ctrl = m_rectCtrl;
        } else if (type == PaintType::PT_ellipse) {
            ctrl = m_ellipseCtrl;
        } else if (type == PaintType::PT_arrow) {
            ctrl = m_arrowCtrl;
        } else if (type == PaintType::PT_pencil) {
        } else if (type == PaintType::PT_marker_pen) {
            ctrl = m_markerPenCtrl;
        } else if (type == PaintType::PT_mosaic) {
            ctrl = m_mosaicCtrl;
        } else if (type == PaintType::PT_text) {
            ctrl = m_textCtrl;
        } else if (type == PaintType::PT_serial) {
            ctrl = m_serialCtrl;
        }

        const auto& btns = ctrl->findChildren<QToolButton*>();
        for (int i = 0; i < btns.count(); ++i) {
            if (btns.at(i)->isChecked()) return i;
        }
    }

    return ret;
}

AbsBtnsCtrl* PaintCtrlBar::initSliderCtrl()
{
    QSlider* slider = new QSlider(m_orie, this);

    if (m_orie == Qt::Horizontal) slider->setFixedWidth(120);
    else if (m_orie == Qt::Vertical) slider->setFixedHeight(120);

    slider->setValue(MOSAIC_DEFAUTLE_VAL);
    slider->setTickPosition(QSlider::NoTicks);
    slider->setRange(3, 30);
    slider->setPageStep(5);
    slider->setSingleStep(1);
    slider->setTracking(true);
    QLabel* labSlider = new QLabel(QString::number(slider->value()));
    AbsBtnsCtrl* absBtnsCtrl = new AbsBtnsCtrl(m_orie, this);
    absBtnsCtrl->setFixSpacing(10);
    absBtnsCtrl->addWidget(slider, false);
    absBtnsCtrl->addWidget(labSlider, false);

    absBtnsCtrl->setProperty(PIXELATED_MOSAIC_VAL, slider->value());
    absBtnsCtrl->setProperty(SMOOTH_MOSAIC_VAL, slider->value());

    connect(slider, &QSlider::valueChanged, this, [this, absBtnsCtrl, labSlider](int val){

        int id = -1;
        if (m_mosaicCtrl && m_mosaicSliderCtrl) {
            const auto btns = m_mosaicCtrl->findChildren<QToolButton*>();
            for (int i = 0; i < btns.size(); ++i) {
                if (btns.at(i)->isChecked()) id = i;
            }

            if (id == 0) {
                absBtnsCtrl->setProperty(PIXELATED_MOSAIC_VAL, val);
                qDebug() << "btns:" << btns << "id == 0  aaa";
            } else if (id == 1) {
                absBtnsCtrl->setProperty(SMOOTH_MOSAIC_VAL, val);
                qDebug() << "btns:" << btns << "id == 1  bbb";
            } else  {
                qDebug() << "btns:" << btns << "id:" << id;
            }


            auto slider = m_mosaicSliderCtrl->findChild<QSlider*>();
            slider->setValue(val);
        }

        emit sigMosaicSliderValueChanged(id, val);
        labSlider->setText(QString::number(val));
    });

    return absBtnsCtrl;
}

void PaintCtrlBar::setCurrMosaicBtnfuzzyValue()
{
    if (m_mosaicCtrl && m_mosaicSliderCtrl) {
        const auto btns = m_mosaicCtrl->findChildren<QToolButton*>();
        int id = -1;
        for (int i = 0; i < btns.size(); ++i) {
            if (btns.at(i)->isChecked()) id = i;
        }

        int val = -1;
        if (id == 0) {
            val = m_mosaicSliderCtrl ? m_mosaicSliderCtrl->property(PIXELATED_MOSAIC_VAL).toInt() : MOSAIC_DEFAUTLE_VAL;
            qDebug() << "btns:" << btns << "id == 0  aaa";
        } else if (id == 1) {
            val = m_mosaicSliderCtrl ? m_mosaicSliderCtrl->property(SMOOTH_MOSAIC_VAL).toInt() : MOSAIC_DEFAUTLE_VAL;
        } else  {
            qDebug() << "btns:" << btns << "id:" << id;
        }

        emit sigMosaicSliderValueChanged(id, val);
        auto slider = m_mosaicSliderCtrl->findChild<QSlider*>();
        slider->setValue(val);
    }
}

void PaintCtrlBar::addWidget(QWidget *w, const bool &bAddSpaceLine, int stretch, Qt::Alignment alignment)
{
    m_layout->addWidget(w, stretch, alignment);
    if (bAddSpaceLine) addSpacerLine(m_layout, m_orie);
    w->show();
}

void PaintCtrlBar::onIdReleased(int id)
{
    qDebug() << "----sender（）:" << sender() << "parent():" << sender()->parent() << "   id:" << id ;
    emit sigPaintCtrlIdReleased(id);

//    QButtonGroup *buttonGroup = qobject_cast<QButtonGroup*>(sender());
//    if (buttonGroup) {

//        for (auto& it : buttonGroup->buttons()) {
//            qDebug() << "Button" << it << "isCheckable():" << it->isCheckable() << "isChecked():" << it->isChecked();
//        }

//        QToolButton *btn = qobject_cast<QToolButton*>(buttonGroup->button(id));
//        if (btn) {
//        }
    //    }
}

void PaintCtrlBar::onTextCtrlToggled(int id, bool checked)
{

    static TextFlags flags;

    if (id == 0) {
        if (checked) flags |= TextFlag::TF_blod;
        else flags &= ~TextFlags(TextFlag::TF_blod);
    } else if (id == 1) {
        if (checked) flags |= TextFlag::TF_italic;
        else flags &= ~TextFlags(TextFlag::TF_italic);
    } else if (id == 2) {
        if (checked) flags |= TextFlag::TF_outline;
        else flags &= ~TextFlags(TextFlag::TF_outline);
    } else if (id == 3) {
        if (checked) flags |= TextFlag::TF_strikeout;
        else flags &= ~TextFlags(TextFlag::TF_strikeout);
    } else if (id == 4) {
        if (checked) flags |= TextFlag::TF_underline;
        else flags &= ~TextFlags(TextFlag::TF_underline);
    } else {
        qDebug() << "id is other!";
    }


    emit sigTextCtrlToggled(flags);
    qDebug() << "id:" << id << "checked:" << checked << "flags:" << flags;
}

void PaintCtrlBar::onMosaicCtrlIdReleased(int id)
{
    setCurrMosaicBtnfuzzyValue();
    onIdReleased(id);
}

void PaintCtrlBar::onPaintBtnRelease(const PaintType &type, const bool& isCheckable)
{
    bool bPointCtrlShow = true;
    bool bColorPickerShow = true;
    if (isCheckable) hideAllBtnsCtrl();

    if (type == PaintType::PT_rectangle) {
        addWidget(m_rectCtrl);
    } else if (type == PaintType::PT_ellipse) {
        addWidget(m_ellipseCtrl);
    } else if (type == PaintType::PT_arrow) {
        addWidget(m_arrowCtrl);
    } else if (type == PaintType::PT_pencil) {
    } else if (type == PaintType::PT_marker_pen) {
        addWidget(m_markerPenCtrl);
    } else if (type == PaintType::PT_mosaic) {
        addWidget(m_mosaicCtrl);
        addWidget(m_mosaicSliderCtrl, false);
        bPointCtrlShow = false;
        bColorPickerShow = false;
    } else if (type == PaintType::PT_text) {
        addWidget(m_textCtrl);
        addWidget(m_fontFamily, false);
        addWidget(m_fontScale, false);
        bPointCtrlShow = false;
    } else if (type == PaintType::PT_serial) {
        addWidget(m_serialCtrl);
    } else {
        return;
    }
//    if (type == PaintType::PT_undo) {
//    } else if (type == PaintType::PT_redo) {

//    } else if (type == PaintType::PT_save) {
//    } else if (type == PaintType::PT_cancel) {
//    } else if (type == PaintType::PT_finish) {
//    } else {
//        qDebug() << "type is unknow PaintType!";
//    }

    if (bPointCtrlShow) addWidget(m_pointCtrl);
    else m_pointCtrl->hide();

    if (bColorPickerShow)  {
        addWidget(m_colorPicker, false);
    } else {
        m_colorPicker->hide();
    }

    addSpacerItem(m_layout, m_orie); // 实际是有效果的，被子组合控件的弹簧所影响了
    int id = btnIdIschecked(type, isCheckable);
    emit sigPaintCtrlIdReleased(id); // fix: paintToolBar 按下时, id 默认没有上报

    for (int i = 0; i < m_layout->count(); ++i) {
        QLayoutItem *item = m_layout->itemAt(i);
        qDebug() << "-----@3-->i:" << i << item;
    }
}
