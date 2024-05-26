#ifndef XGUIDEBUTTON_H
#define XGUIDEBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QFontMetrics>
#include <QRect>
#include <QString>
#include <QColor>
#include <QDebug>

enum GuidTipsType {
    GTT_empty,
    GTT_WSAD,
    GTT_azimuth_arrow,
    GTT_tab,
    GTT_shift,
    GTT_ctrl,
    GTT_quoteleft
};

class XGuideButton : public QWidget {
public:
    XGuideButton(GuidTipsType type, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    void updateSize();
    QString getTextForType(GuidTipsType type);
    bool isSingleSquareLetter() const;

private:
    GuidTipsType m_type;
    QRect m_margin;
    const int m_spaceStr;
    QColor m_white;
    int m_charSpacing;
};

#endif // XGUIDEBUTTON_H
