#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QSize>
#include <QColor>
#include <QWidget>
#include <QPointer>
#include <QToolButton>
#include <QGridLayout>
#include <QButtonGroup>
#include "xwidget_global.h"
#include "xcolorbutton.h"

enum class ColorPickerType
{
    CT_horizontal,                           // GridLayout 仅一行 的水平布局，为活动色选择
    CT_grid_horizontal,                      // GridLayout 为两行显示
    CT_grid_vertical
};

class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPicker(QWidget *parent = nullptr);
    explicit ColorPicker(QSize size = QSize(16, 16), const ColorPickerType type = ColorPickerType::CT_horizontal, QWidget *parent = nullptr);

public:
    const QColor pickedColor();
    void setColorPickerType(const ColorPickerType type = ColorPickerType::CT_horizontal);

private:
    void initUI();

public slots:
    void onPickedColor(QAbstractButton *btn);

private:
    bool                     m_bRainbow;
    QSize                    m_size;
    QGridLayout*             m_gridLayout;
    QToolButton*             m_pickedBtn;
    ColorPickerType          m_colorPickerType;
    QPointer<QButtonGroup>   m_colorsGroup;
};

#endif // COLORPICKER_H
