// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "xmonitorlabel.h"
#include "qpainter.h"
#include <QDebug>
#include <QStandardPaths>


XMonitorLabel::XMonitorLabel(QWidget *parent)
    : QWidget(parent)
    , m_scrns(qGuiApp->screens())
    , m_priScrn(qGuiApp->primaryScreen())
{
    // setStyleSheet("background-color: red;");
    scrnsInfo();
}

QStringList XMonitorLabel::scrnsInfo() const
{
    int i = 1;
    QStringList lists;

    for (const auto& it : m_scrns) {
        QRect tGeometry(it->geometry());
        QString geometry = QString("(%1,%2 %3x%4)").arg(tGeometry.x()).arg(tGeometry.y()).arg(tGeometry.width()).arg(tGeometry.height());
        QSizeF tPhysicalSize(it->physicalSize());
        QString physicalSize = QString("%1x%2").arg(tPhysicalSize.width()).arg(tPhysicalSize.height());

        QString multiIdx = QString("%1 %2").arg(i++).arg(it == m_priScrn ? "[Primary]" : "");
        if (m_scrns.size() >= 1) lists << tr("Idx") << multiIdx;


        lists << tr("DevicePixelRatio") << QString::number(it->devicePixelRatio())
              << tr("Logical DPI") << QString("%1 (%2% scaling)").arg(it->logicalDotsPerInch()).arg(scaling(it) * 100)
              << tr("Physical DPI") << QString::number(it->physicalDotsPerInch())
              << tr("RefreshRate") << QString::number(it->refreshRate())
              << tr("Geometry") << geometry
              << tr("PhysicalSize") << physicalSize;
    }

    return lists;
}

const QSize& XMonitorLabel::fixSize()
{
    // 缩小比例
    qreal scaleFactor = 0.08;
    const int textMargin = 40;
    const QSize size = m_priScrn->virtualGeometry().size() * scaleFactor + QSize(textMargin, textMargin) + QSize(20, 20);  // 虚拟尺寸 + 写文字的区域 + 距离边框的距离
    return size;
}

QPixmap XMonitorLabel::renderMonitorToPixmap()
{
    qreal scaleFactor = 0.08;
    const int margin = 10;          // 距离绘画区域左上角的间隔，距离↖各10
    const int textMargin = 20;      // 显示器矩形的外轮廓填写 文本的预留间隔
    const QSize virSize = m_priScrn->virtualGeometry().size() * scaleFactor + QSize(textMargin, textMargin) * 2;  // 实际的最大虚拟屏幕的区域 + 文字轮廓
    const QRect maxRt(QPoint(margin, margin), virSize);
    const QSize& clientSize(virSize + QSize(margin, margin) * 2);   // 实际的最大虚拟屏幕的区域 + 文字轮廓 + 距离绘画区域左上角的间隔

    QPixmap pixmap(clientSize);
    pixmap.fill(Qt::transparent); // Fill it with transparent background
    QPainter pa(&pixmap);

    pa.setPen(QPen(Qt::blue, 1));
    pa.setBrush(Qt::NoBrush);
    pa.drawRect(maxRt);

    QPoint center(maxRt.topLeft() + QPoint(30, 30));
    QVector<QPoint> offsets;
    QVector<QSize> originalSizes;
    for (int i = 0; i < m_scrns.size(); ++i) {
        QPoint offset = m_scrns.at(i)->geometry().center() - center;
        offsets.append(offset);
        originalSizes.append(m_scrns.at(i)->geometry().size());
    }

    for (int i = 0; i < m_scrns.size(); ++i) {
        QRect rt = m_scrns.at(i)->geometry();
        QSize newSize(rt.size() * scaleFactor);
        QPoint newCenter = center + offsets.at(i) * scaleFactor;
        QPoint newTopLeft = newCenter - QPoint(newSize.width() / 2, newSize.height() / 2);
        QRect newRect(newTopLeft, newSize);

        // 绘画显示器矩形
        pa.setPen(QPen(Qt::black, 1));
        pa.drawRect(newRect);

        // 绘画高的文字
        pa.setPen(QPen(Qt::red, 1));
        const int& space = 5;
        QString heightText = QString::number(originalSizes.at(i).height());
        pa.save();
        pa.translate(newRect.topLeft() + QPoint(-space, newRect.height() / 2));
        pa.rotate(-90);
        pa.drawText(QPoint(-10, 0), heightText);
        pa.restore();

        // 绘画宽的文字
        QString widthText = QString::number(originalSizes.at(i).width());
        pa.drawText(QPoint(newRect.center().x() - 10, newRect.top() - space), widthText);

        // 绘画序号的文字
        QString text = QString("序号: %1").arg(i);
        pa.drawText(newRect.bottomLeft() + QPoint(5, -5), text);

        // 绘画标记主屏幕
        if (m_scrns.at(i) == m_priScrn)
            pa.drawText(newRect.topLeft() + QPoint(5, 15), QObject::tr("Primary"));
    }

    pa.setPen(QPen(Qt::black, 4));
    pa.drawPoint(center);

    pa.setPen(QPen(Qt::green, 2));
    pa.drawRect(0, 0, clientSize.width(), clientSize.height());

    // QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    // QString filePath = desktopPath + "/monitor_image.png";
    // pixmap.save(filePath);


    return pixmap;
}

