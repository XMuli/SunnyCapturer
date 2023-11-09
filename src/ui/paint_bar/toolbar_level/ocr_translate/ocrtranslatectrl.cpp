#include "ocrtranslatectrl.h"
#include "../paintbarhelper.h"
#include "communication.h"

OcrTranslateCtrl::OcrTranslateCtrl(const Qt::Orientations &orien, QWidget *parent)
    : AbsBtnsCtrl(orien, parent)
    , m_swBtnShowModel(new SwitchButton(this))
    , m_tbCopy(nullptr)
    , m_cbbFrom(new QComboBox(this))
    , m_lab(new QLabel("→", this))
    , m_cbbTo(new QComboBox(this))
{
    initUI();
}

void OcrTranslateCtrl::initUI()
{
    const double& scal = dpiScale();
    const double& fixSize = ICON_SIZE * scal * 3 / 4;
//    m_swBtnShowModel->setChecked(true);
    m_swBtnShowModel->setMinimumWidth(80);
    m_swBtnShowModel->setFixedHeight(fixSize);

    const auto& currLanguageFrom = "auto"; //CONF_MANAGE.property("XGeneral_language").toString();
    const auto& currLanguageTo = "中文简体";

    const auto& cbbFrom = youDaoLanguageMapCode();
    const auto& cbbTo = youDaoLanguageMapCode();
    for (const auto& it : cbbFrom) m_cbbFrom->addItem(it.first, it.second);
    for (const auto& it : cbbTo) m_cbbTo->addItem(it.first, it.second);

    m_cbbFrom->setCurrentText(currLanguageFrom);
    m_cbbTo->setCurrentText(currLanguageTo);

    const QString& dir = ":/resources/icons/paint_tool_bar/paint_ctrl_btn/";
    m_tbCopy = creatorXToolButton(dir, "ocr_text_copy", false);
    m_tbCopy->setCheckable(false);
    m_tbCopy->hide();

    // 布局插入
    const int space = 10;
    addWidget(m_swBtnShowModel, false);
    addSpacerItem(space);
    addWidget(m_swBtnShowModel, false);
//    addWidget(m_tbCopy, true);

    addSpacerItem(2 * space);
    addWidget(m_cbbFrom, false);
    addSpacerItem(space);
    addWidget(m_lab, false);
    addSpacerItem(space);
    addWidget(m_cbbTo, false);
}
