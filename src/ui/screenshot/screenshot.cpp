#include "screenshot.h"
#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QDebug>
#include <QIcon>
#include "windowsrect.h"

WindowsRect g_windowsRect;

ScreenShot::ScreenShot(QWidget *parent)
    : QWidget(parent)
    , m_primaryScreen(qGuiApp->primaryScreen())
    , m_screens(qGuiApp->screens())
    , m_origPix()
    , m_vdRect()
    , m_bAutoDetectRect(true)
    , m_actionType(ActionType::AT_wait)
{
    initUI();

    setMouseTracking(true);
    if (m_actionType == ActionType::AT_wait) {
        if (m_node.pickedRect.isEmpty())
            m_actionType = m_bAutoDetectRect ? ActionType::AT_picking_detection_windows_rect : ActionType::AT_picking_custom_rect;

        if (m_actionType == ActionType::AT_picking_detection_windows_rect) {
            #if defined(Q_OS_WIN)
                g_windowsRect.startWindowsHook();
            #endif
        }

    }
}


ScreenShot::~ScreenShot()
{
    preDestruction();
}

void ScreenShot::capture()
{
    originalPixmap();
    show();
}

void ScreenShot::initUI()
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_vdRect = m_primaryScreen->virtualGeometry();
    monitorsInfo();

#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)
//    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags());  // | Qt::WindowStaysOnTopHint

    // linux下鼠标穿透要放在后面两行代码的全前面，否则无效(但是鼠标穿透了会导致一些奇怪的问题，如窗口显示不全，所以这里不使用)
    // windows下如果不设置鼠标穿透则只能捕获到当前窗口
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    // setAttribute(Qt::WA_TranslucentBackground);                         // 背景透明
    setWindowOpacity(0.4);
#ifdef HALF_SCRN_DEV
    m_vdRect = currentScreen()->geometry();
    setWindowFlag(Qt::WindowStaysOnTopHint, false);
    if (m_screens.size() == 1) m_vdRect.setWidth(m_vdRect.width() / 2);
#endif

    resize(m_vdRect.size());
    move(m_vdRect.topLeft());
#else // Q_OS_MAC
    QRect geom = curScrn(QCursor::pos())->geometry();
    m_captureScrnRt = geom;
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);   // 窗口置顶 + 隐藏标题栏
#ifdef HALF_SCRN_DEV
    if (m_scrns.size() == 1)
        geom.setWidth(geom.width() / 2);
#endif

    setFixedSize(geom.size());   // resize() cannot get the desired effect
    move(geom.topLeft());
    qDebug() << "#2-->" << geom << "   " << this->rect();
#endif
}

void ScreenShot::drawShadowOverlay(const QRect &fullRect, const QRect &pickedRect, QPainter& pa) const
{
    QPainterPath path;
    path.addRect(fullRect);
    path.addRect(pickedRect);
    path.setFillRule(Qt::OddEvenFill);
    pa.save();
    pa.setPen(Qt::NoPen);
    pa.setBrush(QColor(0, 0, 0, 0.4 * 255));
    pa.drawPath(path);
    pa.restore();
}

