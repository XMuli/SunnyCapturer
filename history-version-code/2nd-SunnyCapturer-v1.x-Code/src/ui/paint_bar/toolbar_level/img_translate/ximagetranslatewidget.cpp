// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "ximagetranslatewidget.h"
#include "communication.h"

XImageTranslateWidget::XImageTranslateWidget(const Qt::Orientations &orien, QWidget *parent)
    : AbsBtnsCtrl(orien, parent)
    , m_tbTrans(nullptr)
    , m_cbbFrom(new QComboBox(this))
    , m_cbbTo(new QComboBox(this))
    , m_lab(new QLabel("→", this))
{
    initUI();
}

void XImageTranslateWidget::initUI()
{
    QString comboBoxStyle = "QComboBox { border: none; background-color: transparent; min-width: 120px;} "
                            "QComboBox::drop-down { width: 0px; } "
                            "QComboBox::down-arrow { width: 0px; } "
                            "QComboBox QAbstractItemView { min-width: 180px; }";

    m_cbbFrom->setStyleSheet(comboBoxStyle);
    m_cbbTo->setStyleSheet(comboBoxStyle);

    const int& iamge_translate_channel = CJ_GET("tokens.iamge_translate.channel");
    QString currLanguageFrom;
    QString currLanguageTo;
    if (iamge_translate_channel == 0) {  // TBD
        const auto& cbbFrom = languageMapCodeBaiDu();
        const auto& cbbTo = languageMapCodeBaiDu();
        currLanguageFrom = findKeyByValue(cbbFrom, CJ_GET_QSTR("tokens.iamge_translate.baidu.from"));
        currLanguageTo = findKeyByValue(cbbTo, CJ_GET_QSTR("tokens.iamge_translate.baidu.to"));

        for (const auto& it : cbbFrom) m_cbbFrom->addItem(it.first, it.second);
        for (const auto& it : cbbTo) {
            if (it.second != "auto")
                m_cbbTo->addItem(it.first, it.second);
        }

    } else if (iamge_translate_channel == 1) {
        const auto& cbbFrom = languageMapCodeYouDao();
        const auto& cbbTo = languageMapCodeYouDao();
        currLanguageFrom = findKeyByValue(cbbFrom, CJ_GET_QSTR("tokens.iamge_translate.youdao.from"));
        currLanguageTo = findKeyByValue(cbbTo, CJ_GET_QSTR("tokens.iamge_translate.youdao.to"));

        for (const auto& it : cbbFrom) m_cbbFrom->addItem(it.first, it.second);
        for (const auto& it : cbbTo) {
            if (it.second != "auto")
                m_cbbTo->addItem(it.first, it.second);
        }

    }

    m_cbbFrom->setCurrentText(currLanguageFrom);
    m_cbbTo->setCurrentText(currLanguageTo);

    const QString& dir = ":/resources/icons/paint_tool_bar/paint_ctrl_btn/";
    m_tbTrans = creatorXToolButton(dir, "ocr_text_translate", true);
    m_cbbFrom->hide();
    m_lab->hide();

    // 布局插入
    const int space = 10;
    addWidget(m_tbTrans, true);

    // addSpacerItem(2 * space);
    // addWidget(m_cbbFrom, false);
    // addSpacerItem(space);
    // addWidget(m_lab, false);
    addSpacerItem(space);
    addWidget(m_cbbTo, false);
}
