// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef GENERAL_H
#define GENERAL_H

#include "xwidget_global.h"
#include <QWidget>

namespace Ui {
class General;
}

class General : public QWidget
{
    Q_OBJECT

public:
    explicit General(QWidget *parent = nullptr);
    ~General();

private slots:
    void onCbbLanguageCurrentTextChanged(const QString &arg1);
    void on_cbbThemes_currentTextChanged(const QString &arg1);
    void on_cbbLogLevel_currentTextChanged(const QString &arg1);
    void on_btnFont_released();
    void on_cbAutostart_clicked(bool checked);

public slots:
    void onBtnResetClicked(bool checked = false);

private:
    void initUI();
    void setAutoStart(const bool& enable);

private:
    Ui::General *ui;
};

#endif // GENERAL_H
