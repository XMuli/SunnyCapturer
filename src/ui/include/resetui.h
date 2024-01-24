// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef RESETUI_H
#define RESETUI_H

#include "xwidget_global.h"
#include <QWidget>

namespace Ui {
class ResetUI;
}

class ResetUI : public QWidget
{
    Q_OBJECT

public:
    explicit ResetUI(QWidget *parent = nullptr);
    ~ResetUI();

signals:
    void sigBtnHintClicked(bool checked = false);
    void sigBtnResetClicked(bool checked = false);


private:
    Ui::ResetUI *ui;
};

#endif // RESETUI_H
