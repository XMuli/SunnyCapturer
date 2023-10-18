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
#include "../../../data/configmanager.h"
#include "../toolbar_level/paintbarhelper.h"


PinWidget::PinWidget(const QPixmap &pixmap, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PinWidget)
    , m_menu(new QMenu(this))
    , m_timer(new QTimer(this))
    , m_pixmap(pixmap)
    , m_shadowEffect(new QGraphicsDropShadowEffect(this))
{
    ui->setupUi(this);
    initUI();
}

PinWidget::~PinWidget()
{
    delete ui;
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
    auto aColse = m_menu->addAction(tr("Close"), this, &PinWidget::close, QKeySequence(Qt::CTRL + Qt::Key_W));


    connect(aCopy, &QAction::triggered, this, &PinWidget::onCopy);
    connect(aSave, &QAction::triggered, this, &PinWidget::onSave);


//    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close())); // TODO 2022.07.29: 替换为 Qt5 形式
    new QShortcut(Qt::Key_Escape, this, SLOT(close()));
    // 使用单值捕获，不然有问题： https://zhuanlan.zhihu.com/p/346991724
    //    connect(aShadow, &QAction::triggered, this, [&, aShadow](bool checked) { aShadow->setChecked(checked); });
}

void PinWidget::setScaledPixmapToLabel(const QSize &newSize, const qreal scale, const bool expanding)
{
    QPixmap scaledPixmap;
    const auto aspectRatio = expanding ? Qt::KeepAspectRatioByExpanding : Qt::KeepAspectRatio;

    scaledPixmap = m_pixmap.scaled(newSize * scale, aspectRatio, Qt::SmoothTransformation); // Qt::FastTransformation
    scaledPixmap.setDevicePixelRatio(scale);
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


    const int step = degrees.manhattanLength() * direction;
    const int newWidth = qBound(50, ui->label->width() + step, maximumWidth());
    const int newHeight = qBound(50, ui->label->height() + step, maximumHeight());

    const QSize newSize(newWidth, newHeight);
    const qreal scale = qApp->devicePixelRatio();
    const bool isExpanding = direction > 0;
    setScaledPixmapToLabel(newSize, scale, isExpanding);

//    changeShadowColor();
    adjustSize();                                         // Reflect scaling to the label
    e->accept();
}

void PinWidget::contextMenuEvent(QContextMenuEvent *e)
{
    m_menu->exec(e->globalPos());
}
