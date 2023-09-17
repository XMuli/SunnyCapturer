#include "capturehelper.h"
#include <QDebug>
#include <QMetaEnum>
#include <QMetaObject>
#include <QPainterPath>
#include <QPen>
#include <QCursor>
#include <QPointF>
#include <QtMath>
#include <QtGlobal>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QFontMetrics>
#include "xtextedit.h"
#include "../paint_bar/toolbar_level/paintbarhelper.h"
#include "../../../data/configmanager.h"

CaptureHelper::CaptureHelper(QObject *parent)
    : QObject{parent}
{

}

QRect CaptureHelper::rectFromTowPos(const QPoint &p1, const QPoint &p2)
{
    int xMin = qMin<int>(p1.x(), p2.x());
    int xMax = qMax<int>(p1.x(), p2.x());
    int yMin = qMin<int>(p1.y(), p2.y());
    int yMax = qMax<int>(p1.y(), p2.y());

    // 若是重复点，则会返回 QRect(0, 0, 1, 1);
    if (xMin == xMax && yMin == yMax)
        return QRect(xMin, xMax, 0, 0);
    else
        return QRect(QPoint(xMin, yMin), QPoint(xMax, yMax));
}

QString actionTypeToString(ActionType actionType)
{
    switch (actionType) {
    case ActionType::AT_wait:
        return "AT_wait";
    case ActionType::AT_picking_custom_rect:
        return "AT_picking_custom_rect";
    case ActionType::AT_picking_detection_windows_rect:
        return "AT_picking_detection_windows_rect";
    case ActionType::AT_select_picked_rect:
        return "AT_select_picked_rect";
    case ActionType::AT_select_drawn_shape:
        return "AT_select_drawn_shape";
    case ActionType::AT_drawing_shap:
        return "AT_drawing_shap";
    case ActionType::AT_move_drawn_shape:
        return "AT_move_drawn_shape";
    case ActionType::AT_move_picked_rect:
        return "AT_move_picked_rect";
    case ActionType::AT_stretch_drawn_shape:
        return "AT_stretch_drawn_shape";
    case ActionType::AT_stretch_picked_rect:
        return "AT_stretch_picked_rect";
    default:
        return "Unknown";
    }
}

QString paintShapeTypeToString(PaintShapeType pst)
{
    switch (pst) {
    case PaintShapeType::PST_empty:
        return "PST_empty";
    case PaintShapeType::PST_rect:
        return "PST_rect";
    case PaintShapeType::PST_ellipse:
        return "PST_ellipse";
    case PaintShapeType::PST_arrow:
        return "PST_arrow";
    case PaintShapeType::PST_pen:
        return "PST_pen";
    case PaintShapeType::PST_marker_pen:
        return "PST_marker_pen";
    case PaintShapeType::PST_mosaic:
        return "PST_mosaic";
    case PaintShapeType::PST_text:
        return "PST_text";
    case PaintShapeType::PST_serial:
        return "PST_serial";
    case PaintShapeType::PST_point:
        return "PST_point";
    default:
        return "Unknown";
    }
}

// length = 3 px 之内都看作是点击的这个已存在的 检测矩形，合理误差
bool allowableRangeErrorForPoint(const QPoint &p1, const QPoint &pt, const int &length)
{
    if (qAbs(p1.x() - pt.x()) <= length &&  qAbs(p1.y() - pt.y()) <= length)
        return true;
    else
        return false;
}

bool allowableRangeErrorForLine(const QPoint &p1, const QPoint &p2, const QPoint &pt, const int &length)
{
    if (p1 == p2) return false;

    QRect rect;
    if (p1.x() == p2.x() && p1.y() != p2.y()) {          // vertical
        rect = QRect(p1.x() - length, qMin(p1.y(), p2.y()), length * 2, qAbs(p1.y() - p2.y()));
    } else if (p1.x() != p2.x() && p1.y() == p2.y()) {   // horizontal
        rect = QRect(qMin(p1.x(), p2.x()), p1.y() - length, qAbs(p1.x() - p2.x()), length * 2);
    } else if (p1.x() != p2.x() && p1.y() != p2.y()) {   // incline
        qDebug() << "倾斜矩形，遇到时候时候再来完善";
        return false;
    }

    return rect.contains(pt);
}

