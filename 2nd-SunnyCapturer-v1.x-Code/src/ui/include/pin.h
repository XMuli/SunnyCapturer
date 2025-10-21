// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef PASTE_H
#define PASTE_H

#include <QWidget>

namespace Ui {
class Pin;
}

class Pin : public QWidget
{
    Q_OBJECT

public:
    explicit Pin(QWidget *parent = nullptr);
    ~Pin();

private slots:
    void on_sbOpacity_valueChanged(int arg1);
    void on_sbMaxSize_valueChanged(int arg1);

public slots:
    void onBtnResetClicked(bool checked = false);

private:
    void initUI();

private:
    Ui::Pin *ui;
};

#endif // PASTE_H
