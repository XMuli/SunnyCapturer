// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "pinwidget.h"
#include "ui_pinwidget.h"
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
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QShortcut>
#include <QKeySequence>
#include <QStringList>
#include <QFont>
#include <QLabel>
#include <QTimer>
#include "../../../data/configmanager.h"
#include "../paintbarhelper.h"
#include "../../data/configjson.h"


PinWidget::PinWidget(const QPixmap &pixmap, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PinWidget)
    , m_zoom(1)  // 起始 100% 比例
    , m_menu(new QMenu(this))
    , m_timer(new QTimer(this))
    , m_pixmap(pixmap)
    , m_labZoom(new QLabel(nullptr))
    , m_timerLabZoom(new QTimer(this))
    , m_shadowEffect(new QGraphicsDropShadowEffect(this))
{
    ui->setupUi(this);
    initUI();
}

PinWidget::~PinWidget()
{
    delete ui;
}

const int PinWidget::layoutMargin() const
{
    return ui->layout->margin();
}

void PinWidget::initUI()
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);              // 屏蔽后，可以看到实际的黑色边框的大小
    setAttribute(Qt::WA_DeleteOnClose);
//    setStyleSheet("background: transparent;");

    ui->label->setPixmap(m_pixmap);
    m_shadowEffect->setColor(highlightColor(true));
    m_shadowEffect->setBlurRadius(ui->layout->margin() * 2);  // 对应半径
    m_shadowEffect->setOffset(0, 0);
    setGraphicsEffect(m_shadowEffect);

    initMenu();
//    m_timer->start(500);
//    connect(m_timer, SIGNAL(timeout()), this, SLOT(changeShadowColor()));


    // 私有布局，控件叠加悬浮显示
    m_labZoom->setParent(ui->label);
    m_labZoom->setStyleSheet("QLabel { background-color: white; border: 1px solid black; }");
    m_labZoom->raise();
    m_labZoom->hide();
    m_labZoom->move(5, 5);

    // QVBoxLayout* vLayout = new QVBoxLayout(ui->label);
    // vLayout->addWidget(m_labZoom);
    // vLayout->setAlignment(m_labZoom, Qt::AlignTop);

    connect(m_timerLabZoom, &QTimer::timeout, [this]() { m_labZoom->hide();});
}

void PinWidget::initMenu()
{
    QStringList list = CONF_MANAGE.property("XGeneral_font").toString().split(",");
    if (list.size() < 2) {
        list .clear();

#if defined(Q_OS_WIN)
        list << "Microsoft YaHei" << "9";
#elif defined(Q_OS_LINUX)
        list << "WenQuanYi Micro Hei" << "9";
#elif defined(Q_OS_MAC)
        list << "PingFang SC" << "11";
#endif

    }
    const QFont font(list.at(0), list.at(1).toInt());
    setFont(font);
    m_menu->setFont(font);

    const auto& aCopy = m_menu->addAction(tr("Copy"));
    const auto& aSave = m_menu->addAction(tr("Save"));
    m_menu->addSeparator();
    const auto& aShadow = m_menu->addAction(tr("Shadow"));
    aShadow->setCheckable(true);
    aShadow->setChecked(true);

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
    m_menu->addAction(tr("Close"), this, &PinWidget::close, QKeySequence(Qt::CTRL + Qt::Key_W));

    connect(aCopy, &QAction::triggered, this, &PinWidget::onCopy);
    connect(aSave, &QAction::triggered, this, &PinWidget::onSave);
    connect(aShadow, &QAction::triggered, this, &PinWidget::onShadow);

    new QShortcut(Qt::Key_Escape, this, SLOT(close()));
//    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close())); // TODO 2022.07.29: 替换为 Qt5 形式
    // 使用单值捕获，不然有问题： https://zhuanlan.zhihu.com/p/346991724
    //    connect(aShadow, &QAction::triggered, this, [&, aShadow](bool checked) { aShadow->setChecked(checked); });
}

void PinWidget::setScaledPixmapToLabel(const QSize &newSize, const qreal dpr)
{
    QPixmap scaledPixmap;
    scaledPixmap = m_pixmap.scaled(newSize * dpr, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Qt::FastTransformation
    scaledPixmap.setDevicePixelRatio(dpr);
    ui->label->setPixmap(scaledPixmap);
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

void PinWidget::onShadow(bool checked)
{
    if (checked) {
        if (!m_shadowEffect) m_shadowEffect = new QGraphicsDropShadowEffect(this);

        m_shadowEffect->setColor(highlightColor(true));
        m_shadowEffect->setBlurRadius(ui->layout->margin() * 2);  // 对应半径
        m_shadowEffect->setOffset(0, 0);
        setGraphicsEffect(m_shadowEffect);   // TODO 2022.08.03: 重新加载后有点问题。

        // resize(size() - QSize(1, 1));
        // adjustSize();  // 必须手动实现才行
        // update();
    } else {
        setGraphicsEffect(nullptr);          // 旧的 shadowEffect 会被 qt 底层删除，见此函数实现，所以每次都会新建
    }
}

void PinWidget::onOpacity(const int &opacity)
{
    setWindowOpacity(opacity / 100.0);
}

void PinWidget::changeShadowColor()
{
    // 生成随机颜色
    int red = qrand() % 256;
    int green = qrand() % 256;
    int blue = qrand() % 256;

    QColor newColor =  QColor(red, green, blue);
    m_shadowEffect->setColor(newColor);

    adjustSize();
    update();

    //    QTimer::singleShot(50, [this](){ this->setAttribute(Qt::WA_TranslucentBackground, true); update();});
}

void PinWidget::updateZoomLabel()
{
    m_labZoom->setText(QString(tr("Zoom: %1%")).arg(m_zoom * 100));
    m_labZoom->show();
    m_labZoom->adjustSize();      // 自动调整大小以适应文本
    m_timerLabZoom->stop();       // 取消之前的定时器
    m_timerLabZoom->start(2000);  // 创建一个新的定时器，并在时间到达时隐藏标签
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
    static QSize origSize = ui->label->size();
    QSize newSize = origSize;
    const QPoint degrees = e->angleDelta() / 8;
    const int direction = degrees.y() > 0 ? 1 : -1;       // zooming in or out
    m_zoom += direction * 0.1;

    newSize *= m_zoom;
    const int& min = 50;
    const int& max = CJ_GET("pin.maximum_size").get<int>();
    newSize.setWidth(qBound(min, newSize.width(), max));
    newSize.setHeight(qBound(min, newSize.height(), max));

    setScaledPixmapToLabel(newSize, qApp->devicePixelRatio());

    adjustSize();                                              // Reflect scaling to the label
    e->accept();
    QWidget::wheelEvent(e);

    updateZoomLabel();
}

void PinWidget::contextMenuEvent(QContextMenuEvent *e)
{
    m_menu->exec(e->globalPos());
}
