// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "pinwidget.h"
#include "../../screen/drawhelper.h"
#include "../../xglobal.h"
#include <QMenu>
#include <QColor>
#include <QPoint>
#include <QLabel>
#include <QDebug>
#include <QPixmap>
#include <QAction>
#include <QDateTime>
#include <QShortcut>
#include <QBoxLayout>
#include <QClipboard>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <QApplication>
#include <QGuiApplication>
#include <QContextMenuEvent>
#include <QGraphicsDropShadowEffect>
#include "QActionGroup"
#include "../../screen/datamaid.h"

//QPoint m_p;   // 窗口的左上角
//QPoint m_p1;
//QPoint m_p2;
//
//QPixmap m_pixmap;
//QLabel* m_label;
//QGraphicsDropShadowEffect* m_shadowEffect;
//
//QMenu* m_menu;

PinWidget::PinWidget(const QPixmap &pixmap, const QRect &geometry, QWidget *parent)
    : QWidget(parent)
    , m_p(0, 0)
    , m_p1(0, 0)
    , m_p2(0, 0)
    , m_pixmap(pixmap)
    , m_label(new QLabel(this))
    , m_shadowEffect(new QGraphicsDropShadowEffect(this))
    , m_menu(new QMenu(this))
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);           // Otherwise it is a black background
    setAttribute(Qt::WA_DeleteOnClose);

    const int maxWidth = DATAMAID->paraValue("maxSize").toInt();
    setMaximumSize(maxWidth, maxWidth);
    setWindowOpacity(DATAMAID->paraValue(tpOpacity).toInt() / 100.0);

    auto vLayout = new QVBoxLayout(this);
    const int margin = PW_MARGIN;
    vLayout->setContentsMargins(margin, margin, margin, margin);

    m_shadowEffect->setColor(DATAMAID->paraValue("borderColor").toString());
    m_shadowEffect->setBlurRadius(2 * margin);
    m_shadowEffect->setOffset(0, 0);
    setGraphicsEffect(m_shadowEffect);

    m_label->setPixmap(m_pixmap);
    vLayout->addWidget(m_label);

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close())); // TODO 2022.07.29: 替换为 Qt5 形式
    new QShortcut(Qt::Key_Escape, this, SLOT(close()));

    qreal devicePixelRatio = 1;
#if defined(Q_OS_MACOS)
    devicePixelRatio = 2;
#endif
    const int m = margin * devicePixelRatio;
    QRect adjusted_pos = geometry + QMargins(m, m, m, m);
    setGeometry(adjusted_pos);
    adjustSize();

    connect(DATAMAID, &DataMaid::sigWindowShadow, this, &PinWidget::onWindowShadow);
    connect(DATAMAID, &DataMaid::sigOpacity, this, &PinWidget::onOpacity);
    connect(DATAMAID, &DataMaid::sigMaxSize, this, &PinWidget::onMaxSize);

    initUI();
}

void PinWidget::initUI()
{
    auto aCopy = m_menu->addAction(tr("Copy"));
    auto aSave = m_menu->addAction(tr("Save"));
    m_menu->addSeparator();
    auto aShadow = new QAction(); // m_menu->addAction(tr("Shadow"));

    
    auto aOpicaty = new QMenu(tr("Opicaty"), this);
    auto opicatyGroup = new QActionGroup(this);
    opicatyGroup->setExclusive(true);
    for (int i = 10; i >= 1; --i) {
        auto act = aOpicaty->addAction(tr("%1%").arg(i * 10));
        act->setCheckable(true);

        if (DATAMAID->paraValue(tpOpacity).toInt() == i * 10)
            act->setChecked(true);
        else
            act->setChecked(false);

        opicatyGroup->addAction(act);
        connect(act, &QAction::triggered, this, [&, i]() { onOpacity(i * 10); });
    }

    m_menu->addMenu(aOpicaty);
    m_menu->addSeparator();
    //auto aGrop = m_menu->addAction(tr("Move to grop"));
    m_menu->addSeparator();
    //auto aDel = m_menu->addAction(tr("Delete"), this, &PinWidget::close, QKeySequence(Qt::Key_Delete));   // 但是按键是不生效的，很奇怪
    auto aColse = m_menu->addAction(tr("Close"), this, &PinWidget::close, QKeySequence(Qt::CTRL + Qt::Key_W));

    aShadow->setCheckable(true);
    aShadow->setChecked(true);
    
    //void changed();
    //void triggered(bool checked = false);  // 可以不用设置 setCheckable(true) 而直接触发
    //void hovered();
    //void toggled(bool);

    connect(aCopy, &QAction::triggered, this, &PinWidget::onCopy);
    connect(aSave, &QAction::triggered, this, &PinWidget::onSave);

    // 使用单值捕获，不然有问题： https://zhuanlan.zhihu.com/p/346991724
    connect(aShadow, &QAction::triggered, this, [&, aShadow](bool checked) { aShadow->setChecked(checked); });
}

