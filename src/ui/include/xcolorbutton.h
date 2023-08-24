#ifndef XCOLORBUTTON_H
#define XCOLORBUTTON_H

#include "xwidget_global.h"
#include <QToolButton>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>

class XColorButton : public QToolButton
{
    Q_OBJECT
public:
    explicit XColorButton(QWidget *parent = nullptr);
    virtual ~XColorButton() = default;

    QColor color() const;
    void setColor(const QColor &newColor);

    bool bRainbow() const;
    void setBRainbow(bool newBRainbow);

private:
    void setConicalGradientColor(QPainter &pa);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QColor m_color;
    bool   m_bRainbow;      // false:纯色   true:彩色
};

#endif // XCOLORBUTTON_H
