#ifndef PAINTTOOLBAR_H
#define PAINTTOOLBAR_H
#include <vector>
#include <memory>
#include <QToolButton>
#include <QWidget>
#include <QGridLayout>
#include <QPointer>
#include "xblureffect.h"
#include "paintctrlbar.h"
#include "paintbarhelper.h"

class PaintToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit PaintToolBar(const Qt::Orientation& orie = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~PaintToolBar() = default;

    bool hadPaintBtnChecked();
    void setLowerBlurEffect(const QPixmap& pix, int radius);

private:
    void initUI();
    void initBtns();
    void paintBtnsExclusive(const QToolButton* tBtn = nullptr, const bool& bSpik = true);
    void setLayoutSpacing(int horSpace = 0, int verSpace = 0);   // fix: 当有两行时， 其会多一个 hor spaceing 长度；暂都设置为 0
    void setPaintCtrlBarToLayout();
    void printfAllItems(const QString& prompted);

//    void onBtnRectangle();
//    void onBtnEllipse();
//    void onBtnArrow();
//    void onBtnPencil();
//    void onBtnMosaic();
//    void onBtnMarkerPen();
//    void onBtnText();
//    void onBtnSerial();
//    void onBtnPin();
//    void onBtnUndo();
//    void onBtnRedo();
//    void onBtnSave();
//    void onBtnCancel();
//    void onBtnFinis();


protected:
    void resizeEvent(QResizeEvent *e) override;
    void enterEvent(QEvent *e) override;

public slots:
    void onPaintBtnReleased();

private:
    QGridLayout*                    m_layout;
    std::vector<PaintBtn>           m_btns;
    Qt::Orientation                 m_orie;
    std::unique_ptr<XBlurEffect>    m_blurEffect;
    QPointer<PaintCtrlBar>          m_paintCtrlBar;
};

#endif // PAINTTOOLBAR_H
