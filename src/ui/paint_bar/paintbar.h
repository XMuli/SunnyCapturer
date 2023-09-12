#ifndef PAINTBAR_H
#define PAINTBAR_H
#include <QWidget>
#include <QPointer>
#include <QBoxLayout>
#include "xblureffect.h"
#include "toolbar_level/painttoolbar.h"
#include "toolbar_level/paintctrlbar.h"

class PaintBar : public QWidget
{
    Q_OBJECT
public:
    explicit PaintBar(const Qt::Orientation& orie = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~PaintBar();

    void transposePaintBar(const bool& bTranspose = false);  // true-PaintToolBar在上，PaintCtrlBar在下; false- 颠倒一下顺序
    bool hadDrawBtnsChecked() const;
    void setLowerBlurEffect(const QPixmap& pix, int radius);

private:
    void initUI();
    void initConnect();

signals:
    void sigUpdatePaintBarBlurPixmap();
    void sigTextCtrlToggled(const TextFlags& flages);
    void sigPointCtrlReleased(const int& id);                           // PaintCtrlBar PointCtrl 被按下
    void sigMosaicSliderValueChanged(int id, int val);                                 // PaintCtrlBar Slider 数值被发生改变时候
    void sigPickedColor(const QColor& color);                                          // PaintCtrlBar 调色盘 选取新颜色时候

    void sigPaintToolBtnsRelease(const PaintType& type, const bool& isCheckable);      // paintToolBar 按钮被按下
    void sigPaintCtrlIdReleased(const int& id);                                        // PaintCtrlBar 按钮被按下


private slots:
    void onPaintToolBtnsRelease(const PaintType& type, const bool& isCheckable);

protected:
    void resizeEvent(QResizeEvent *e) override;
    void enterEvent(QEvent *e) override;

private:
    QBoxLayout*                     m_layout;
    Qt::Orientation                 m_orie;
    QPointer<XBlurEffect>           m_blurEffect;
    QPointer<PaintToolBar>          m_paintToolBar;
    QPointer<PaintCtrlBar>          m_paintCtrlBar;
};

#endif // PAINTBAR_H

