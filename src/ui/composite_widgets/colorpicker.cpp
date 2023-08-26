#include "colorpicker.h"

#include <QVariant>

ColorPicker::ColorPicker(QWidget *parent)
    : QWidget(parent)
    , m_bRainbow(false)
    , m_gridLayout(new QGridLayout(this))
    , m_pickedBtn(nullptr)
    , m_colorPickerType(ColorPickerType::Horizontal)
    , m_colorsGroup(new QButtonGroup(this))
{
    initUI();
}

const QColor ColorPicker::pickedColor()
{
    const QColor& color = m_pickedBtn ? QColor(m_pickedBtn->property("color").toString()) : QColor();
    return color;
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

        btn->setFixedSize(20, 20);
        btn->setProperty("color", colors.at(i));
        btn->setStyleSheet(QString("background-color: %1;").arg(colors.at(i)));
        btn->setToolButtonStyle(Qt::ToolButtonTextOnly);
        btn->setAutoRaise(true);
        btn->setCheckable(true);
        m_colorsGroup->addButton(btn);

        if (m_colorPickerType == ColorPickerType::Horizontal) {
            m_gridLayout->addWidget(btn, 0, i);
        } else if (m_colorPickerType == ColorPickerType::Grid) {
            m_gridLayout->addWidget(btn, i <= 3 ? 0 : 1, i % 4);
        }
    }

    connect(m_colorsGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonReleased), this, &ColorPicker::onPickedColor);
}

void ColorPicker::onPickedColor(QAbstractButton *btn)
{
    if (!btn) return;
    QToolButton* t = qobject_cast<QToolButton *>(btn);
    if(t) m_pickedBtn = t;
}
