// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XOCRDLG_H
#define XOCRDLG_H

#include <QAction>
#include <QScrollBar>
#include <QWidget>

namespace Ui {
class XOcrDlg;
}

class XOcrDlg : public QWidget
{
    Q_OBJECT

public:
    explicit XOcrDlg(QWidget *parent = nullptr);
    ~XOcrDlg();

    void setLeftPixmap(const QPixmap& pix);
    void setRightText(const QString& text);
    void appendRightText(const QString& text);
    void setOcrSize();

private slots:
    void zoomIn();
    void zoomOut();
    void normalSize();

private:
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

private:
    Ui::XOcrDlg *ui;
    double  m_scaleFactor = 1;
//    QAction *m_actZoomIn;
//    QAction *m_actZoomOut;
//    QAction *m_actNormalSize;
};

#endif // XOCRDLG_H
