// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XOCRDLG_H
#define XOCRDLG_H

#include <QAction>
#include <QMouseEvent>
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

// private slots:
//     void onZoomIn();
//     void onZoomOut();
//     void onNormalSize();

// private:
//     void pixmapMoveCenter();
//     void updateTitleText();
//     void scaleImage(double factor);

// protected:
//     void wheelEvent(QWheelEvent *e) override;

private:
    Ui::XOcrDlg *ui;
    double  m_scaleFactor;
};

#endif // XOCRDLG_H