void XMonitorLabel::paintEvent(QPaintEvent *e)
{
    QPainter pa(this);

    // 缩小比例
    qreal scaleFactor = 0.08; // 缩小为原来的十分之一
    const int textMargin = 40;
    const QSize size = m_priScrn->virtualGeometry().size() * scaleFactor + QSize(textMargin, textMargin);
    QRect maxRt(QPoint(10, 10), size);
    pa.setPen(QPen(Qt::blue, 1));
    pa.setBrush(Qt::NoBrush);
    pa.drawRect(maxRt);

    // 获取窗口中心点坐标
    // QPoint center(width() / 2, height() / 2);
    QPoint center(maxRt.topLeft() + QPoint(30, 30));


    // 计算每个矩形的相对于窗口中心点的偏移量
    QVector<QPoint> offsets;
    QVector<QSize> originalSizes; // 用于存储未缩放前的原始大小
    for (int i = 0; i < m_scrns.size(); ++i) {
        QPoint offset = m_scrns.at(i)->geometry().center() - center;
        offsets.append(offset);
        originalSizes.append(m_scrns.at(i)->geometry().size());
    }

    for (int i = 0; i < m_scrns.size(); ++i) {
        QRect rt = m_scrns.at(i)->geometry();

        // 计算新的矩形大小
        QSize newSize(rt.size() * scaleFactor);

        // 计算新的矩形位置，使得中心点位于窗口的中心
        QPoint newCenter = center + offsets.at(i) * scaleFactor;
        QPoint newTopLeft = newCenter - QPoint(newSize.width() / 2, newSize.height() / 2);
        QRect newRect(newTopLeft, newSize);

        pa.setPen(QPen(Qt::black, 1));
        // 绘制矩形
        pa.drawRect(newRect);

        pa.setPen(QPen(Qt::red, 1));
        const int& space = 5;
        // 绘制高度文本
        QString heightText = QString::number(originalSizes.at(i).height());
        pa.save();
        pa.translate(newRect.topLeft() + QPoint(-space, newRect.height() / 2)); // 将坐标移动到左侧中间
        pa.rotate(-90); // 旋转90度
        pa.drawText(QPoint( - 10, 0), heightText);
        pa.restore();

        // 绘制宽度文本
        QString widthText = QString::number(originalSizes.at(i).width());
        pa.drawText(QPoint(newRect.center().x() - 10, newRect.top() - space), widthText);

        // 绘制序号
        QString text = QString("序号: %1").arg(i);
        pa.drawText(newRect.bottomLeft() + QPoint(5, -5), text);

        if (m_scrns.at(i) == m_priScrn)
            pa.drawText(newRect.topLeft() + QPoint(5, 15), tr("Primary"));
    }


    // 绘制黑色原点
    pa.setPen(QPen(Qt::black, 4));
    pa.drawPoint(center);

    pa.drawRect(rect());
}




double XMonitorLabel::scaling(const QScreen *screen) const
{
    double scale = 0;
#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)          // or defined(Q_WS_WIN) || defined(Q_WS_X11)
    scale = screen->logicalDotsPerInch() / 96.0;
    if (scale < 1.25)
        return 1;
    else if (1.25 <= scale && scale < 1.5)
        return 1.25;
    else if (1.5 <= scale && scale < 1.75)
        return 1.5;
    else if (1.75 <= scale && scale < 2)
        return 1.75;
    else if (2 <= scale && scale < 2.25)
        return 2;
    else if (2.25 <= scale && scale < 2.5)
        return 2.25;
    else if (2.5 <= scale && scale < 3)
        return 2.5;
    else if (3 <= scale && scale < 3.5)
        return 3;
    else if (3.5 <= scale && scale < 4)
        return 3.5;
    else
        return scale;
#elif  defined(Q_OS_MAC)
    scale = screen->logicalDotsPerInch() / 72.0;
    return scale;
#endif
}
