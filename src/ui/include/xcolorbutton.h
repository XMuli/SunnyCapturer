// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XCOLORBUTTON_H
#define XCOLORBUTTON_H

#include <QToolButton>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QMouseEvent>

enum class ColorPickerType
{
    CT_horizontal,                           // GridLayout 仅一行 的水平布局，为活动色选择
    CT_grid_horizontal,                      // GridLayout 为两行显示
    CT_grid_vertical
};

class XColorButton : public QToolButton
{
    Q_OBJECT
public:
    XColorButton(QWidget *parent = nullptr);
    XColorButton(const QColor& color, const ColorPickerType& type, QWidget *parent = nullptr);
    virtual ~XColorButton() = default;

    QColor color() const;
    void setColor(const QColor &newColor);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QColor m_color;
    ColorPickerType m_pickType;
};

#endif // XCOLORBUTTON_H