void PinWidget::setScaledPixmapToLabel(const QSize &newSize, const qreal scale, const bool expanding)
{
    QPixmap scaledPixmap;
    const auto aspectRatio = expanding ? Qt::KeepAspectRatioByExpanding : Qt::KeepAspectRatio;

    scaledPixmap = m_pixmap.scaled(newSize * scale, aspectRatio, Qt::SmoothTransformation); // Qt::FastTransformation
    scaledPixmap.setDevicePixelRatio(scale);
    m_label->setPixmap(scaledPixmap);
}

void PinWidget::createShadowEffect()
{
    //if (!m_shadowEffect)
    //    m_shadowEffect = new QGraphicsDropShadowEffect(this);

    //const int margin = PW_MARGIN;
    //m_shadowEffect->setColor(XHelper::instance().borderColor());
    //m_shadowEffect->setBlurRadius(2 * margin);
    //m_shadowEffect->setOffset(0, 0);
    //setGraphicsEffect(m_shadowEffect);
}

void PinWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton) {
        m_p1 = e->globalPos();
        m_p2 = m_p1;
        m_p = pos();
    }
}

void PinWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton) {
        m_p2 = e->globalPos();
        move(m_p + m_p2 - m_p1);
    }
}

void PinWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton) {
        m_p2 = e->globalPos();
        m_p1 = m_p2;
    }
}

void PinWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton)
        close();
}

void PinWidget::wheelEvent(QWheelEvent *event)
{
    const QPoint degrees = event->angleDelta() / 8;
    const int direction = degrees.y() > 0 ? 1 : -1;       // zooming in or out

    const int step = degrees.manhattanLength() * direction;
    const int newWidth = qBound(50, m_label->width() + step, maximumWidth());
    const int newHeight = qBound(50, m_label->height() + step, maximumHeight());

    const QSize newSize(newWidth, newHeight);
    const qreal scale = qApp->devicePixelRatio();
    const bool isExpanding = direction > 0;
    setScaledPixmapToLabel(newSize, scale, isExpanding);

    adjustSize();                                         // Reflect scaling to the label
    event->accept();
}

void PinWidget::contextMenuEvent(QContextMenuEvent* event)
{
    m_menu->exec(event->globalPos());
}

void PinWidget::onWindowShadow(bool enable)
{
    //if (enable) {
    //    if (!m_shadowEffect)
    //        m_shadowEffect = new QGraphicsDropShadowEffect(this);

    //    // TODO 2022.08.03: 重新加载后有点问题。
    //    const int margin = PW_MARGIN;
    //    m_shadowEffect->setColor(XHelper::instance().borderColor());
    //    m_shadowEffect->setBlurRadius(2 * margin);
    //    ////m_shadowEffect->setOffset(0, 0);
    //    setGraphicsEffect(m_shadowEffect);

    //} else {
    //    setGraphicsEffect(nullptr);  // 旧的 shadowEffect 会被 qt 底层删除，见此函数实现，所以每次都会新建
    //}
}

void PinWidget::onOpacity(int opacity)
{
    setWindowOpacity(opacity / 100.0);
}

void PinWidget::onMaxSize(double val)
{
    setMaximumSize(val, val);
}

void PinWidget::onCopy()
{
    if (!m_pixmap.isNull())
        QApplication::clipboard()->setPixmap(m_pixmap);
}

void PinWidget::onSave()
{
    if (m_pixmap.isNull())
        return;

    QString fileter(tr("Image Files(*.png);;Image Files(*.jpg);;All Files(*.*)"));
    QString fileNmae = QFileDialog::getSaveFileName(this, tr("Save Files"), "picshot_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".png", fileter);
    
    QTime startTime = QTime::currentTime();
    m_pixmap.save(fileNmae);
    QTime stopTime = QTime::currentTime();
    int elapsed = startTime.msecsTo(stopTime);
    qDebug() << "save m_pixmap tim =" << elapsed << "ms" << m_pixmap.size();
}

