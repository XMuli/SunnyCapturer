// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XFRAMELESSWIDGET_H
#define XFRAMELESSWIDGET_H

#include <QWidget>
#include <QPointer>

class QMenu;
class QLabel;
class QPoint;
class QPixmap;
class QMouseEvent;
class QWheelEvent;
class QGraphicsDropShadowEffect;

class PinWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PinWidget(const QPixmap& pixmap, const QRect& geometry, QWidget* parent = nullptr);

private:
    void initUI();
    void setScaledPixmapToLabel(const QSize& newSize, const qreal scale, const bool expanding);
    void createShadowEffect();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private slots:
    void onCopy();
    void onSave();

    void onWindowShadow(bool enable);
    void onOpacity(int opacity);
    void onMaxSize(double val);

private:
    QPoint m_p;   // 窗口的左上角
    QPoint m_p1;
    QPoint m_p2;

    QPixmap m_pixmap;
    QLabel* m_label;
    QPointer<QGraphicsDropShadowEffect> m_shadowEffect;

    QMenu* m_menu;
};

#endif // XFRAMELESSWIDGET_H
