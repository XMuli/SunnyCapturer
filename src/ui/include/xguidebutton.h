#ifndef XGUIDEBUTTON_H
#define XGUIDEBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QFontMetrics>
#include <QRect>
#include <QString>
#include <QColor>
#include <QDebug> // 用于调试的头文件

// 在类外定义枚举
enum GuidTipsType {
    GTT_empty,
    GTT_WSAD,              // W S A D
    GTT_azimuth_arrow,     // 方位箭头 ← ↑ → ↓
    GTT_tab,
    GTT_shift,
    GTT_ctrl,
    GTT_quoteleft        // 重音键 `
};

class XGuideButton : public QWidget {
public:
    XGuideButton(GuidTipsType type, QWidget *parent = nullptr);
    // void setType(GuidTipsType type);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    void updateSize();
    QString getTextForType(GuidTipsType type);

private:
    GuidTipsType m_type;
    QRect m_margin;          // 整体外边框的额外边距
    const int m_spaceStr;    // Tab 这种前后加一点空白
    QColor m_white;          // 字符串颜色
    int m_charSpacing;       // 字符间隔
};


#endif // XGUIDEBUTTON_H
