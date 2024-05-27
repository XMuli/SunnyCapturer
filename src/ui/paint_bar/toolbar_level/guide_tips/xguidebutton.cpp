#include "xguidebutton.h"
#include "communication.h"

XGuideButton::XGuideButton(GuidTipsType type, QWidget *parent)
    : QWidget(parent)
    , m_type(type)
    , m_margin(2, 2, 2, 2)
    , m_spaceStr(qMax(m_margin.left() + m_margin.right(), 14))
    , m_white(255, 255, 255, 0.9 * 255)
    , m_charSpacing(10)
{
    m_textHeight = 20 * cursorScrnScale(false); // 固定高度为 20 像素
    updateSize();
}

void XGuideButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    // 设置背景色
    // painter.setBrush(QColor(255, 0, 0, 0.4 * 255)); // 0.4 的红色

    // 绘制整体的外边框
    QRect borderRect = rect();
    painter.drawRect(borderRect);

    // 设置字体和颜色
    painter.setFont(font());
    painter.setPen(QPen(m_white, 1));

    QString text = getTextForType(m_type);
    QFontMetrics fm(font());

    int textHeight = m_textHeight;
    int cubeSize = (textHeight - 2) / 2; // 每个小正方形的边长是文本高度减去 2 的一半
    int gap = 5; // 小正方形之间的间隙

    if (m_type == GTT_ctrl || m_type == GTT_shift) {
        // 绘制文字和边框
        QRect textRect = QRect(borderRect.left() + m_margin.left(), borderRect.top() + m_margin.top(), fm.horizontalAdvance(text) + m_spaceStr, borderRect.height() - m_margin.top());
        painter.drawRect(textRect);
        painter.drawText(textRect, Qt::AlignCenter, text);

        // 绘制加号
        int plusSize = fm.horizontalAdvance("+") + m_charSpacing;
        QRect plusRect(textRect.right() + gap, textRect.top(), plusSize, textHeight);
        // painter.drawRect(plusRect);
        painter.drawText(plusRect, Qt::AlignCenter, "+");

        // 绘制方位提示的小正方形
        QRect topRect(plusRect.right() + cubeSize + gap * 2  , textRect.top(), cubeSize, cubeSize);
        painter.drawRect(topRect);

        QRect leftRect(topRect.left() - cubeSize - gap, topRect.bottom() + gap, cubeSize, cubeSize);
        QRect middleRect(topRect.left(), leftRect.top(), cubeSize, cubeSize);
        QRect rightRect(topRect.left() + cubeSize + gap, leftRect.top(), cubeSize, cubeSize);

        painter.drawRect(leftRect);
        painter.drawRect(middleRect);
        painter.drawRect(rightRect);
    } else if (m_type == GTT_mouse_wheel) {
        // 计算基于 textHeight 的尺寸
        QRect rt = QRect(borderRect.left() + m_margin.left(), borderRect.top() + m_margin.top(), textHeight, textHeight);
        QPixmap pix(":/resources/icons/guide_tips/wheel_mouse.svg");
        painter.drawPixmap(rt, pix);

    } else if (isSingleSquareLetter()) {
        int charWidth = textHeight;
        int totalWidth = text.length() * charWidth + (text.length() - 1) * m_charSpacing;
        QRect charRect(m_margin.left(), m_margin.top(), charWidth, charWidth);
        for (int i = 0; i < text.length(); ++i) {
            charRect.moveLeft(i * (charWidth + m_charSpacing));
            painter.drawRect(charRect);
            painter.drawText(charRect, Qt::AlignCenter, QString(text[i]));
        }
    } else {
        QRect textRect(borderRect.topLeft() + QPoint(m_margin.left(), m_margin.top()), QSize(fm.horizontalAdvance(text) + m_spaceStr, textHeight));
        painter.drawRect(textRect);
        painter.drawText(textRect, Qt::AlignCenter, text);
    }

    painter.restore();
}

void XGuideButton::updateSize()
{
    QString text = getTextForType(m_type);
    QFontMetrics fm(font());

    if (m_type == GTT_ctrl || m_type == GTT_shift) {
        int textWidth = fm.horizontalAdvance(text) + m_spaceStr;
        int textHeight = m_textHeight; // 固定高度为 30 像素
        int cubeSize = (textHeight - 2) / 2;
        int gap = 5;
        int plusSize = fm.horizontalAdvance("+") + m_charSpacing;
        int totalWidth = textWidth + gap * 4 + cubeSize * 3 + plusSize;
        setFixedSize(totalWidth + m_margin.left() + m_margin.right(), textHeight + m_margin.top() + m_margin.bottom());
    } else if (m_type == GTT_mouse_wheel) {
        int ellipseWidth = m_textHeight;
        int ellipseHeight = m_textHeight;
        setFixedSize(ellipseWidth + m_margin.left() + m_margin.right(), ellipseHeight + m_margin.top() + m_margin.bottom());
    } else if (isSingleSquareLetter()) {
        int charWidth = m_textHeight;
        int charHeight = m_textHeight;
        int totalWidth = text.length() * charWidth + (text.length() - 1) * m_charSpacing + m_margin.left() + m_margin.right();
        setFixedSize(totalWidth, charHeight + m_margin.top() + m_margin.bottom());
    } else {
        int textWidth = fm.horizontalAdvance(text) + m_spaceStr;
        int textHeight = m_textHeight;
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
    case GTT_mouse_wheel: // Added new case
        return "Mouse Wheel";
    case GTT_empty:
    default:
        return "";
    }
}

bool XGuideButton::isSingleSquareLetter() const
{
    return (m_type == GTT_WSAD || m_type == GTT_azimuth_arrow || m_type == GTT_quoteleft);
}

int XGuideButton::textHeight() const
{
    return m_textHeight;
}

void XGuideButton::setTextHeight(int newTextHeight)
{
    m_textHeight = newTextHeight;
    updateSize();
}
