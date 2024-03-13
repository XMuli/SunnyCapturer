// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "ocrtranslatectrl.h"
#include "../paintbarhelper.h"
#include "communication.h"

OcrTranslateCtrl::OcrTranslateCtrl(const Qt::Orientations &orien, QWidget *parent)
    : AbsBtnsCtrl(orien, parent)
    , m_tbTranslate(nullptr)
    , m_tbCopy(nullptr)
    , m_cbbFrom(new QComboBox(this))
    , m_lab(new QLabel("→", this))
    , m_cbbTo(new QComboBox(this))
{
    initUI();
}

void OcrTranslateCtrl::initUI()
{
    const auto& cbbFrom = youDaoLanguageMapCode();
    const auto& cbbTo = youDaoLanguageMapCode();
    const auto& currLanguageFrom = findKeyByValue(cbbFrom, CONF_GET_PROPERTY(XTokens_iamge_translate_from).toString());
    const auto& currLanguageTo = findKeyByValue(cbbTo, CONF_GET_PROPERTY(XTokens_iamge_translate_to).toString());

    // const auto& currLanguageFrom = "auto"; //GET_VALUE_PROPERTY(XTokens_iamge_translate_from);
    // const auto& currLanguageTo = "中文简体";

    for (const auto& it : cbbFrom) m_cbbFrom->addItem(it.first, it.second);
    for (const auto& it : cbbTo) m_cbbTo->addItem(it.first, it.second);

    m_cbbFrom->setCurrentText(currLanguageFrom);
    m_cbbTo->setCurrentText(currLanguageTo);

    const QString& dir = ":/resources/icons/paint_tool_bar/paint_ctrl_btn/";
    m_tbTranslate = creatorXToolButton(dir, "ocr_text_translate", true);
    m_tbCopy = creatorXToolButton(dir, "ocr_text_copy", false);
    m_tbCopy->setCheckable(false);
    m_tbCopy->hide();  // 临时屏蔽

    // 布局插入
    const int space = 10;
    addWidget(m_tbTranslate, false);
    addWidget(m_tbCopy, true);

    addSpacerItem(2 * space);
    addWidget(m_cbbFrom, false);
    addSpacerItem(space);
    addWidget(m_lab, false);
    addSpacerItem(space);
    addWidget(m_cbbTo, false);
}
