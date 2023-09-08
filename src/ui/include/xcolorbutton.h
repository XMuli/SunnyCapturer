#ifndef XCOLORBUTTON_H
#define XCOLORBUTTON_H

#include <QToolButton>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QMouseEvent>

class XColorButton : public QToolButton
{
    Q_OBJECT
public:
    explicit XColorButton(const QColor& color, QWidget *parent = nullptr);
    virtual ~XColorButton() = default;

    QColor color() const;
    void setColor(const QColor &newColor);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QColor m_color;
};

#endif // XCOLORBUTTON_H
