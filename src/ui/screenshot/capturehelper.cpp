#include "capturehelper.h"
#include "qdebug.h"
#include <QtGlobal>

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


