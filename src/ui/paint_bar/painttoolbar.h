#ifndef PAINTTOOLBAR_H
#define PAINTTOOLBAR_H
#include <vector>
#include <QToolButton>
#include <QWidget>
#include <QGridLayout>
#include <QPointer>
#include "paintctrlbar.h"
#include "paintbarhelper.h"



class PaintToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit PaintToolBar(const Qt::Orientation& orie = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~PaintToolBar() = default;

private:
    void initUI();
    void initBtns();

public slots:
    void onBtnReleased();

signals:


private:
    QGridLayout*          m_layout;
    std::vector<PaintBtn> m_btns;
    Qt::Orientation       m_orie;

    QPointer<PaintCtrlBar> m_paintCtrlBar;
};

#endif // PAINTTOOLBAR_H
