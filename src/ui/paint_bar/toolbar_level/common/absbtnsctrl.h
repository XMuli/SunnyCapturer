// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef ABSBTNSCTRL_H
#define ABSBTNSCTRL_H

#include <QBoxLayout>
#include <QWidget>
#include "xtoolbutton.h"
class AbsBtnsCtrl : public QWidget
{
    Q_OBJECT
public:
    explicit AbsBtnsCtrl(const Qt::Orientations& orien, QWidget *parent = nullptr);
    virtual ~AbsBtnsCtrl() = default;
    void addWidget(QWidget *w, const bool& bAddSpacer = true);
    void addSpacerLine(const int &length = 30);
    void addSpacerItem(const int &length = 0);
    void setFixSpacing(const int& val = 0);

protected:
    XToolButton* creatorXToolButton(const QString& dir, const QString& name, const bool& defaultChecked = false);


private:
    Qt::Orientations m_orien;
    QBoxLayout* m_layout;
};

#endif // ABSBTNSCTRL_H
