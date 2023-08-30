#include "colorpicker.h"


#include <QVariant>

ColorPicker::ColorPicker(QWidget *parent)
    : QWidget(parent)
    , m_bRainbow(false)
    , m_size(QSize(16, 16))
    , m_gridLayout(new QGridLayout(this))
    , m_pickedBtn(nullptr)
    , m_colorPickerType(ColorPickerType::CT_horizontal)
    , m_colorsGroup(new QButtonGroup(this))
{
    initUI();
}

ColorPicker::ColorPicker(QSize size, const ColorPickerType type, QWidget *parent)
    : QWidget(parent)
    , m_bRainbow(false)
    , m_size(size)
    , m_gridLayout(new QGridLayout(this))
    , m_pickedBtn(nullptr)
    , m_colorPickerType(type)
    , m_colorsGroup(new QButtonGroup(this))
{
    initUI();
}

const QColor ColorPicker::pickedColor()
{
    const QColor& color = m_pickedBtn ? QColor(m_pickedBtn->property("color").toString()) : QColor();
    return color;
}

//void ColorPicker::setSize(QSize size)
//{
//    m_size = size;
//}

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
    const QStringList& colors = {  "#DB000F", "#FFCF53", "#12F63B", "#0E70FF"
                                 , "#FB4288", "#323232", "#FBFBFB" , ""};

    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    m_colorsGroup->setExclusive(true);
    for (int i = 0; i < colors.count(); ++i) {
        auto btn = new QToolButton(this); // XColorButton
//        if (i == colors.count() - 1) {
//            btn->setBRainbow(true);
//        } else {
//            btn->setColor(QColor(colors.at(i)));
//        }

        btn->setFixedSize(m_size);
        btn->setProperty("color", colors.at(i));
        btn->setStyleSheet(QString("background-color: %1;").arg(colors.at(i)));
        btn->setToolButtonStyle(Qt::ToolButtonTextOnly);
        btn->setAutoRaise(true);
        btn->setCheckable(true);
        m_colorsGroup->addButton(btn);

        if (m_colorPickerType == ColorPickerType::CT_horizontal) {
            m_gridLayout->addWidget(btn, 0, i);
        } else if (m_colorPickerType == ColorPickerType::CT_grid_horizontal) {
            m_gridLayout->addWidget(btn, i <= 3 ? 0 : 1, i % 4);
        } else if (m_colorPickerType == ColorPickerType::CT_grid_vertical) {
            m_gridLayout->addWidget(btn, i % 4, i <= 3 ? 0 : 1);
        }

        m_gridLayout->setHorizontalSpacing(3);
        m_gridLayout->setVerticalSpacing(3);
    }

    connect(m_colorsGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonReleased), this, &ColorPicker::onPickedColor);
}

void ColorPicker::onPickedColor(QAbstractButton *btn)
{
    if (!btn) return;
    QToolButton* t = qobject_cast<QToolButton *>(btn);
    if(t) m_pickedBtn = t;
}