bool allowableRangeErrorForLine(const QLine &line, const QPoint &pt, const int &length)
{
    return allowableRangeErrorForLine(line.p1(), line.p2(), pt, length);
}

OrientationType containsForRect(const QRect &rect, const QPoint &pt)
{
    if (!rect.isEmpty()) {
        if (rect.contains(pt, true)) {
            return OrientationType::OT_internal;
        } else {
            if (rect.top() <= pt.y() && pt.y() <= rect.bottom() && pt.x() <= rect.left()) {           // 仅左侧中间
                return OrientationType::OT_left;
            } else if (rect.left() <= pt.x() && pt.x() <= rect.right() && pt.y() <= rect.top()) {     // 仅上面中间
                return OrientationType::OT_top;
            } else if (rect.top() <= pt.y() && pt.y() <= rect.bottom() && pt.x() >= rect.right()) {   // 仅右侧中间
                return OrientationType::OT_right;
            } else if (rect.left() <= pt.x() && pt.x() <= rect.right() && pt.y() >= rect.bottom()) {   // 仅下面中间
                return OrientationType::OT_bottom;
            } else if (pt.x() < rect.left() && pt.y() < rect.top()) {     // 左上角
                return OrientationType::OT_topLeft;
            } else if (pt.x() > rect.right() && pt.y() < rect.top()) {    // 右上角
                return OrientationType::OT_topRight;
            } else if (pt.x() < rect.left() && pt.y() > rect.bottom()) {    // 左下角
                return OrientationType::OT_bottomLeft;
            } else if (pt.x() > rect.right() && pt.y() > rect.bottom()) {    // 右下角
                return OrientationType::OT_bottomRight;
            } else {
                return OrientationType::OT_empty;
            }
        }
    }

    return OrientationType::OT_empty;
}

QRect largestRect(const QRect &rect, const QPoint &pt)
{
    return largestRect(rect.topLeft(), rect.bottomRight(), pt);
}

QRect largestRect(const QPoint &p1, const QPoint &p2, const QPoint &pt)
{
    const int& left = qMin(qMin(p1.x(), p2.x()), pt.x());
    const int& right = qMax(qMax(p1.x(), p2.x()), pt.x());
    const int& top = qMin(qMin(p1.y(), p2.y()), pt.y());
    const int& bottom = qMax(qMax(p1.y(), p2.y()), pt.y());
    return QRect(QPoint(left, top), QPoint(right, bottom));
}

QRect largestRect(const QPoint &p1, const QPoint &p2)
{
    int left = qMin(p1.x(), p2.x());
    int right = qMax(p1.x(), p2.x());
    int top = qMin(p1.y(), p2.y());
    int bottom = qMax(p1.y(), p2.y());

    return QRect(QPoint(left, top), QPoint(right, bottom));
}

