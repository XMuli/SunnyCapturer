// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef PICSHOT_WINFULLSCREEN_H
#define PICSHOT_WINFULLSCREEN_H

#include "rectcalcu.h"
#include <memory>
#include <set>
#include <vector>
#include <map>
#include <QWidget>
#include <QList>
#include <QColor>
#include <QScreen>
#include "drawhelper.h"
#include "../widget/xtextwidget.h"
#include "../tool/selectbar.h"
#include "../tool/parameterbar.h"
#include "../tool/selectsize/selectsize.h"

struct comp {
    bool operator()(const QRect& rt1, const QRect& rt2) const { return rt1.width() < rt2.width(); }
};

class QPixmap;
class ScreenShot : public  QWidget
{
    Q_OBJECT
public:
    ScreenShot(QWidget* parent = nullptr);
    virtual ~ScreenShot() = default;
    bool isSelBorder();
    const Qt::Orientation getBarOrien() const;
    void setBarOrien(Qt::Orientation val);
    void launchCapture(CaptureHelper::CaptureType type = CaptureHelper::SST_ScrnCapture);
    static double GetScale(const QScreen* scrn = QApplication::primaryScreen());
    static double GetDevicePixelRatio(const QScreen* scrn = QApplication::primaryScreen());

private:
    void scrnsCapture();
    void windowCapture();
    void delayCapture(int ms = 5000);
    void fullScrnCapture();
    QScreen *priScrn() const;
    QScreen *curScrn(const QPoint &pos = QCursor::pos()) const;

    void updateAutoDetectRt();
    ScrnOperate updateScrnType(const QPoint pos) const;
    void updateCursorShape(const QPoint pos);
    void updateBorderCursorShape(const CursorArea& cursArea);

    QPixmap* virtualScrnPixmap();
    bool drawToCurPixmap();
    void drawStep(QPainter& pa, const XDrawStep& step);
    void imageQuickSave();
    void imageAutoSave();

    void whichShape();
    bool isDrawShape(XDrawStep& step) const;
    void selectedShapeMove(QPainter& pa) const;                                      // Effect of selected graphics

    void drawToolBar() const;
    void drawCrosshair(QPainter& pa) const;
    void drawMaskLayer(const QRect& virGeom, const QRect& rtSel, QPainter& pa) const;
    void drawBorder(QRect& rtSel, QPainter& pa) const;
    void drawBorderFlipped(QPainter& pa, const QRect& rt, bool isRound = false) const;
    void drawBorderBlackWhite (QPainter& pa, const QRect& rt, int num = 8, bool isRound = true) const;
    void drawBorderLightBlue(QPainter& pa, const QRect& rt, int num = 8, bool isRound = true) const;
    const QVector<QPoint> drawBarPosition(Qt::Orientation orien = Qt::Horizontal, ToolBarOffset offset = ToolBarOffset::TBO_Middle) const;
    const QPoint drawSelSizeTip(const QRect& rt) const;

    void drawWinInfo(QPainter& pa) const;                                            // Drawing Window information path title etc.
    void adjustSelectedRect(QKeyEvent* e);                                           // Move or Stretch

    void scrnsInfo() const;
    void showAllDrawedShape(QPainter& pa);                                           // Test
    void showDebugInfo(QPainter& pa, QRect& rtSel);                                  // Test

signals:
    void sigClearScreen();
    void sigLineWidthChange(int width);
    void sigNotificQuickSave(const bool bSaveOk, const QString& pathName);

public slots:
    void onEnableDraw(bool enable);
    void onSelShape(DrawShape shape, bool checked);
    void onRevocation();
    void onRenewal();
    void onPin();
    void onSave();
    void onCancel();
    void onFinish();

    void onQuickSave();

    void onInterruptEdit(const QPoint& pos);
    void clearnAndClose();
    void onParaBtnId(DrawShape shape, QToolButton* tb);
    void onLineWidthChange(int width);
    void onSelColor(QColor col);
    void onClearScreen();

protected:
    QPen easyRecognizeColorPen(const QColor& color, const bool bDefaultPenWidth = false) const;
    void paintEvent(QPaintEvent *e) override;
    void keyReleaseEvent(QKeyEvent* e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent* e) override;

private:
    double m_scal;
    QRect m_captureScrnRt;                        // 截图时的桌面矩形：Mac 当前屏幕（非 virtualScerrn）；Windows 为多屏总和的 virtualScerrn
    QPixmap* m_curPix;                            // 当前屏幕截图
    QPixmap m_savePix;                            // 当前屏幕截图 + 遮罩   无构造初始化
    RectCalcu m_rtCalcu;                          // 选中矩形区域
    bool m_bSmartWin;                             // 是否开启智能窗口
    bool m_bFirstSel;                             // 初次选中 截图矩形 完成
    bool m_bFirstPress;                           // 初次 左键按下 完成(十字线)

    std::vector<XDrawStep> m_vDrawed;             // 已绘步骤
    std::vector<XDrawStep> m_vDrawUndo;           // 撤销步骤
    std::set<QRect, comp> m_scrnRts;              // 特殊窗口的矩形，绘画时需略调整
    XDrawStep* m_pCurShape;                       // 移动状态下的选中矩形； nullptr 为 最外层框， 非 nullptr 为具体选中
    static XDrawStep m_step;                      // 当前绘画一步骤

    QRect m_autoDetectRt;                         // 自动检测窗口矩形大小；用以给其它赋值
    Qt::Orientation m_barOrien;
    std::map<QScreen*, ScrnTypes> m_scrns;
    std::unique_ptr<SelectBar> m_selBar;
    std::unique_ptr<ParameterBar> m_paraBar;
    std::unique_ptr<SelectSize> m_selSizeTip;     // 选中矩形的尺寸提示
    std::unique_ptr<SelectSize> m_widthTip;       // 画笔宽度临时预览
    std::unique_ptr<XTextWidget> m_edit;
};

#endif //PICSHOT_WINFULLSCREEN_H
