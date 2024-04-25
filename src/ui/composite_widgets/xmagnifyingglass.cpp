// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "xmagnifyingglass.h"
#include "ui_xmagnifyingglass.h"
#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QPixmap>
#include <QEvent>
#include <QPainter>
#include "../../ui/paint_bar/toolbar_level/paintbarhelper.h"

void XMagnifyingGlass::setPixmap(const QPoint &pt)
{
    const int rX = 15;  // 宽、高的一半
    const int rY = 10;   // 太大则会将其放大镜部分左上角也截图上来
    const QPixmap& pix = qGuiApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), pt.x() - rX, pt.y() - rY, 2 * rX, 2 * rY);
    QLabel* lab = ui->labPixmap;

    const QScreen* screen = QGuiApplication::screenAt(QCursor::pos());
    const double& scale = dpiScale(screen);
    lab->setFixedSize(QSize(150, 100) * scale);
    const double pixScale = (lab->width() / (rX * 1.0)) * scale ;
    lab->clear(); // 清除之前的内容

    // 将 pix 绘制到 QLabel 上
    QPixmap pixmap = pix.scaled(lab->size());
    QPainter pa(&pixmap);
    // 绘制红色十字线
    pa.setPen(QPen(QColor(255, 0, 0, 255 * 0.2), pixScale, Qt::SolidLine)); // 将 painter 改为 pa
    pa.drawLine(pixmap.width() / 2, 0, pixmap.width() / 2, pixmap.height());
    pa.drawLine(0, pixmap.height() / 2, pixmap.width(), pixmap.height() / 2);

    // 绘制中心点的黑色边框正方形
    int centerSize = 6 * scale; // 中心点正方形的边长
    int centerY = pixmap.height() / 2 - centerSize / 2;
    int centerX = pixmap.width() / 2 - centerSize / 2;
    pa.setPen(QPen(Qt::black, 1, Qt::SolidLine)); // 将 painter 改为 pa
    pa.drawRect(centerX, centerY, centerSize, centerSize);

    // 在 QLabel 上显示绘制好的 pixmap
    lab->setPixmap(pixmap);

    adjustSize();
    update();
}


XMagnifyingGlass::XMagnifyingGlass(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XMagnifyingGlass)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TransparentForMouseEvents, true); // 鼠标穿透，设置窗口忽略所有鼠标事件
    // setMouseTracking(false); // 确保窗口不会跟踪鼠标移动事件
}

XMagnifyingGlass::~XMagnifyingGlass()
{
    delete ui;
}

void XMagnifyingGlass::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    // painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.fillRect(rect(), QColor(0, 0, 0, 255 * 0.4)); // 透明度为 128 的灰色矩形覆盖整个窗口
}
