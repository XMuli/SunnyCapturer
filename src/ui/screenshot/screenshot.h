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

    QScreen* currentScreen(const QPoint &pos = QCursor::pos()) const;

    void preDestruction();             // 销毁对象之前的清理工作
    void monitorsInfo() const;

    void dealMousePressEvent(QMouseEvent *e);
    void dealMouseReleaseEvent(QMouseEvent *e);
    void dealMouseMoveEvent(QMouseEvent *e);

protected:
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

    bool                 m_bAutoDetectRect;    // 是否自动监测矩形
    ActionType           m_actionType;         // 当前的操作状态
    PainterEnv           m_paEnv;              // 绘画环境
    Node                 m_node;               // 一次操作的集合
};

#endif // SCREENSHOT_H
