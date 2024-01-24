// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef TOKENS_H
#define TOKENS_H

#include "xwidget_global.h"
#include <QWidget>

namespace Ui {
class Tokens;
}

class Tokens : public QWidget
{
    Q_OBJECT

public:
    explicit Tokens(QWidget *parent = nullptr);
    ~Tokens();

private:
    void initUI();

public slots:
    void onBtnResetClicked(bool checked = false);

private slots:
    void on_leYDAppID_textChanged(const QString &arg1);
    void on_leYDApiSecret_textChanged(const QString &arg1);
    void on_leBdApiKey_textChanged(const QString &arg1);
    void on_leBdSecretKey_textChanged(const QString &arg1);

private:
    Ui::Tokens *ui;
};

#endif // TOKENS_H
