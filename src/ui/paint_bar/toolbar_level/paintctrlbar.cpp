// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "paintctrlbar.h"
#include <QDebug>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSize>
#include <QButtonGroup>
#include <QLabel>
#include <QFont>
#include "paintbarhelper.h"
#include "communication.h"
#include "horspacerline.h"
#include "verspacerline.h"
#include "xtoolbutton.h"
#include "../../data/configmanager.h"

#define PIXELATED_MOSAIC_VAL  "pixelatedMosaicValue"
#define SMOOTH_MOSAIC_VAL  "smoothMosaicValue"
#define MOSAIC_DEFAUTLE_VAL 10

PaintCtrlBar::PaintCtrlBar(const int &colorPickerIconSize, const Qt::Orientation &orie, QWidget *parent)
    : QWidget(parent)
    , m_layout(nullptr)
    , m_orie(orie)
    , m_rectCtrl(nullptr)
    , m_ellipseCtrl(nullptr)
    , m_arrowCtrl(nullptr)
    , m_markerPenCtrl(nullptr)
    , m_mosaicCtrl(nullptr)
    , m_textCtrl(nullptr)
    , m_serialCtrl(nullptr)
    , m_pointCtrl(nullptr)
    , m_colorPicker(new ColorPicker(QSize(colorPickerIconSize, colorPickerIconSize) * dpiScale(qGuiApp->screenAt(QCursor::pos())) / 2, orie == Qt::Horizontal ? ColorPickerType::CT_grid_horizontal : ColorPickerType::CT_grid_vertical, this))
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
    // m_ocrTranslateCtrl->deleteLater();

    m_fontFamily->deleteLater();
    m_fontScale->deleteLater();
    m_mosaicSliderCtrl->deleteLater();
}

