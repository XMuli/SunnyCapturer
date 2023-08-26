#include "capturehelper.h"

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
