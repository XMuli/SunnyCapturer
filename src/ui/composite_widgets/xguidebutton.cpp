#include "xguidebutton.h"


XGuideButton::XGuideButton(GuidTipsType type, QWidget *parent)
    : QWidget(parent)
    , m_type(type)
    , m_margin(2, 2, 2, 2)
    , m_spaceStr(qMax(m_margin.left() + m_margin.right(), 14))
    , m_white(255, 255, 255, 0.7 * 255)
    , m_charSpacing(10)
{
    updateSize();
}



void XGuideButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 设置背景色
    // painter.setBrush(QColor(255, 0, 0, 0.4 * 255)); // 0.4 的红色

    // 绘制整体的外边框
    QRect borderRect = rect().adjusted(0, 0, -1, -1);
    painter.drawRect(borderRect);

    // 设置字体和颜色
    painter.setFont(font());
    painter.setPen(m_white);

    QString text = getTextForType(m_type);
    QFontMetrics fm(font());
    int textWidth = fm.horizontalAdvance(text);
    int textHeight = 30; // 固定高度为 30 像素

    if (m_type == GTT_WSAD || m_type == GTT_azimuth_arrow) {
        int charWidth = textHeight; // 使用字符高度作为正方形的边长
        int charHeight = textHeight;// 使用字符高度作为正方形的边长
        int totalWidth = text.length() * charWidth + (text.length() - 1) * m_charSpacing;
        QRect charRect(0, 0, charHeight, charHeight); // 使用字符高度作为正方形的边长
        for (int i = 0; i < text.length(); ++i) {
            charRect.moveLeft(i * (charWidth + m_charSpacing));
            painter.drawRect(charRect);
            painter.drawText(charRect, Qt::AlignCenter, QString(text[i]));
        }
    } else {
        QRect textRect(borderRect.topLeft(), QSize(textWidth + m_spaceStr, textHeight));  //borderRect.adjusted(m_margin.left(), m_margin.top(), -m_margin.right(), -m_margin.bottom());
        painter.drawRect(textRect);
        painter.drawText(textRect, Qt::AlignCenter, text);
    }
}

void XGuideButton::updateSize()
{
    QString text = getTextForType(m_type);
    QFontMetrics fm(font());

    // 计算整体大小
    if (m_type == GTT_WSAD || m_type == GTT_azimuth_arrow) {
        int charWidth = 30; // 固定边长为 30 像素
        int charHeight = 30; // 固定边长为 30 像素
        int totalWidth = text.length() * charWidth + (text.length() - 1) * m_charSpacing + + m_margin.left() + m_margin.right();
        setFixedSize(totalWidth, charHeight + m_margin.top() + m_margin.bottom());
    } else {
        int textWidth = fm.horizontalAdvance(text) + m_spaceStr;
        int textHeight = 30; // 固定高度为 30 像素
        setFixedSize(textWidth + m_margin.left() + m_margin.right(), textHeight + m_margin.top() + m_margin.bottom());
    }
}

QString XGuideButton::getTextForType(GuidTipsType type)
{
    switch (type) {
    case GTT_WSAD:
        return "WSAD";
    case GTT_azimuth_arrow:
        return "←↑→↓";
    case GTT_tab:
        return "Tab";
    case GTT_shift:
        return "Shift";
    case GTT_ctrl:
        return "Ctrl";
    case GTT_quoteleft:
        return "`";
    case GTT_empty:
    default:
        return "";
    }
}

// 需要改变 type 时，对外使用的接口
// void XGuideButton::setType(GuidTipsType type)
// {
//     m_type = type;
//     updateSize();
//     update();
// }