void PaintCtrlBar::initUI()
{
    if (m_orie == Qt::Horizontal) {
        m_layout = new QHBoxLayout(this);
        m_fontFamily->setMaximumWidth(120);
    } else if (m_orie == Qt::Vertical) {
        m_layout = new QVBoxLayout(this);
        m_fontFamily->setMaximumWidth(100);
    }

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_colorPicker->setCurrPickedColor(CONF_PBS_DATA.paPen.color().name());
    m_colorPicker->hide();
    m_fontFamily->hide();

    m_fontFamily->setCurrentFont(QFont(CONF_PBS_DATA.fontFamily, CONF_PBS_DATA.fontSize));
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
    QStringList textLists;
    if (CONF_PBS_DATA.textBold) textLists << "0";
    if (CONF_PBS_DATA.textItalic) textLists << "1";
    if (CONF_PBS_DATA.textOutline) textLists << "2";
    if (CONF_PBS_DATA.textStrikeout) textLists << "3";
    if (CONF_PBS_DATA.textUnderline) textLists << "4";
    const QString& dir(":/resources/icons/paint_tool_bar/paint_ctrl_btn/");



    connect(creatorAbsBtnsCtrl(m_orie, m_rectCtrl, dir, QStringList() << "rectangle" << "rectangle_fill", QStringList() << QString::number(CONF_PBS_DATA.rectintType)), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_ellipseCtrl, dir, QStringList() << "ellipse" << "ellipse_fill", QStringList() << QString::number(CONF_PBS_DATA.ellipseType)), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_arrowCtrl, dir, QStringList() << "arrow" << "line", QStringList() << QString::number(CONF_PBS_DATA.arrowType)), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_markerPenCtrl, dir, QStringList() << "ellipse_fill" << "rectangle_fill", QStringList() << QString::number(CONF_PBS_DATA.marker_penType)), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_mosaicCtrl, dir, QStringList() << "mosaic" << "blur", QStringList() << QString::number(CONF_PBS_DATA.mosaicType)), &QButtonGroup::idReleased, this, &PaintCtrlBar::onMosaicCtrlIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_textCtrl, dir, QStringList() << "bold" << "italic" << "outline" << "strikeout" << "underline", textLists, false, false), &QButtonGroup::idToggled, this, &PaintCtrlBar::onTextCtrlToggled);
    connect(creatorAbsBtnsCtrl(m_orie, m_serialCtrl, dir, QStringList() << "serial_number" << "serial_letter", QStringList() << QString::number(CONF_PBS_DATA.serialType)), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    connect(creatorAbsBtnsCtrl(m_orie, m_pointCtrl, dir, QStringList() << "point_small" << "point_medium" << "point_large", QStringList() << QString::number(CONF_PBS_DATA.pointType)), &QButtonGroup::idReleased, this, &PaintCtrlBar::onIdReleased);
    // OcrTranslateCtrl 的对象
    // m_ocrTranslateCtrl = new OcrTranslateCtrl(m_orie, this);
    // connect(m_ocrTranslateCtrl->m_tbTranslate, &XToolButton::toggled, this, &PaintCtrlBar::onOcrTranslateStatusChanged);
    // connect(m_ocrTranslateCtrl->m_tbCopy, &XToolButton::toggled, this, &PaintCtrlBar::onOcrTranslateCopy);
    // connect(m_ocrTranslateCtrl->m_cbbFrom, &QComboBox::currentTextChanged, this, &PaintCtrlBar::onCbbFromCurrentTextChanged);
    // connect(m_ocrTranslateCtrl->m_cbbTo, &QComboBox::currentTextChanged, this, &PaintCtrlBar::onCbbToCurrentTextChanged);

    connect(m_fontFamily, &QFontComboBox::currentFontChanged, this, &PaintCtrlBar::sigTextFontFamilyChanged);
    connect(m_fontScale, &QComboBox::currentTextChanged, this, &PaintCtrlBar::sigTextFontSizeChanged);

    // m_ocrTranslateCtrl->hide();
    m_fontFamily->setEditable(false);
    m_fontScale->setEditable(true);
    const QStringList& fontSize = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "14", "16", "18", "20", "22", "24", "26", "28", "36", "48", "72", "96", "124"};
    m_fontScale->addItems(fontSize);
    m_fontScale->setCurrentText(QString::number(CONF_PBS_DATA.fontSize));


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
int PaintCtrlBar::btnIdIschecked(const PaintType& type, const bool &isCheckable, const bool& isChecked)
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
        } else if (type == PaintType::PT_img_translate) {  //不需要向外传递，模拟切换过来，就直接相应【初次】翻译

            // 线路发生改变，需要重新赋值
            const auto& channel = CONF_GET_PROPERTY(XTokens_iamge_translate_channel).toString();
            m_imgTransDate.channel = channel;
            if (channel == "baidu") {
                m_imgTransDate.from_baidu = CONF_GET_PROPERTY(XTokens_iamge_translate_from_baidu).toString();
                m_imgTransDate.to_baidu = CONF_GET_PROPERTY(XTokens_iamge_translate_to_baidu).toString();

            } else if (channel == "youdao") {
                m_imgTransDate.from = CONF_GET_PROPERTY(XTokens_iamge_translate_from_youdao).toString();
                m_imgTransDate.to = CONF_GET_PROPERTY(XTokens_iamge_translate_to_youdao).toString();
            }

            m_imgTransDate.bTranslate = true;
            if (isChecked)
                emit sigImgTranslate(m_imgTransDate);
        } else if (type == PaintType::PT_ocr) {       //不需要向外传递，模拟切换过来，就直接相应【初次】 OCR 提取文字
            m_ocrTextDate.bTranslate = true;

            // OCR 线路发生改变，需要重新赋值
            const auto& text = CONF_GET_PROPERTY(XTokens_ocr_channel).toString();
            OcrChannel ocr;
            if (text == "high") ocr = OcrChannel::OCR_high_precision;
            else if (text == "high_location") ocr = OcrChannel::OCR_high_precision_location;
            else if (text == "standard") ocr = OcrChannel::OCR_standard;
            else if (text == "standard_location") ocr = OcrChannel::OCR_standard_location;
            m_ocrTextDate.pipeline = ocr;

            if (isChecked) emit sigOcr(m_ocrTextDate);
        }

        const auto& btns = ctrl->findChildren<XToolButton*>();
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
            const auto btns = m_mosaicCtrl->findChildren<XToolButton*>();
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

AbsBtnsCtrl *PaintCtrlBar::initOcrTranslateCtrl()
{

    return nullptr;
}

