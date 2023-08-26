#ifndef COLORPICKER_H
#define COLORPICKER_H

#include "xwidget_global.h"
#include <QColor>
#include <QWidget>
#include <QPointer>
#include <QToolButton>
#include <QGridLayout>
#include <QButtonGroup>
#include "xcolorbutton.h"

enum class ColorPickerType
{
    Horizontal,                           // GridLayout 仅一行 的水平布局，为活动色选择
    Grid                                  // GridLayout 为两行显示
};

class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPicker(QWidget *parent = nullptr);

public:
    const QColor pickedColor();
    void setColorPickerType(const ColorPickerType type = ColorPickerType::Horizontal);

private:
    void initUI();

public slots:
    void onPickedColor(QAbstractButton *btn);

private:
    bool                     m_bRainbow;
    QGridLayout*             m_gridLayout;
    QToolButton*             m_pickedBtn;
    ColorPickerType          m_colorPickerType;
    QPointer<QButtonGroup>   m_colorsGroup;
};

#endif // COLORPICKER_H