void stretchRect(QRect &rect, const QPoint &pt, const OrientationType &type)
{
    if (type == OrientationType::OT_internal) {
    } else if (type == OrientationType::OT_left) {
        const int& length = pt.x() - rect.left();
        rect.setLeft(rect.left() + length);
    } else if (type == OrientationType::OT_top) {
        const int& length = pt.y() - rect.top();
        rect.setTop(rect.top() + length);
    } else if (type == OrientationType::OT_right) {
        const int& length = pt.x() - rect.right();
        rect.setRight(rect.right() + length);
    } else if (type == OrientationType::OT_bottom) {
        const int& length = pt.y() - rect.bottom();
        rect.setBottom(rect.bottom() + length);
    } else if (type == OrientationType::OT_topLeft) {
        const auto& tPos = pt - rect.topLeft();
        rect.setTopLeft(rect.topLeft()  + tPos);
    } else if (type == OrientationType::OT_topRight) {
        const auto& tPos = pt - rect.topRight();
        rect.setTopRight(rect.topRight()  + tPos);
    } else if (type == OrientationType::OT_bottomRight) {
        const auto& tPos = pt - rect.bottomRight();
        rect.setBottomRight(rect.bottomRight()  + tPos);
    } else if (type == OrientationType::OT_bottomLeft) {
        const auto& tPos = pt - rect.bottomLeft();
        rect.setBottomLeft(rect.bottomLeft()  + tPos);
    } else {
        qDebug() << "stretchRect is empty?";
    }
}


void ensurePositiveSize(QRect &rect)
{
    int x = rect.left();
    int y = rect.top();
    int width = rect.width();
    int height = rect.height();

    if (width < 0) {
        x += width;
        width = qAbs(width);
    }

    if (height < 0) {
        y += height;
        height = qAbs(height);
    }

    rect = QRect(QPoint(x, y), QSize(width, height));
}


// 当前使用者:当前是拉伸窗口时，放widget/height为负数时候，确保 paintBtnsBar 能显示在正确的位置上
QRect toAbsoluteRect(const QRect &rect)
{
    QRect ret(rect);

    if (rect.width() < 0 || rect.height() < 0)
        ret = largestRect(rect.topLeft(), rect.bottomRight());

    return ret;
}

