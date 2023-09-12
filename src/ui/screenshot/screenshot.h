#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QWidget>
#include <QScreen>
#include <QList>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QRect>
#include <QPointer>
#include "tips.h"
#include "capturehelper.h"
#include "xtextedit.h"
#include "../paint_bar/paintbar.h"

#ifdef _WIN32
#include "ntwindowsrect.h"
#elif __linux__
#include "x11windowsrect.h"
#elif __APPLE__ || __MACH__
#endif

class ScreenShot : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenShot(const Qt::Orientation& orie = Qt::Horizontal, QWidget *parent = nullptr);  // Horizontal Vertical
    virtual ~ScreenShot();

    void capture();

private:
    void btnPin();
    void btnUndo();
    void btnRedo();
    void btnSave();
    void btnCancel();
    void btnFinish();
    QPixmap finishPixmap();
    QPixmap finishDrewPixmap(const QRect& rect = QRect());
    void originalPixmap();
    void setMosaicPix();


public slots:
    void onPaintBtnRelease(const PaintType& type, const bool& isCheckable);
    void onPaintCtrlIdReleased(const int& id);

    void onTextCtrlToggled(const TextFlags& flages);
    void onPointCtrlReleased(const int& id);
    void onMosaicSliderValueChanged(int id, int val);
    void onHidePointTips();
    void onUpdateToolBarBlurPixmap();
    void onPickedColor(const QColor& color);

private:
    void initUI();
    void initConnect();
    void drawShadowOverlay(const QRect& fullRect, const QRect& pickedRect, QPainter& pa) const;
    void drawBorderFlipped(QPainter& pa, const QRect& rt) const;
    void drawBorderMacOS(QPainter& pa, const QRect& rt, int num = 8) const;
    void drawBorderDDE(QPainter& pa, const QRect& rt, int num = 8) const;
    void stashMosaicPixmap();           // 定格此刻的图片，用作打马赛克的素材
    void setCursorShape(const OrientationType &type, const QPoint &pt);
    void setMovePickedRect();
    void showCustomWidget(QWidget* w);

    QScreen* currentScreen(const QPoint &pos = QCursor::pos()) const;
    void preDestruction();             // 销毁对象之前的清理工作
    void monitorsInfo() const;
    void rectNodesMapFromGlobal();
    void firstRectNodesAssignmentNode();  // 枚举遍历的窗口信息，将第一个赋值给 m_node
    QPoint customWidgetShowPositionRule(const CustomWidgetType& cwt);
    void showPointTips(const QString &text);
    void showPickedRectTips();

    void printfDevelopProjectInfo(QPainter &pa);
    void prinftWindowsRects(QPainter &pa);

    void dealMousePressEvent(QMouseEvent *e);
    void dealMouseReleaseEvent(QMouseEvent *e);
    void dealMouseMoveEvent(QMouseEvent *e);

protected:
    // [核心: 窗口的绘画的坐标都是 (0, 0) 开始； 对于绝对坐标，可以考虑转换为 相对坐标来计算 ！！！ 统一]
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

private:
    QScreen*                 m_primaryScreen;
    QList<QScreen *>         m_screens;
    QPixmap                  m_origPix;            // 原始象图, 初次赋值后，设计为只读模式
    QPixmap                  m_mosaicPix;          // 准备使用马赛克/完成截图相关功能时候
    QRect                    m_vdRect;             // virtual Desktop Rect;

    bool                     m_bFistPressed;       // true-已经按下; false-还没有按过 是否按下过第一次
    bool                     m_bAutoDetectRect;    // 是否自动监测矩形
    ActionType               m_actionType;         // 当前的操作状态
    Node                     m_node;               // 一次操作的集合
    QPointer<PaintBar>       m_paintBar;           // PaintBar

    PaintNode                m_paintNode;          // 当前绘画元素
    std::vector<PaintNode>   m_undo;               // 撤销-图案元素
    std::vector<PaintNode>   m_redo;               // 重做-图案元素

    OrientationType          m_stretchPickedRectOrieType;
    Qt::Orientation          m_orie;
    std::vector<RectNode>    m_rectNodes;

    QPointer<XTextEdit>      m_textEdit;
    QPointer<Tips>           m_pointTips;
    QPointer<Tips>           m_pickedRectTips;
    QTimer*                  m_timerPoint;


};


#if defined(Q_OS_WIN)
    const QRect rectToQRect(const RECT& rect);
#elif defined(Q_OS_LINUX)

#elif defined(Q_OS_MAC)
#endif


#endif // SCREENSHOT_H
