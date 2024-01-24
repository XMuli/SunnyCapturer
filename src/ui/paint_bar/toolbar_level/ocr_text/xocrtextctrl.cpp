// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "xocrtextctrl.h"

XOcrTextCtrl::XOcrTextCtrl(const Qt::Orientations &orien, QWidget *parent)
    : AbsBtnsCtrl(orien, parent)
    , m_tbdit(nullptr)
    , m_tbCopy(nullptr)
    , m_tbUpdate(nullptr)
{
    initUI();
}

void XOcrTextCtrl::initUI()
{
    const QString& dir = ":/resources/icons/paint_tool_bar/paint_ctrl_btn/";
    m_tbdit = creatorXToolButton(dir, "ocr_text_edit", false);
    m_tbCopy = creatorXToolButton(dir, "ocr_text_copy", false);
    m_tbUpdate = creatorXToolButton(dir, "ocr_text_update", false);
    m_tbCopy->setCheckable(false);
    m_tbUpdate->setCheckable(false);

    addWidget(m_tbdit, false);
    addWidget(m_tbCopy, false);
    addWidget(m_tbUpdate, false);
//    m_tbUpdate->hide();
}
