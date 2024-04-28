// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XIMAGETRANSLATEWIDGET_H
#define XIMAGETRANSLATEWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include <QLabel>
#include "xtoolbutton.h"
#include "../common/absbtnsctrl.h"

class XImageTranslateWidget : public AbsBtnsCtrl
{
    Q_OBJECT
public:
    explicit XImageTranslateWidget(const Qt::Orientations& orien, QWidget *parent = nullptr);
    virtual ~XImageTranslateWidget() = default;

private:
    void initUI();

public:
    XToolButton*  m_tbTranslate;
    XToolButton*  m_tbCopy;

    QComboBox*    m_cbbFrom;
    QComboBox*    m_cbbTo;
    QLabel*       m_lab;
};

#endif // XIMAGETRANSLATEWIDGET_H
