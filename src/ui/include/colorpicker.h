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



class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPicker(QWidget *parent = nullptr);
    explicit ColorPicker(QSize size = QSize(14, 14), const ColorPickerType type = ColorPickerType::CT_horizontal, QWidget *parent = nullptr);

public:
    const QColor pickedColor();
    void setCurrPickedColor(const QString &color);
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