void drawShape(const PaintNode &paintNode, QPainter &pa)
{
    pa.save();
    pa.setRenderHint(QPainter::Antialiasing);

    if (paintNode.pst == PaintShapeType::PST_rect) {
        pa.setPen(paintNode.pen);

        if (paintNode.id == 0) {
            pa.setBrush(Qt::NoBrush);
        } else if (paintNode.id == 1) {
            pa.setBrush(paintNode.pen.color());
        }

        const auto& rect = largestRect(paintNode.node.p1, paintNode.node.p2);
        pa.drawRect(rect);

    } else if (paintNode.pst == PaintShapeType::PST_ellipse) {
        pa.setPen(paintNode.pen);

        if (paintNode.id == 0) {
            pa.setBrush(Qt::NoBrush);
        } else if (paintNode.id == 1) {
            pa.setBrush(paintNode.pen.color());
        }

        const auto& rect = largestRect(paintNode.node.p1, paintNode.node.p2);
        pa.drawEllipse(rect);
    } else if (paintNode.pst == PaintShapeType::PST_arrow) {
        pa.setPen(paintNode.pen);
        pa.setBrush(Qt::NoBrush);

        if (paintNode.id == 0) {
            drawArrow(pa, paintNode.node.p1, paintNode.node.p2);
        } else if (paintNode.id == 1) {
            pa.drawLine(paintNode.node.p1, paintNode.node.p2);
        } else if (paintNode.id == 2) {
        }

    } else if (paintNode.pst == PaintShapeType::PST_pen) {
        pa.setPen(paintNode.pen);
        pa.setBrush(Qt::NoBrush);
        const auto& trackPos = paintNode.node.trackPos;

        // 绘制平滑的轨迹
        if (trackPos.size() >= 2) {
            QPainterPath path;
            path.moveTo(trackPos[0]);

            for (int i = 1; i < trackPos.size(); ++i) {
                int n = trackPos.size();
                QPointF p0 = i == 1 ? trackPos[0] : trackPos[i - 2];
                QPointF p1 = trackPos[i - 1];
                QPointF p2 = trackPos[i];
                QPointF p3 = (i == n - 1) ? p2 : trackPos[i + 1];

                // 计算三次样条曲线控制点
                QPointF ctrl1 = p1 + (p2 - p0) / 6.0;
                QPointF ctrl2 = p2 - (p3 - p1) / 6.0;

                path.cubicTo(ctrl1, ctrl2, p2);
            }

            pa.drawPath(path);
        }

        // 绘制原始点
        for (const auto& point : trackPos) {
            pa.drawPoint(point);
        }

    } else if (paintNode.pst == PaintShapeType::PST_marker_pen) {

        pa.setPen(Qt::NoPen);
        QColor color = paintNode.brush.color();
        color.setAlpha(0.3 * 255);
        pa.setBrush(color);
        const auto& rect = largestRect(paintNode.node.p1, paintNode.node.p2);

        if (paintNode.id == 0) {
            pa.drawEllipse(rect);
        } else if (paintNode.id == 1) {
            pa.drawRect(rect);
        }

    } else if (paintNode.pst == PaintShapeType::PST_mosaic) {
        /* 绘画马赛克/毛玻璃功能思路:
         * 1. Mouse Press + drawing_shap(mosaic) 条件下,保存此刻整个屏幕的截图，备用
         * 2. Mouse Move/Release 时刻，都通过深拷贝 pix = m_mosaicPix.copy(rect) 获取局部元素
         * 3. 按照不同选项，进行 pixelatedMosaic / smoothMosaic 处理， 然后保存到成员变量 m_paintNode.pixmap = pix; 中
         * 4. 在 Mouse Release时刻，确定最终完成 rect 范围(move 时其实也是实时确定); 进行 m_redo.push_back(m_paintNode) +  m_paintNode.pixmap = QPixmap(); 归为等操作
         * 5. 就可以在函数中，直接进行绘画即可
         */
        const auto& rect = largestRect(paintNode.node.p1, paintNode.node.p2);
        if (!paintNode.pixmap.isNull() && rect.isValid())
            pa.drawPixmap(rect, paintNode.pixmap);

    } else if (paintNode.pst == PaintShapeType::PST_text) {
        // nothing, 在外面 showCreatorRichText() 处理了， 绘画图片感觉也可以参考
    } else if (paintNode.pst == PaintShapeType::PST_serial) {
        QString str;
        if (paintNode.id == 0) {
            str = QString::number(paintNode.serialNode.number);
        } else if (paintNode.id == 1) {
            str = paintNode.serialNode.letter;
        }

        QFont font(pa.font());
        font.setPointSize(15);
        pa.setFont(font);

        const QFontMetrics fm(pa.fontMetrics());
        QRect boundingRect = fm.boundingRect(str);
        const int& width = qMax<int>(boundingRect.width(), boundingRect.height());
        boundingRect.setSize(QSize(width, width));
        boundingRect.moveCenter(paintNode.node.p2);
        const int margin = qMax<int>(width / 6, 4);
        const QRect adjustRt = boundingRect.adjusted(-margin, -margin, margin, margin);

        pa.setPen(/*paintNode.pen*/ QPen(Qt::white, 2));
        pa.setBrush(paintNode.brush);
        pa.drawRect(adjustRt);
        pa.setPen(/*paintNode.pen*/ QPen(Qt::white, 2));
        pa.drawText(boundingRect, Qt::AlignCenter, str);

    } else if (paintNode.pst == PaintShapeType::PST_point) {
        pa.setPen(paintNode.pen);
        pa.setBrush(Qt::NoBrush);
        pa.drawPoint(QCursor::pos());  // 设置延长几秒钟比较好

    } else {
        qDebug() << "paintNode.pst is PST_empty!";
    }

    pa.restore();

}


