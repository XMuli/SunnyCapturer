#ifndef PAINTCTRLBAR_H
#define PAINTCTRLBAR_H

#include <QPointer>
#include <QWidget>
#include <QSlider>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFontComboBox>
#include "colorpicker.h"
#include "common/absbtnsctrl.h"
#include "paintbarhelper.h"

class PaintCtrlBar : public QWidget
{
    Q_OBJECT
public:
    explicit PaintCtrlBar(const Qt::Orientation& orie = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~PaintCtrlBar();


private:
    void initUI();
    void initBtns();
    void hideAllBtnsCtrl();

    void addWidget(QWidget *w, const bool& bAddSpaceLine = true, int stretch = 0, Qt::Alignment alignment = Qt::AlignCenter);

public slots:
    void onIdReleased(int id);
    void onPaintBtnRelease(const PaintType& type, const bool &isCheckable);

signals:
    void sigWidgetResized();                              // layout change completed


private:
    QBoxLayout*              m_layout;
    Qt::Orientation          m_orie;

    QPointer<AbsBtnsCtrl>    m_rectCtrl;                      // 矩形
    QPointer<AbsBtnsCtrl>    m_ellipseCtrl;                   // 圆形
    QPointer<AbsBtnsCtrl>    m_arrowCtrl;                     // 箭头
    QPointer<AbsBtnsCtrl>    m_mosaicCtrl;                    // 马赛克
    QPointer<AbsBtnsCtrl>    m_textCtrl;                      // 文本
    QPointer<AbsBtnsCtrl>    m_serialCtrl;                    // 序号图形
    QPointer<AbsBtnsCtrl>    m_pointCtrl;                     // 线宽
    QPointer<AbsBtnsCtrl>    m_markerPenCtrl;                  // 记号笔
    QPointer<ColorPicker>    m_colorPicker;                   // 取色板
    QPointer<QFontComboBox>  m_fontFamily;
    QPointer<QComboBox>      m_fontScale;
    QPointer<QSlider>        m_scrollBar;

};

#endif // PAINTCTRLBAR_H
