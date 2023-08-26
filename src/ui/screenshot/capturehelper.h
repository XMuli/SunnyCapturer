#ifndef CAPTUREHELPER_H
#define CAPTUREHELPER_H

#include <QObject>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QPainter>

// C++11 新增带作用域的枚举，用 enum class  或enum struct（两者等价）声明。
// https://blog.csdn.net/luckysym/article/details/1666114
enum class ActionType {
    AT_wait,
    AT_picking_detection_windows_rect,   // 采摘自动检测到的矩形
    AT_picking_custom_rect,              // 采摘自定义的矩形
    AT_select_picked_rect,               // 选中当前 框选的边框 矩形
    AT_select_drawn_shape,               // 选择已绘画的图形(不算当前正在绘画中的)
    AT_drawing_shap,
    AT_move_drawn_shape,
    AT_move_picked_rect,
    AT_stretch_drawn_shape,
    AT_stretch_picked_rect
};

QString actionTypeToString(ActionType actionType);

struct PainterEnv
{
    PainterEnv() {}

    QPen     pen = QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QBrush   brush = QBrush(Qt::red, Qt::SolidPattern);
    QPainter painter;

};

// 每一次鼠标按下，到松开中，操作一次，所需要保存的信息
struct Node
{
    QPoint p1;                                  // 起点
    QPoint p2;                                  // 终点
    QPoint p3;                                  // 轨迹点
    QPoint pt;                                  // 临时保存一下(上一个新的点)
    std::vector<QPoint>   trackPos;             // move 累计的点
    QRect  pickedRect;                          // 初始绘画位置: 由 p1、p2 构成

    QPen pen = QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QBrush brush = QBrush(Qt::red, Qt::SolidPattern);
};

class CaptureHelper : public QObject
{
    Q_OBJECT
public:
    explicit CaptureHelper(QObject *parent = nullptr);

    static QRect rectFromTowPos(const QPoint& p1, const QPoint& p2);
};



#endif // CAPTUREHELPER_H
