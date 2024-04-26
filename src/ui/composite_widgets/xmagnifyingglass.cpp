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
#include <QDebug>
#include <QPainter>
#include "../../ui/paint_bar/toolbar_level/paintbarhelper.h"


XMagnifyingGlass::XMagnifyingGlass(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XMagnifyingGlass)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TransparentForMouseEvents, true); // 鼠标穿透，设置窗口忽略所有鼠标事件
    // setMouseTracking(false); // 确保窗口不会跟踪鼠标移动事件

    QList<QLabel *> labelList = findChildren<QLabel *>();

    // 设置字体、字号和颜色
    QFont font("Arial", 12); // 设置字体和字号
    QPalette palette = labelList.first()->palette(); // 获取第一个 QLabel 的 palette
    palette.setColor(QPalette::WindowText, Qt::white); // 设置文本颜色为白色

    // 遍历所有 QLabel 对象，设置样式
    for (auto label : labelList) {
        // label->setFont(font); // 设置字体和字号
        label->setPalette(palette); // 设置文本颜色
    }

    ui->tbColor->setColor(m_color);
}

XMagnifyingGlass::~XMagnifyingGlass()
{
    delete ui;
}

void XMagnifyingGlass::setPixmap(const QPoint &pt)
{
    const int rX = 12;  // 宽、高的一半
    const int rY = 9;   // 太大则会将其放大镜部分左上角也截图上来
    const QPixmap& pix = qGuiApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), pt.x() - rX, pt.y() - rY, 2 * rX, 2 * rY);
    QLabel* lab = ui->labPixmap;

    const QScreen* screen = QGuiApplication::screenAt(QCursor::pos());
    const int& scale = 1; //qCeil(dpiScale(screen));  // 如为 150% 缩放像素会异常
    lab->setFixedSize(QSize(rX, rY) * 12 * scale);  // 默认 14 倍;
    const double pixScale = (lab->width() / (rX * 1.0)) * scale ;
    lab->clear(); // 清除之前的内容

    // 将 pix 绘制到 QLabel 上
    QPixmap pixmap = pix.scaled(lab->size());
    QPainter pa(&pixmap);

    // 绘制红色十字线
    const int offset = pixScale / 4;
    int centerSize = pixScale / 2; // 中心点正方形的边长
    int centerY = pixmap.height() / 2 - centerSize / 2;
    int centerX = pixmap.width() / 2 - centerSize / 2;
    QRect rect = QRect(centerX + offset, centerY + offset, centerSize, centerSize).adjusted(-1, -1, 0, 0);
    QImage image = pixmap.toImage();
    m_color = image.pixelColor(rect.center());  // 绘画之前就先取色
    // 绘画十字线
    pa.setBrush(Qt::NoBrush);
    pa.setPen(QPen(highlightColor(0.2), pixScale / 2, Qt::SolidLine));
    pa.drawLine(0, pixmap.height() / 2 + offset, pixmap.width(), pixmap.height() / 2+ offset);
    pa.drawLine(pixmap.width() / 2 + offset, 0, pixmap.width() / 2  + offset, pixmap.height());
    // 绘制中心点的黑色边框正方形
    pa.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pa.drawRect(rect);
    pa.setPen(QPen(Qt::white, 1, Qt::SolidLine));
    pa.setBrush(QBrush(m_color));
    pa.drawRect(rect.adjusted(1, 1, -1, -1));

    // 在 QLabel 上显示绘制好的 pixmap
    lab->setPixmap(pixmap);

    const QPoint& p1 = QCursor::pos();
    ui->labPtAbsolute->setText(QString(tr("(%1, %2)")).arg(p1.x()).arg(p1.y()));
    ui->tbColor->setColor(m_color);

    ui->labColorName->setText(m_color.toHsv().name());
    ui->widget->setFixedWidth(ui->labPixmap->width());
    adjustSize();
    update();
}



void XMagnifyingGlass::paintEvent(QPaintEvent *e)
{
    QPainter pa(this);
    pa.setPen(Qt::NoPen);
    pa.setBrush(Qt::NoBrush);
    pa.fillRect(rect(), QColor(0, 0, 0, 255 * 0.65));

    const QRect& pixRt = ui->labPixmap->rect();
    const QRect& rt = QRect(ui->labPixmap->mapToParent(pixRt.topLeft()), pixRt.size());
    int margin = 1;
    pa.setPen(QPen(Qt::white, 1));
    pa.drawRect(rt.adjusted(-margin, -margin, margin-1, margin-1));
    margin = 2;
    pa.setPen(QPen(Qt::black, 1));
    pa.drawRect(rect());
    pa.drawRect(rt.adjusted(-margin, -margin, margin-1, margin-1));
}
