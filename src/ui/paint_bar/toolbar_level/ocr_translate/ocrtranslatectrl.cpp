#include "ocrtranslatectrl.h"
#include "../paintbarhelper.h"

OcrTranslateCtrl::OcrTranslateCtrl(const Qt::Orientations &orien, QWidget *parent)
    : AbsBtnsCtrl(orien, parent)
    , m_swBtnShowModel(new SwitchButton(this))
    , m_tbModelSwitch(nullptr)
    , m_cbbFrom(new QComboBox(this))
    , m_cbbTo(new QComboBox(this))
    , m_tbdit(nullptr)
    , m_tbCopy(nullptr)
    , m_tbUpdate(nullptr)
{
    initUI();
    addWidget(m_swBtnShowModel, false);
    addWidget(m_tbModelSwitch, true);

    addWidget(m_cbbFrom, false);
    addWidget(m_cbbTo, true);

    addWidget(m_tbdit, false);
    addWidget(m_tbCopy, false);
    addWidget(m_tbUpdate, false);
}

void OcrTranslateCtrl::initUI()
{
    const double& scal = dpiScale();
    const double& fixSize = ICON_SIZE * scal * 3 / 4;
    m_swBtnShowModel->setMinimumWidth(60);
    m_swBtnShowModel->setFixedHeight(fixSize);

    const QString& dir = ":/resources/icons/paint_tool_bar/paint_ctrl_btn/";
    m_tbModelSwitch = creatorXToolButton(dir, "ocr_text_model_switch", true);
    m_tbdit = creatorXToolButton(dir, "ocr_text_edit", true);
    m_tbCopy = creatorXToolButton(dir, "ocr_text_copy", false);
    m_tbUpdate = creatorXToolButton(dir, "oct_text_update", false);

    m_tbCopy->setCheckable(false);
    m_tbUpdate->setCheckable(false);
}