void ScreenShot::drawBorderFlipped(QPainter &pa, const QRect &rt) const
{
    pa.save();
    pa.setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    const int addFixWidth = 6;
    pen.setWidth(3 + addFixWidth);
    pen.setColor(QColor(14, 112, 255));
    pa.setPen(pen);
    pa.setBrush(Qt::NoBrush);

    int x1 = rt.left();
    int y1 = rt.top();
    int x2 = rt.right();
    int y2 = rt.bottom();

    const int penWidth = pen.width();
    const int cornerLength = 100;       // 四角周辅助的 ∟ 的长度
    const int doubleCornerLength = 2 * cornerLength;

    if (rt.width() >= doubleCornerLength && rt.height() >= doubleCornerLength) {
        // hor 且补齐交叉角落的空缺的那一块
        QLine l1(QPoint(x1 - penWidth / 2, y1), QPoint(x1 + cornerLength, y1));
        QLine l2(QPoint(x1 - penWidth / 2, y2), QPoint(x1 + cornerLength, y2));
        QLine l3(QPoint(x2 + penWidth / 2, y1), QPoint(x2 - cornerLength, y1));
        QLine l4(QPoint(x2 + penWidth / 2, y2), QPoint(x2 - cornerLength, y2));

        // ver
        QLine l5(QPoint(x1, y1), QPoint(x1, y1 + cornerLength));
        QLine l6(QPoint(x1, y2), QPoint(x1, y2 - cornerLength));
        QLine l7(QPoint(x2, y1), QPoint(x2, y1 + cornerLength));
        QLine l8(QPoint(x2, y2), QPoint(x2, y2 - cornerLength));

        pa.drawLine(l1.translated(QPoint(0, -penWidth / 2)));
        pa.drawLine(l2.translated(QPoint(0, penWidth / 2)));
        pa.drawLine(l3.translated(QPoint(0, -penWidth / 2)));
        pa.drawLine(l4.translated(QPoint(0, penWidth / 2)));
        pa.drawLine(l5.translated(QPoint(-penWidth / 2, 0)));
        pa.drawLine(l6.translated(QPoint(-penWidth / 2, 0)));
        pa.drawLine(l7.translated(QPoint(penWidth / 2, 0)));
        pa.drawLine(l8.translated(QPoint(penWidth / 2, 0)));
    }

    pen.setWidth(3);
    pa.setPen(pen);
    pa.drawRect(rt);
    pa.restore();
}

void ScreenShot::drawBorderMacOS(QPainter &pa, const QRect &rt, int num) const
{
    pa.save();
    pa.setRenderHint(QPainter::Antialiasing, false);
    pa.setBrush(Qt::NoBrush);
    QPen penWhite(QColor(255, 255, 255, 1 * 255), 1);
    penWhite.setStyle(Qt::CustomDashLine);
    penWhite.setDashOffset(0);
    penWhite.setDashPattern(QVector<qreal>() << 4 << 4 );
    penWhite.setCapStyle(Qt::FlatCap);
    pa.setPen(penWhite);
    pa.drawLine(QPoint(rt.left(), rt.top()), QPoint(rt.right(), rt.top()));
    pa.drawLine(QPoint(rt.left(), rt.top()), QPoint(rt.left(), rt.bottom()));
    pa.drawLine(QPoint(rt.left(), rt.bottom()), QPoint(rt.right(), rt.bottom()));
    pa.drawLine(QPoint(rt.right(), rt.top()), QPoint(rt.right(), rt.bottom()));

    QPen penBlack(penWhite);
    penBlack.setColor(QColor(0, 0, 0, 1 * 255));
    penBlack.setDashOffset(4);
    pa.setPen(penBlack);
    pa.drawLine(QPoint(rt.left(), rt.top()), QPoint(rt.right(), rt.top()));
    pa.drawLine(QPoint(rt.left(), rt.top()), QPoint(rt.left(), rt.bottom()));
    pa.drawLine(QPoint(rt.left(), rt.bottom()), QPoint(rt.right(), rt.bottom()));
    pa.drawLine(QPoint(rt.right(), rt.top()), QPoint(rt.right(), rt.bottom()));

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    rt.getCoords(&x1, &y1, &x2, &y2);

    QVector<QPoint> ver = { QPoint(x1, y1), QPoint(x2, y1), QPoint(x1, y2), QPoint(x2, y2),
                            QPoint((x1 + x2) / 2.0, y1),
                            QPoint((x1 + x2) / 2.0, y2),
                            QPoint(x1, (y1 + y2) / 2.0),
                            QPoint(x2, (y1 + y2) / 2.0) };

    pa.setPen(QPen(Qt::white, 1.5));
    pa.setBrush(QColor(146, 146, 146, 1 * 255));
    QPoint offsetPos(6, 6);  // 边框上标记点的半径
    pa.setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i < num; ++i)
        pa.drawEllipse(ver[i], offsetPos.x(), offsetPos.y());

    pa.restore();
}

