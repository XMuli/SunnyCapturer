#ifndef XTOOLBUTTON_H
#define XTOOLBUTTON_H

#include <QToolButton>

class XToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit XToolButton(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // XTOOLBUTTON_H
