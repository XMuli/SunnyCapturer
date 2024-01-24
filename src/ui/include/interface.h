// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef INTERFACE_H
#define INTERFACE_H

#include "xwidget_global.h"
#include <QWidget>

namespace Ui {
class Interface;
}

enum class StyleType
{
    ST_sunny,
    ST_macos,
    ST_dde
};
Q_DECLARE_METATYPE(StyleType)                     // 可以被 QVariant 类型存储

class Interface : public QWidget
{
    Q_OBJECT

public:
    explicit Interface(QWidget *parent = nullptr);
    ~Interface();

private slots:
    void on_cbbStyle_currentTextChanged(const QString &arg1);
    void on_cbbOrientation_currentTextChanged(const QString &arg1);
    void onHighlightPickedColor(const QColor& color);
    void onCrosshairPickedColor(const QColor& color);
    void on_sbBorderWidth_valueChanged(int arg1);
    void on_sbCrosshairWidth_valueChanged(int arg1);
    void on_cbAcrylicEffect_clicked(bool checked);
    void on_cbAutoDetectWindows_clicked(bool checked);
    void on_cbAutoCopyToClipboard_clicked(bool checked);
    void on_cbCrosshairShow_clicked(bool checked);

    void on_gbCustomSizeEnable_clicked(bool checked);
    void on_cbTopLeft_clicked(bool checked);
    void on_cbSize_clicked(bool checked);
    void on_cbDelay_clicked(bool checked);
    void on_sbLeft_valueChanged(int arg1);
    void on_sbTop_valueChanged(int arg1);
    void on_sbWidth_valueChanged(int arg1);
    void on_sbHeight_valueChanged(int arg1);
    void on_dsbDelay_valueChanged(double arg1);

public slots:
    void onBtnResetClicked(bool checked = false);
    void onLanguageChange(const QString qmName);

private:
    void initUI();

private:
    Ui::Interface *ui;
};

#endif // INTERFACE_H
