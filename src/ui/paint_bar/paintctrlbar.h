#ifndef PAINTCTRLBAR_H
#define PAINTCTRLBAR_H
#include <QGridLayout>
#include <QPointer>
#include <QWidget>
#include <QBoxLayout>
#include "absbtnsctrl.h"

class PaintCtrlBar : public QWidget
{
    Q_OBJECT
public:
    explicit PaintCtrlBar(const Qt::Orientation& orie = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~PaintCtrlBar();


private:
    void initUI();
    void initBtns();

private slots:
    void onIdReleased(int idx);

signals:

private:
    QBoxLayout*          m_layout;
    Qt::Orientation       m_orie;

    QPointer<AbsBtnsCtrl>    m_rectCtrl;                      // 矩形
    QPointer<AbsBtnsCtrl>    m_ellipseCtrl;                   // 圆形
    QPointer<AbsBtnsCtrl>    m_arrowCtrl;                     // 箭头
    QPointer<AbsBtnsCtrl>    m_mosaicCtrl;                    // 马赛克
    QPointer<AbsBtnsCtrl>    m_textCtrl;                      // 文本
    QPointer<AbsBtnsCtrl>    m_serialCtrl;                    // 序号图形
    QPointer<AbsBtnsCtrl>    m_lienWidthBar;                  // 线宽
//    QPointer<ColorParaBar> m_colorBar;                     // 取色板
};

#endif // PAINTCTRLBAR_H
