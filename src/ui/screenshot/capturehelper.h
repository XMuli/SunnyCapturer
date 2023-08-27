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

// 判断某个点在 pickedRect 中的方位
enum class OrientationType {
    OT_empty,
    OT_internal,

    OT_left,
    OT_top,
    OT_right,
    OT_bottom,

    OT_topLeft,
    OT_topRight,
    OT_bottomLeft,
    OT_bottomRight
};

void ensurePositiveSize(QRect &rect);                                         // 若宽度或高度为负数，重新生成一个矩形，确保左上角和右下角坐标正确
OrientationType containsForRect(const QRect& rect, const QPoint& pt);         // 判断点在矩形区域的方位
QRect largestRect(const QRect& rect, const QPoint& pt);                       // 获取公共的最大矩形, 通过3个点
QRect largestRect(const QPoint& p1, const QPoint& p2, const QPoint& pt);      // 获取公共的最大矩形, 通过3个点
QRect largestRect(const QPoint& p1, const QPoint& p2);                        // 获取公共的最大矩形, 通过2个点

void stretchRect(QRect &rect, const QPoint& pt, const OrientationType& type);
//void setMovePickedRect(QRect &rect, const QPoint& pt);                           // 移动矩形的位置

bool allowableRangeErrorForPoint(const QPoint& p1, const QPoint& pt, const int& length = 3); // 允许的误差，比如手抖偏移几个像素
bool allowableRangeErrorForLine(const QPoint& p1, const QPoint& p2, const QPoint& pt, const int& length = 3); // 允许的误差，比如手抖偏移几个像素
bool allowableRangeErrorForLine(const QLine& line, const QPoint& pt, const int& length = 3); // 允许的误差，比如手抖偏移几个像素


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
    QPoint pt;                                  // 临时保存一下(上一个新的点); 只要使用全程是同一个枚举，即可以放心使用
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
