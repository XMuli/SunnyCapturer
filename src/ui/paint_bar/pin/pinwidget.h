#ifndef PINWIDGET_H
#define PINWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QPixmap>
#include <QPaintEvent>
#include "../../screenshot/capturehelper.h"


class PinWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PinWidget(const QPixmap &pixmap, QWidget *parent = nullptr);

    QPixmap pixmap() const;
    void setPixmap(const QPixmap &newPixmap);

private:
    void initUI();
    void initMenu();

signals:

private slots:
    void onCopy();
    void onSave();
    void onColse();
    void onOpacity(const int& opacity);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

private:
    Node      m_node;
    QMenu*    m_menu;
    QPixmap   m_pixmap;
};

#endif // PINWIDGET_H
