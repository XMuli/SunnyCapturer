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
#include "capturehelper.h"
#include "windowsrect.h"

class ScreenShot : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenShot(QWidget *parent = nullptr);
    virtual ~ScreenShot();

    void capture();

private:
    void initUI();
    void drawShadowOverlay(const QRect& fullRect, const QRect& pickedRect, QPainter& pa) const;
    void drawBorderFlipped(QPainter& pa, const QRect& rt) const;
    void drawBorderMacOS(QPainter& pa, const QRect& rt, int num = 8) const;
    void drawBorderDDE(QPainter& pa, const QRect& rt, int num = 8) const;
    void originalPixmap();
    void setCursorShape(const OrientationType &type, const QPoint &pt);
    void setMovePickedRect();

    QScreen* currentScreen(const QPoint &pos = QCursor::pos()) const;
    void preDestruction();             // 销毁对象之前的清理工作
    void monitorsInfo() const;
    void rectNodesMapFromGlobal();
    void firstRectNodesAssignmentNode();  // 枚举遍历的窗口信息，将第一个赋值给 m_node

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
//    void wheelEvent(QWheelEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

private:
    QScreen*             m_primaryScreen;
    QList<QScreen *>     m_screens;
    QPixmap              m_origPix;            // 原始象图, 初次赋值后，设计为只读模式
    QRect                m_vdRect;             // virtual Desktop Rect;

    bool                 m_bFistPressed;       // true-已经按下; false-还没有按过 是否按下过第一次
    bool                 m_bAutoDetectRect;    // 是否自动监测矩形
    ActionType           m_actionType;         // 当前的操作状态
    PainterEnv           m_paEnv;              // 绘画环境
    Node                 m_node;               // 一次操作的集合

    OrientationType      m_stretchPickedRectOrieType;

    std::vector<RectNode> m_rectNodes;


};


#if defined(Q_OS_WIN)
    const QRect rectToQRect(const RECT& rect);
#elif defined(Q_OS_LINUX)

#elif defined(Q_OS_MAC)
#endif


#endif // SCREENSHOT_H