void drawArrow(QPainter& pa, const QPoint& p1, const QPoint& p2, int arrowSize)
{
    QLineF line(p1, p2);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    pa.drawLine(line);
    double angle = qAcos(line.dx() / line.length());
    if (line.dy() >= 0) angle = 2.0 * M_PI - angle;

    QPointF destArrowP1 = p2 + QPointF(qSin(angle - M_PI / 3) * arrowSize, qCos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = p2 + QPointF(qSin(angle - M_PI + M_PI / 3) * arrowSize, qCos(angle - M_PI + M_PI / 3) * arrowSize);

    pa.drawLine(QLineF(destArrowP1, p2));
    pa.drawLine(QLineF(destArrowP2, p2));
}

void pixelatedMosaic(QPixmap &pixmap, const int& px)
{
    if (pixmap.isNull() || px <= 0) return;

    QImage image = pixmap.toImage(); // 将QPixmap转换为QImage以进行像素级操作
    for (int x = 0; x < image.width(); x += px) {
        for (int y = 0; y < image.height(); y += px) {
            QRect rect(x, y, px, px);

            // 确保矩形区域在图像范围内
            if (rect.right() >= image.width()) rect.setWidth(image.width() - x);
            if (rect.bottom() >= image.height()) rect.setHeight(image.height() - y);

            // 获取矩形区域的平均颜色
            QRgb averageColor = image.copy(rect).scaled(1, 1, Qt::KeepAspectRatio, Qt::SmoothTransformation).pixel(0, 0);

            // 将矩形区域填充为平均颜色
            for (int i = x; i < x + rect.width(); i++) {
                for (int j = y; j < y + rect.height(); j++) image.setPixel(i, j, averageColor);
            }
        }
    }

    pixmap = QPixmap::fromImage(image); // 将QImage转回QPixmap
}

void smoothMosaic(QPixmap &pixmap, int radius)
{
    if (pixmap.isNull() || radius <= 0) return;

    radius = qMax<int>(10, radius);
    QGraphicsBlurEffect* blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(radius);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
    item->setGraphicsEffect(blur);

    QGraphicsScene scene;
    scene.addItem(item);

    QPainter painter(&pixmap);
    const double& DPI = qGuiApp->devicePixelRatio();
    const QRect tRt(pixmap.rect().topLeft() / DPI, pixmap.rect().size() / DPI);
    scene.render(&painter, tRt, QRectF());

    blur->setBlurRadius(radius + 2);
    // multiple repeat for make blur effect stronger
    scene.render(&painter, tRt, QRectF());
}

// 返回此时此刻的桌面截图，作为马赛克原始素材使用
QPixmap monitoredDesktopPixmap(const QRect &rect)
{
    QPixmap pixmap;
    if (rect.isValid()) {
        pixmap =  qGuiApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), rect.x(), rect.y(), rect.width(), rect.height());
        qDebug() << "monitoredDesktopPixmap(), &pixmap:" << &pixmap << "pixmap:" << pixmap;
    }

    return pixmap;
}

void PaintNode::printf() const
{
    qDebug() << "pst:" << paintShapeTypeToString(pst) << "bShow:" << bShow
             << "&pixmap:" << &pixmap << "pixmap:" << pixmap;

    const auto& p1 = node.p1;
    const auto& p2 = node.p2;
    const auto& p3 = node.p3;
    const auto& pt = node.pt;
    const auto& pickedRect = node.pickedRect;
    const auto& absoluteRect = node.absoluteRect;

    qDebug() << QString("p1(%1, %2) p2(%3, %4) p3(%5, %6) pt(%7, %8)")
                    .arg(p1.x()).arg(p1.y())
                    .arg(p2.x()).arg(p2.y())
                    .arg(p3.x()).arg(p3.y())
                    .arg(pt.x()).arg(pt.y());

    qDebug() << QString("pickedRect(%1, %2, %3 * %4) absoluteRect(%5, %6, %7 * %8)\n\n")
                    .arg(pickedRect.x()).arg(pickedRect.y()).arg(pickedRect.width()).arg(pickedRect.height())
                    .arg(absoluteRect.x()).arg(absoluteRect.y()).arg(absoluteRect.width()).arg(absoluteRect.height());
}

