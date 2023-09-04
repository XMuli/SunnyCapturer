#ifndef CAPTUREHELPER_H
#define CAPTUREHELPER_H

#include <QObject>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QMetaEnum>


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

// paint btn 绘画的图案枚举
enum class PaintShapeType {
    PST_empty,
    PST_rect,
    PST_ellipse,
    PST_arrow,
    PST_pen,
    PST_marker_pen,
    PST_mosaic,
    PST_text,
    PST_serial,
    PST_point
};

QString paintShapeTypeToString(PaintShapeType pst);

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


// 自定义控件
enum class CustomWidgetType {
    CWT_paint_btns_bar,
    CWT_magnifying_glass,
    CWT_picked_rect_tooptip,      // 选中矩形尺寸预览
    CWT_point_changed_tooptip     // 线宽 px 改变
};

void ensurePositiveSize(QRect &rect);                                         // 若宽度或高度为负数，重新生成一个矩形，确保左上角和右下角坐标正确
OrientationType containsForRect(const QRect& rect, const QPoint& pt);         // 判断点在矩形区域的方位
QRect largestRect(const QRect& rect, const QPoint& pt);                       // 获取公共的最大矩形, 通过3个点
QRect largestRect(const QPoint& p1, const QPoint& p2, const QPoint& pt);      // 获取公共的最大矩形, 通过3个点
QRect largestRect(const QPoint& p1, const QPoint& p2);                        // 获取公共的最大矩形, 通过2个点
QRect toAbsoluteRect(const QRect& rect);                                      // 将一个为负的 width/height 的 rect 转化为其为正的；  eg: m_node.pickedRect -> m_node.absoluteRect

void stretchRect(QRect &rect, const QPoint& pt, const OrientationType& type);                                 // 将一个传入 rect 矩形拉伸变大变小
bool allowableRangeErrorForPoint(const QPoint& p1, const QPoint& pt, const int& length = 3);                  // 允许的误差，比如手抖偏移几个像素
bool allowableRangeErrorForLine(const QPoint& p1, const QPoint& p2, const QPoint& pt, const int& length = 3); // 允许的误差，比如手抖偏移几个像素
bool allowableRangeErrorForLine(const QLine& line, const QPoint& pt, const int& length = 3);                  // 允许的误差，比如手抖偏移几个像素

// 每一次鼠标按下，到松开中，操作一次，所需要保存的信息
struct Node
{
    QPoint p1;                                  // 起点
    QPoint p2;                                  // 终点
    QPoint p3;                                  // 轨迹点
    QPoint pt;                                  // 临时保存一下(上一个新的点); 只要使用全程是同一个枚举，即可以放心使用
    std::vector<QPoint>   trackPos;             // move 累计的点
    QRect  pickedRect;                          // 初始绘画位置: 由 p1、p2 构成
    QRect  absoluteRect;                        // 绝对值矩形: 如拉伸时 pickedRect 的widget/height 可能为负数，此时就需要依靠它开show paintBtnsBar 等作为基准参考点

};

struct PaintNode
{
    Node node;
    PaintShapeType pst = PaintShapeType::PST_empty;             // 当前绘画的图案枚举
    bool bShow = false;                                         // true-在 paintEvent 中绘画;反之则不绘画

    int     id = -1;                                            // PST_rect/PST_ellipse/PST_arrow/PST_pen?/PST_marker_pen/PST_mosaic/PST_serial
    int     fuzzyRange = -1;                                    // PST_mosaic: mosaic、 blur    模糊值
                                                                // PST_text is TBD: 后面单独设计为一个富文本编辑框所需要的元素  ???
    QChar   serial;                                             // PST_serial 当前字符
    QColor  serialText;                                         // 序号文字颜色
    QColor  serialBackground;                                   // 序号背景颜色

    QPen pen = QPen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QBrush brush = QBrush(Qt::red, Qt::SolidPattern);
};

void drawShape(const PaintNode& paintNode, QPainter& pa);      // 绘画当某一步骤的图案
void drawArrow(QPainter& pa, const QPoint& p1, const QPoint& p2, int arrowSize = 20);

class CaptureHelper : public QObject
{
    Q_OBJECT
public:
    explicit CaptureHelper(QObject *parent = nullptr);

    static QRect rectFromTowPos(const QPoint& p1, const QPoint& p2);
};



#endif // CAPTUREHELPER_H