void ScreenShot::drawBorderDDE(QPainter &pa, const QRect &rt, int num) const
{
    pa.save();
    pa.setPen(QPen(QColor(14, 112, 255), 3));
    pa.setBrush(Qt::NoBrush);
    pa.drawRect(rt);

    pa.setPen(Qt::NoPen);

    QIcon icon(":/resources/screenshot_ui/screenshot/board_circle.svg");
    const int radius = 6;
    QPixmap pixmap = icon.pixmap(QSize(radius, radius) * 4);
    pixmap.setDevicePixelRatio(devicePixelRatio());

    QPoint offsetPos(radius * 2, radius * 2 ) ;
    pa.drawPixmap(rt.topLeft() - offsetPos, pixmap);
    pa.drawPixmap(rt.topRight() - offsetPos, pixmap);
    pa.drawPixmap(rt.bottomLeft() - offsetPos, pixmap);
    pa.drawPixmap(rt.bottomRight() - offsetPos, pixmap);

    if (num == 8) {
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;

        rt.getCoords(&x1, &y1, &x2, &y2);
        pa.drawPixmap(QPoint((x1 + x2) / 2, y1) - offsetPos, pixmap);
        pa.drawPixmap(QPoint((x1 + x2) / 2, y2) - offsetPos, pixmap);
        pa.drawPixmap(QPoint(x1, (y1 + y2) / 2) - offsetPos, pixmap);
        pa.drawPixmap(QPoint(x2, (y1 + y2) / 2) - offsetPos, pixmap);
    }
    pa.restore();
}

void ScreenShot::originalPixmap()
{
    if (m_origPix.isNull()) {m_origPix =  m_primaryScreen->grabWindow(qApp->desktop()->winId(), m_vdRect.x(), m_vdRect.y(), m_vdRect.width(), m_vdRect.height());
        qDebug() << "originalPixmap()， &m_origPix:" << &m_origPix << "m_origPix:" << m_origPix;
    }
}

QScreen *ScreenShot::currentScreen(const QPoint &pos) const
{
    QScreen* scrn = qGuiApp->screenAt(pos);

#if defined(Q_OS_MACOS)
    // In macos, mouse position at the bottom or right edge of the screen will crash
    if (!scrn && (pos.x() >= m_captureScrnRt.right() || pos.y() >= m_captureScrnRt.bottom()))
        scrn = qGuiApp->screenAt(m_captureScrnRt.bottomRight() - QPoint(1, 1));
#endif

    //if (!scrn) curScrn = qGuiApp->primaryScreen();

    if (!scrn) qDebug() << "Gets that the current screen is empty";

    return scrn;
}

void ScreenShot::preDestruction()
{
    if (!m_origPix.isNull()) {
        m_origPix = QPixmap();
    }
}

void ScreenShot::monitorsInfo() const
{
    qInfo() << "---------------QApplication::desktop() Info BEGIN----------------";
    qInfo() << "所有可用区域 m_vdRect:" << m_vdRect << Qt::endl
        << "主屏可用区域 m_primaryScreen->geometry():" << m_primaryScreen->geometry()
        << "是否开启虚拟桌面 isVirtualDesktop:" << (m_primaryScreen->virtualSiblings().size() > 1 ? true : false) << Qt::endl;
    qInfo() << "---------------QApplication::desktop() Info END----------------";

    qInfo() << "---------------m_screens[] Info BEGIN----------------";
    for (const auto& it : m_screens) {
        qInfo() << "it:" << it
                 << "\n可用几何 availableGeometry:" << it->availableGeometry()
                 << "\n可用虚拟几何 availableVirtualSize:" << it->availableVirtualSize()
                 << "\n虚拟几何 virtualGeometry:" << it->virtualGeometry()
                 << "\n几何 geometry:" << it->geometry()
                 << "\n尺寸 size:" << it->size()
                 << "\n物理尺寸 physicalSize:" << it->physicalSize()
                 << "\n刷新率 refreshRate:" << it->refreshRate()
                 << "\n尺寸 size:" << it->size()
                 << "\n虚拟尺寸 virtualSize:" << it->virtualSize()
                 << "\n设备像素比 devicePixelRatio:" << it->devicePixelRatio()
                 << "\n逻辑DPI logicalDotsPerInch:" << int(it->logicalDotsPerInch()) << " DPIX:" << int(it->logicalDotsPerInchX()) << " DPIY:" << int(it->logicalDotsPerInchY())
                 << "\n物理DPI physicalDotsPerInch:" << int(it->physicalDotsPerInch()) << " DPIX:" << int(it->physicalDotsPerInchX()) << " DPIY:" << int(it->physicalDotsPerInchY());
//                 << "\n手算缩放比 getScale:" << GetScale(it)
//                 << "\n虚拟尺寸 m_captureScrnRt:" << m_captureScrnRt << "\n";
    }
    qInfo() << "---------------m_screens[] Info END----------------";
}