// 默认构造函数
PaintNode::PaintNode()
    : pst(PaintShapeType::PST_empty),
    bShow(false),
    id(-1),
    pixelatedFuzzy(10),
    smoothFuzzy(10),
    xTextEditType(XTextEditType::XTET_nullptr),
    pen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
    brush(Qt::red, Qt::SolidPattern) {
    // 在此添加其他成员变量的初始化（如果有需要的话）
}

void showCreatorRichText(const QTextDocument* doc, const QRect& rect, QWidget *w)
{
    if (!doc || doc->isEmpty()) return;      //    if (paintNode.xTextEditType != XTextEditType::XTET_finish) return;
    XTextEdit* newEdit = new XTextEdit(w);   // 改用关联或者 std::智能指针，对哦， w 作主窗口，也是已经实现了的回收； 但是会这个位置重复 new， qdebug 看下，不然会有不断地 new 照成内存泄露
    newEdit->setDocument(doc->clone());      // 最关键的一个一行，对 QTextDocument 进行 1:1 的文本拷贝

    newEdit->show();                         // 这三行的顺序不可以混
    newEdit->setFixedSize(rect.size());
    newEdit->move(rect.topLeft());
    newEdit->setReadOnly(true);
    newEdit->setFocusAble(false);
//    newEdit->setMouseTracking(false);
//    newEdit->setFocusPolicy(Qt::NoFocus);
//    newEdit->setCursor(Qt::ArrowCursor);
    static int i = 1;
    qDebug() << "=============#=====>showDrewText() i:" << i++ << "newEdit:" << newEdit;
}


void drawBorderSunny(QPainter &pa, const QRect &rt)
{
    pa.save();
    pa.setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    const int addFixWidth = 6;
    pen.setWidth(borderWidth() + addFixWidth);
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

    pen.setWidth(borderWidth());
    pa.setPen(pen);
    pa.drawRect(rt);
    pa.restore();
}

void drawBorderMacOS(QPainter &pa, const QRect &rt, int num)
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

void drawBorderDDE(QPainter &pa, const QRect &rt, int num)
{
    pa.save();
    pa.setPen(QPen(QColor(14, 112, 255), borderWidth()));
    pa.setBrush(Qt::NoBrush);
    pa.drawRect(rt);

    pa.setPen(Qt::NoPen);

    QIcon icon(":/resources/screenshot_ui/screenshot/board_circle.svg");
    const int radius = 6;
    QPixmap pixmap = icon.pixmap(QSize(radius, radius) * 4);
    pixmap.setDevicePixelRatio(qApp->devicePixelRatio());

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

void drawBorder(QPainter &pa, const QRect &rt, int num)
{
    const auto& style  = CONF_MANAGE.property("XInterface_style").toString();
    if (style == "Sunny") {
        drawBorderSunny(pa, rt);
    } else if (style == "MacOS") {
        drawBorderMacOS(pa, rt, num);
    } else if (style == "DDE") {
        drawBorderDDE(pa, rt, num);
    } else {
        qDebug() << "other empty border style!";
    }
}


void drawCrosshair(QPainter &pa, const QPoint &pt, const QRect& vdRt)
{
//    const bool& bDraw = CONF_MANAGE.property("XInterface_crosshair_show").toBool();
//    if (!bDraw)  return;

    pa.save();
    pa.setRenderHint(QPainter::Antialiasing, true);
    pa.setPen(QPen(QColor(CONF_MANAGE.property("XInterface_crosshair").toString()), CONF_MANAGE.property("XInterface_crosshair_width").toInt()));
    pa.setBrush(Qt::NoBrush);
    const QLine l1(vdRt.left(), pt.y(), vdRt.right(), pt.y());
    const QLine l2(pt.x(), vdRt.top(), pt.x(), vdRt.bottom());
    pa.drawLine(l1);
    pa.drawLine(l2);
    pa.restore();
}

