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
    void originalPixmap();
    void preDestruction();             // 销毁对象之前的清理工作

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

private:
    QScreen*             m_primaryScreen;
    QList<QScreen *>     m_screens;
    QPixmap*             m_origPix;                     // 原始象图
    QRect                m_vdRect;             // virtual Desktop Rect;
};

#endif // SCREENSHOT_H
