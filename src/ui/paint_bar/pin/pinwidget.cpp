#include "pinwidget.h"
#include <QAction>
#include <QActionGroup>
#include <QClipboard>
#include <QApplication>
#include <QGuiApplication>
#include <QString>
#include <QFileDialog>
#include <QDateTime>
#include <QDebug>
#include <QSize>
#include <QGraphicsView>

PinWidget::PinWidget(const QPixmap &pixmap, QWidget *parent)
    : QWidget(parent)
    , m_pixmap(pixmap)
    , m_menu(new QMenu(this))
{
    initUI();

}

void PinWidget::initUI()
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
//    setAttribute(Qt::WA_TranslucentBackground);           // Otherwise it is a black background
    setAttribute(Qt::WA_DeleteOnClose);

    initMenu();
}

void PinWidget::initMenu()
{
    const auto aCopy = m_menu->addAction(tr("Copy image"));
    const auto aSave = m_menu->addAction(tr("Save image as .."));
    m_menu->addSeparator();

    const auto aOpicaty = new QMenu(tr("Opicaty"), this);
    const auto group = new QActionGroup(this);
    group->setExclusive(true);
    for (int i = 10; i >= 1; --i) {
        auto act = aOpicaty->addAction(tr("%1%").arg(i * 10));
        act->setCheckable(true);
        act->setChecked(i == 10 ? true : false);

        group->addAction(act);
        connect(act, &QAction::triggered, this, [&, i]() { onOpacity(i * 10); });
    }

    m_menu->addMenu(aOpicaty);
    m_menu->addSeparator();
    m_menu->addSeparator();
    auto aColse = m_menu->addAction(tr("Close")/*, this, &PinWidget::close, QKeySequence(Qt::CTRL + Qt::Key_W)*/);


    connect(aCopy, &QAction::triggered, this, &PinWidget::onCopy);
    connect(aSave, &QAction::triggered, this, &PinWidget::onSave);
    connect(aColse, &QAction::triggered, this, &PinWidget::onColse);

    // 使用单值捕获，不然有问题： https://zhuanlan.zhihu.com/p/346991724
    //    connect(aShadow, &QAction::triggered, this, [&, aShadow](bool checked) { aShadow->setChecked(checked); });
}

void PinWidget::onCopy()
{
    if (m_pixmap.isNull()) return;
    qGuiApp->clipboard()->setPixmap(m_pixmap);
}

void PinWidget::onSave()
{
    if (m_pixmap.isNull()) return;

    QString fileter(tr("Image Files(*.png);;Image Files(*.jpg);;All Files(*.*)"));
    QString fileNmae = QFileDialog::getSaveFileName(this, tr("Save Files"), "Sunny_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".png", fileter);

    QTime startTime = QTime::currentTime();
    m_pixmap.save(fileNmae);
    QTime stopTime = QTime::currentTime();
    int elapsed = startTime.msecsTo(stopTime);
    qDebug() << "save m_pixmap tim =" << elapsed << "ms" << m_pixmap.size();
}

void PinWidget::onColse()
{
    close();
}

void PinWidget::onOpacity(const int &opacity)
{
    setWindowOpacity(opacity / 100.0);
}

void PinWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() != Qt::LeftButton) return;
    m_node.p1 = e->globalPos();
    m_node.p2 = e->globalPos();
    m_node.p3 = e->globalPos();
    m_node.pt = pos();
}

void PinWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->buttons() != Qt::LeftButton) return;
    m_node.p2 = e->globalPos();
    m_node.p3 = e->globalPos();
    move(m_node.pt + m_node.p2 - m_node.p1);
}

void PinWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() != Qt::LeftButton) return;
    m_node.p2 = e->globalPos();
    m_node.p3 = e->globalPos();

    move(m_node.pt + m_node.p3 - m_node.p1);
}

void PinWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->buttons() != Qt::LeftButton) return;
    close();
}

void PinWidget::wheelEvent(QWheelEvent *e)
{
    const QPoint degrees = e->angleDelta() / 8;
    const int direction = degrees.y() > 0 ? 1 : -1;       // zooming in or out
    qreal scaleFactor = 1.1; // 10% 的缩放因子

    // 如果 degrees 大于0，表示向上滚动，放大窗口；否则，缩小窗口
    if (direction < 0) scaleFactor = 1.0 / scaleFactor; // 如果向上滚动，将缩放因子取倒数

    QSize currentSize = size();
    int newWidth = currentSize.width() * scaleFactor;
    int newHeight = currentSize.height() * scaleFactor;
    resize(newWidth, newHeight);

    QWidget::wheelEvent(e);
}

void PinWidget::contextMenuEvent(QContextMenuEvent *e)
{
    m_menu->exec(e->globalPos());
}

void PinWidget::paintEvent(QPaintEvent *e)
{
    QPainter pa(this);

    if (m_pixmap.isNull()) return;
    pa.drawPixmap(rect(), m_pixmap);
}

QPixmap PinWidget::pixmap() const
{
    return m_pixmap;
}

void PinWidget::setPixmap(const QPixmap &newPixmap)
{
    m_pixmap = newPixmap;
}