void PaintCtrlBar::setCurrMosaicBtnfuzzyValue()
{
    if (m_mosaicCtrl && m_mosaicSliderCtrl) {
        const auto btns = m_mosaicCtrl->findChildren<XToolButton*>();
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
    qDebug() << "----sender（）:" << sender() << "parent():" << "   id:" << id ;
    emit sigPaintCtrlIdReleased(id);

    const auto& paint = sender()->parent();
    if (paint == m_rectCtrl) {
        CONF_PBS_DATA.rectintType = id;
    } else if (paint == m_ellipseCtrl) {
        CONF_PBS_DATA.ellipseType = id;
    } else if (paint == m_arrowCtrl) {
        CONF_PBS_DATA.arrowType = id;
    } else if (paint == m_markerPenCtrl) {
        CONF_PBS_DATA.marker_penType = id;
    } else if (paint == m_mosaicCtrl) {
        CONF_PBS_DATA.mosaicType = id;
    } else if (paint == m_textCtrl) {
        // TBD:
        const bool& checked = qobject_cast<QButtonGroup *>(sender())->button(id)->isChecked();
        if (id == 0) CONF_PBS_DATA.textBold = checked;
        else if (id == 1) CONF_PBS_DATA.textItalic = checked;
        else if (id == 2) CONF_PBS_DATA.textOutline = checked;
        else if (id == 3) CONF_PBS_DATA.textStrikeout = checked;
        else if (id == 4) CONF_PBS_DATA.textUnderline = checked;
        else qDebug() << "PaintCtrlBar::onIdReleased is m_textCtrl, and id is empty!";
    } else if (paint == m_serialCtrl) {
        CONF_PBS_DATA.serialType = id;
    } else if (paint == m_pointCtrl) {
        emit sigPointCtrlReleased(id);
        CONF_PBS_DATA.pointType = id;
    } else {
        qDebug() << "sender()->parent(): %1 is empty!";
    }

//    QButtonGroup *buttonGroup = qobject_cast<QButtonGroup*>(sender());
//    if (buttonGroup) {

//        for (auto& it : buttonGroup->buttons()) {
//            qDebug() << "Button" << it << "isCheckable():" << it->isCheckable() << "isChecked():" << it->isChecked();
//        }

//        XToolButton *btn = qobject_cast<XToolButton*>(buttonGroup->button(id));
//        if (btn) {
//        }
    //    }
}

void PaintCtrlBar::onTextCtrlToggled(int id, bool checked)
{
    static TextFlags flags;
    if (id == 0) {
        checked ? flags |= TextFlag::TF_blod : flags &= ~TextFlags(TextFlag::TF_blod);
        CONF_PBS_DATA.textBold = checked;
    } else if (id == 1) {
        checked ? flags |= TextFlag::TF_italic : flags &= ~TextFlags(TextFlag::TF_italic);
        CONF_PBS_DATA.textItalic = checked;
    } else if (id == 2) {
        checked ? flags |= TextFlag::TF_outline : flags &= ~TextFlags(TextFlag::TF_outline);
        CONF_PBS_DATA.textOutline = checked;
    } else if (id == 3) {
        checked ? flags |= TextFlag::TF_strikeout : flags &= ~TextFlags(TextFlag::TF_strikeout);
        CONF_PBS_DATA.textStrikeout = checked;
    } else if (id == 4) {
        checked ? flags |= TextFlag::TF_underline : flags &= ~TextFlags(TextFlag::TF_underline);
        CONF_PBS_DATA.textUnderline = checked;
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

void PaintCtrlBar::onPaintBtnRelease(const PaintType &type, const bool& isCheckable, const bool& isChecked)
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
    } else if (type == PaintType::PT_img_translate) {
        bPointCtrlShow = false;
        bColorPickerShow = false;
        if (!isChecked) emit COMM.sigOcrTranslateCtrlHide();
    } else if (type == PaintType::PT_ocr) {
        bPointCtrlShow = false;
        bColorPickerShow = false;
        if (!isChecked) emit COMM.sigOcrTextCtrlHide();
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
    int id = btnIdIschecked(type, isCheckable, isChecked);
    emit sigPaintCtrlIdReleased(id); // fix: paintToolBar 按下时, id 默认没有上报

    for (int i = 0; i < m_layout->count(); ++i) {
        QLayoutItem *item = m_layout->itemAt(i);
        qDebug() << "-----@3-->i:" << i << item;
    }
}

void PaintCtrlBar::onSetTextFontSizeComboBoxValue(const QString &fontSize)
{
    m_fontScale->setCurrentText(fontSize);
}
