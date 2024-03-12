#ifndef XIMAGEVIEWER_H
#define XIMAGEVIEWER_H

#include <QScrollBar>
#include <QWidget>
#include "ximagedlg.h"
namespace Ui {
class XImageViewer;
}

class XImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit XImageViewer(QWidget *parent = nullptr);
    ~XImageViewer();

    void setLeftPixmap(const QPixmap& pix);
    void setRightText(const QString& text);
    void appendRightText(const QString& text);

private slots:
    void zoomIn();
    void zoomOut();
    void normalSize();

private:
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

private:
    Ui::XImageViewer *ui;
    double m_scaleFactor;
};

#endif // XIMAGEVIEWER_H
