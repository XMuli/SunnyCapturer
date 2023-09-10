#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QSize>
#include <QColor>
#include <QWidget>
#include <QPointer>
#include <QToolButton>
#include <QGridLayout>
#include <QButtonGroup>
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
    void initPickupBtn(const QColor &color);

signals:
    void sigPickedColor(const QColor& color);

public slots:
    void onPickedColor(QAbstractButton *btn);
    void onPickupBtnReleased();

private:
    QSize                    m_size;
    QGridLayout*             m_gridLayout;
    ColorPickerType          m_colorPickerType;
    QPointer<XColorButton>   m_pickedBtn;    // 当前选中的效果
    QPointer<QButtonGroup>   m_colorsGroup;
};

#endif // COLORPICKER_H