void ScreenShot::dealMousePressEvent(QMouseEvent *e)
{
    setMouseTracking(false);
    m_node.p1 = e->pos();
    qDebug() << "MousePressEvent, m_node.p1:" << m_node.p1;

    if (m_actionType == ActionType::AT_wait) {
        if (m_node.pickedRect.isEmpty())
            m_actionType = m_bAutoDetectRect ? ActionType::AT_picking_detection_windows_rect : ActionType::AT_picking_custom_rect;

    } else if (m_actionType == ActionType::AT_picking_custom_rect) {
        setMouseTracking(true);
    } else if (m_actionType == ActionType::AT_picking_detection_windows_rect) {
    } else if (m_actionType == ActionType::AT_select_picked_rect) {
    } else if (m_actionType == ActionType::AT_select_drawn_shape) {
    } else if (m_actionType == ActionType::AT_drawing_shap) {
    } else if (m_actionType == ActionType::AT_move_drawn_shape) {
    } else if (m_actionType == ActionType::AT_move_picked_rect) {
    } else if (m_actionType == ActionType::AT_stretch_drawn_shape) {
    } else if (m_actionType == ActionType::AT_stretch_picked_rect) {
    }
}

void ScreenShot::dealMouseReleaseEvent(QMouseEvent *e)
{
    m_node.p2 = e->pos();
    m_node.pickedRect = CaptureHelper::rectFromTowPos(m_node.p1, m_node.p2);
    qDebug() << "MouseReleaseEvent, m_node.p2:" << m_node.p2 << "m_node.pickedRect:" << m_node.pickedRect;

    if (m_actionType == ActionType::AT_wait) {
    } else if (m_actionType == ActionType::AT_picking_custom_rect) {
        m_actionType = ActionType::AT_wait;
    } else if (m_actionType == ActionType::AT_picking_detection_windows_rect) {
#if defined(Q_OS_WIN)
        g_windowsRect.endWindowsHook();
#endif
    } else if (m_actionType == ActionType::AT_select_picked_rect) {
    } else if (m_actionType == ActionType::AT_select_drawn_shape) {
    } else if (m_actionType == ActionType::AT_drawing_shap) {
    } else if (m_actionType == ActionType::AT_move_drawn_shape) {
    } else if (m_actionType == ActionType::AT_move_picked_rect) {
    } else if (m_actionType == ActionType::AT_stretch_drawn_shape) {
    } else if (m_actionType == ActionType::AT_stretch_picked_rect) {
    }

    setMouseTracking(false);
}

