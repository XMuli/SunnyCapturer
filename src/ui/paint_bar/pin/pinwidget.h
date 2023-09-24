#ifndef PINWIDGET_H
#define PINWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QPixmap>
#include <QPointer>
#include <QPaintEvent>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QTimer>
#include "../../screenshot/capturehelper.h"

namespace Ui {
class PinWidget;
}

class PinWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PinWidget(const QPixmap &pixmap, QWidget *parent = nullptr);
    virtual ~PinWidget();

private:
    void initUI();
    void initMenu();
    void setScaledPixmapToLabel(const QSize& newSize, const qreal scale, const bool expanding);

private slots:
    void onCopy();
    void onSave();
    void onOpacity(const int& opacity);
    void changeShadowColor();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

private:
    Ui::PinWidget*                      ui;
    Node                                m_node;
    QMenu*                              m_menu;
    QTimer*                             m_timer;
    QPixmap                             m_pixmap;
    QPointer<QGraphicsDropShadowEffect> m_shadowEffect;
};

#endif // PINWIDGET_H
