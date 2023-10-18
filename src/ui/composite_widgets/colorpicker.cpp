#include "colorpicker.h"
#include <QVariant>
#include <QColorDialog>
#include <QDebug>
#include "communication.h"

ColorPicker::ColorPicker(QWidget *parent)
    : QWidget(parent)
    , m_size(QSize(23, 23))
    , m_gridLayout(new QGridLayout(this))
    , m_colorPickerType(ColorPickerType::CT_horizontal)
    , m_colorsGroup(new QButtonGroup(this))
{
    initUI();
}

ColorPicker::ColorPicker(QSize size, const ColorPickerType type, QWidget *parent)
    : QWidget(parent)
    , m_size(size)
    , m_gridLayout(new QGridLayout(this))
    , m_colorPickerType(type)
    , m_colorsGroup(new QButtonGroup(this))
{
    initUI();
}

const QColor ColorPicker::pickedColor()
{
    return m_pickedBtn->color();
}

void ColorPicker::setCurrPickedColor(const QString &color)
{
//    int i = 0;
    m_pickedBtn->setColor(color);
    if (m_colorPickerType == ColorPickerType::CT_horizontal) {
        for (auto it : findChildren<XColorButton*>()) {

//            qDebug() << "i:" << i++ << it->color().name() << color << bool(it->color().name() == color);
            if (it != m_pickedBtn && it->color().name() == color) {
                it->setChecked(true);
                break;
            }
        }
    }
}

void ColorPicker::setColorPickerType(const ColorPickerType type)
{
    if (m_colorPickerType != type) {
        // Deleting all items in the layout
        while (QLayoutItem* item = m_gridLayout->takeAt(0)) {
            if (QWidget* widget = item->widget())
                widget->deleteLater(); // Ensure proper deletion
            delete item;
        }

        m_colorPickerType = type;
        initUI();
    }
}

void ColorPicker::initUI()
{
    QStringList colors;
    if (m_colorPickerType == ColorPickerType::CT_horizontal) {
//        colors << "#387bfd" << "#8c3e94" << "#e7509e" << "#d13840"
//               << "#e98226" << "#f6c632" << "#74b94a" << "#999898";
        colors << "#DF4187" << "#FF5D00" << "#F8CB00" << "#23C400"
               << "#00A48A" << "#0081FF" << "#3C02FF" << "#8C00D4" << "#4D4D4D";

    } else {
        colors << "#000000" << "#7f7f7f" << "#880015" << "#ed1c24" << "#ff7f27"
               << "#fff200" << "#22b14c" << "#00a2e8" << "#3f48cc" << "#a349a4"
               << "#ffffff" << "#c3c3c3" << "#b97a57" << "#ffaec9" << "#ffc90e"
               << "#efe4b0" << "#b5e61d" << "#99d9ea" << "#7092be" << "#c8bfe7";
    }

    const int halfCount = colors.size() / 2;
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    initPickupBtn(colors.at(3));

    m_colorsGroup->setExclusive(true);
    for (int i = 0; i < colors.count(); ++i) {
        auto btn = new XColorButton(QColor(colors.at(i)), m_colorPickerType, this);
        btn->setFixedSize(m_size);
        btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        btn->setAutoRaise(true);
        btn->setCheckable(true);
        m_colorsGroup->addButton(btn);

        int horSpace = 2;
        int verSpace = 2;
        if (m_colorPickerType == ColorPickerType::CT_horizontal) {
            m_gridLayout->addWidget(btn, 0, i);
        } else if (m_colorPickerType == ColorPickerType::CT_grid_horizontal) {
            m_gridLayout->addWidget(btn, i <= halfCount - 1 ? 0 : 1 , i % halfCount + 2);
            verSpace = 0;
        } else if (m_colorPickerType == ColorPickerType::CT_grid_vertical) {
            m_gridLayout->addWidget(btn, i % halfCount + 2, i <= halfCount - 1 ? 0 : 1);
            horSpace = 0;
        }

        m_gridLayout->setHorizontalSpacing(horSpace);
        m_gridLayout->setVerticalSpacing(verSpace);
    }

    connect(m_colorsGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonReleased), this, &ColorPicker::onPickedColor);
}

void ColorPicker::initPickupBtn(const QColor& color)
{
    m_pickedBtn = new XColorButton(color, m_colorPickerType, this);
    m_pickedBtn->setFixedSize(m_size * 2 + QSize(m_gridLayout->horizontalSpacing(), m_gridLayout->verticalSpacing()));
    m_pickedBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_pickedBtn->setAutoRaise(true);
    m_pickedBtn->setCheckable(true);
    m_pickedBtn->hide();

    if (m_colorPickerType != ColorPickerType::CT_horizontal) {
        m_gridLayout->addWidget(m_pickedBtn, 0, 0, 2, 2, Qt::AlignCenter);
        m_pickedBtn->show();
    }

    connect(m_pickedBtn, &XColorButton::released, this, &ColorPicker::onPickupBtnReleased);
}

void ColorPicker::onPickedColor(QAbstractButton *btn)
{
    if (!btn) return;
    XColorButton* t = qobject_cast<XColorButton *>(btn);
    if (t) {
        m_pickedBtn->setColor(t->color());
        m_pickedBtn->update();  // 自定义绘画的控件，发生改变需要 update 下
        emit sigPickedColor(t->color());
    }
}

void ColorPicker::onPickupBtnReleased()
{
    const QColor& color = m_pickedBtn->palette().color(QPalette::Window);
    const QColor& newColor = QColorDialog::getColor(color, this, tr("select color"), QColorDialog::ShowAlphaChannel);
    if (newColor != color) {
        m_pickedBtn->setColor(newColor);
        emit sigPickedColor(newColor);
    }
}