void ScreenShot::dealMouseMoveEvent(QMouseEvent *e)
{
    m_node.p3 = e->pos();

    if (m_actionType == ActionType::AT_wait) {

    } else if (m_actionType == ActionType::AT_picking_custom_rect) {
        m_node.pickedRect = CaptureHelper::rectFromTowPos(m_node.p1, m_node.p3);
        m_node.trackPos.emplace_back(m_node.p3);
        qDebug() << "MouseMoveEvent, m_node.p3:" << m_node.p3 << "m_node.pickedRect:" << m_node.pickedRect;
    } else if (m_actionType == ActionType::AT_picking_detection_windows_rect) {
        g_windowsRect.detectionWindowsRect();
        const auto&& t(g_windowsRect.rectNode().rect);
        const QRect detectRect(t.left, t.top, t.right - t.left, t.bottom - t.top);

        qDebug() << "detectRectL" << detectRect << "title" << g_windowsRect.rectNode().title;

        // 显示 QWidget 并获取它的 HWND
//        HWND hwnd = static_cast<HWND>(this->winId())
    } else if (m_actionType == ActionType::AT_select_picked_rect) {
    } else if (m_actionType == ActionType::AT_select_drawn_shape) {
    } else if (m_actionType == ActionType::AT_drawing_shap) {
    } else if (m_actionType == ActionType::AT_move_drawn_shape) {
    } else if (m_actionType == ActionType::AT_move_picked_rect) {
    } else if (m_actionType == ActionType::AT_stretch_drawn_shape) {
    } else if (m_actionType == ActionType::AT_stretch_picked_rect) {
    }

}

void ScreenShot::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton) return;
    dealMousePressEvent(e);
    update();
}

void ScreenShot::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton) return;
    dealMouseReleaseEvent(e);
    update();
}

void ScreenShot::mouseMoveEvent(QMouseEvent *e)
{
    dealMouseMoveEvent(e);
    update();
}

//void ScreenShot::wheelEvent(QWheelEvent *e)
//{

//}

void ScreenShot::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        qDebug() << "Key_Escape";
        preDestruction();
        close();
        e->accept();
        // hide() 和 close() 区别: https://stackoverflow.com/questions/39407564
        // 销毁再不会有问题,由单例改写为 new 形式了。排查：1. tray 有关，改用 qpushbutton 和 close即可； 2.单例有关，该市建议修改为 new 指针的比较合适
    } else {
        QWidget::keyReleaseEvent(e);
    }
}

void ScreenShot::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    const auto& pickedRect = m_node.pickedRect;
    QPainter pa(this);     // 始终从相对本窗口的坐标 (0, 0) 开始绘画
    pa.setBrush(Qt::NoBrush);
    pa.setPen(Qt::NoPen);


    if (!m_origPix.isNull()) {
        pa.drawPixmap(QPoint(0, 0), m_origPix);
    }

    drawShadowOverlay(rect(), pickedRect, pa);
//    drawBorderMacOS(pa, pickedRect);
//    drawBorderDDE(pa, pickedRect);
    drawBorderFlipped(pa, pickedRect);


    pa.save();
    pa.setPen(QPen(Qt::green, 2));
    const int tTextX = 0;
    const int tTextY = 20;
    const int tAddHight = 20;
    pa.drawText(QPoint(tTextX, tTextY), QString("m_node:"));
    const auto& tP1 = m_node.p1;
    const auto& tP2 = m_node.p2;
    const auto& tP3 = m_node.p3;
    const auto& tPickedRect = m_node.pickedRect;

    pa.drawText(QPoint(tTextX, tTextY + tAddHight * 1), QString("hasMouseTracking:%1 ActionType:%2")
                                                            .arg(hasMouseTracking() ? "true" : "false").arg(actionTypeToString(m_actionType)));
    pa.drawText(QPoint(tTextX, tTextY + tAddHight * 2), QString("p1:(%1, %2)  p2:(%3, %4) \n p3:(%5, %6)  pickedRect:(%7, %8, %9 * %10)")
                                           .arg(tP1.x()).arg(tP1.y()).arg(tP2.x()).arg(tP2.y()).arg(tP3.x()).arg(tP3.y())
                                           .arg(tPickedRect.x()).arg(tPickedRect.y()).arg(tPickedRect.width()).arg(tPickedRect.height()));
    pa.restore();
}
