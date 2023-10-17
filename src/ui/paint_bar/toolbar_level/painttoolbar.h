#ifndef PAINTTOOLBAR_H
#define PAINTTOOLBAR_H
#include <vector>
#include <memory>
#include <QToolButton>
#include <QWidget>
#include <QBoxLayout>
#include <QGridLayout>
#include <QPointer>
#include "paintbarhelper.h"

class PaintToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit PaintToolBar(const Qt::Orientation& orie = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~PaintToolBar() = default;
    bool hadDrawBtnsChecked() const;

private:
    void initUI();
    void initBtns();
    void paintBtnsExclusive(const QToolButton* tBtn = nullptr, const bool& bSpik = true);
    PaintBtn* findPaintBtn(const PaintType& type);

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

signals:
    void sigPaintToolBtnsRelease(const PaintType& type, const bool& isCheckable);

public slots:
    void onPaintBtnReleased();
    void onAutoDisableUndoAndRedo(const bool& undoDisable, const bool& redoDisable);  // ScreenShot 控制图标置灰

private:
    Qt::Orientation m_orie;
    QBoxLayout* m_layout;
    std::vector<PaintBtn>           m_btns;
};

#endif // PAINTTOOLBAR_H
