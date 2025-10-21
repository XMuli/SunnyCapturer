// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef CAPTUREHELPER_H
#define CAPTUREHELPER_H
#include <QObject>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QMetaEnum>
#include <QTextDocument>
#include "xtextedit.h"

class ScreenShot;

// C++11 新增带作用域的枚举，用 enum class  或enum struct（两者等价）声明。
// https://blog.csdn.net/luckysym/article/details/1666114
enum class ActionType {
    AT_wait,
    AT_picking_detection_windows_rect,   // 采摘自动检测到的矩形
    AT_picking_custom_rect,              // 采摘自定义的矩形
    AT_select_drawn_shape,               // 选择已绘画的图形(不算当前正在绘画中的)
    AT_drawing_shap_without_text,                     // 绘画各种图形，除了文字编辑
    AT_drawing_only_text,                     // 仅仅文字编辑，因为涉及到图标的改变，需要修改
    AT_move_drawn_shape,
    AT_move_picked_rect,                 // 移动采摘的矩形
    AT_stretch_drawn_shape,
    AT_stretch_picked_rect               // 拉伸采摘的矩形
};

QString actionTypeToString(ActionType actionType);

// paint btn 绘画的图案枚举
enum class PaintShapeType {
    PST_empty,
    PST_rect,
    PST_ellipse,
    PST_arrow,
    PST_manual_curve,  // 手绘曲线
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
    CWT_tools_bar,                // 一二级的绘画工具栏
    CWT_color_lens,          // 放大镜 + 取色器
    CWT_picked_rect_tooptip,      // 选中矩形尺寸预览
    CWT_point_changed_tooptip     // 线宽 px 改变
};


enum class ImageSaveType {
    IST_manual_save,
    IST_quick_save,
    IST_auto_save
};
Q_DECLARE_METATYPE(ImageSaveType)                     // 可以被 QVariant 类型存储
Q_DECLARE_FLAGS(ImageSaveTypes, ImageSaveType)        // 枚举 ImageSaveType 生成宏 ImageSaveTypes
Q_DECLARE_OPERATORS_FOR_FLAGS(ImageSaveTypes)         // 重载宏 ImageSaveTypes 的 |() 函数

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


struct SerialNode
{
    int number = 0;
    QChar letter = 'a' - 1;
    int pointSize;
    QColor  background;
};

// 完成一次操作, 需要执行两步骤: 1. 初次点击创建填写文字时候，可以随意拖曳位置 2. 必须掉此一次其外部窗口，才能够成为保存成功入栈
enum class EditType {
    ET_nullptr,    // 为空，此时还没有被创建 1 step
    ET_generated,  // 已 first 生成此对象  1 step
    ET_editing,    // 编辑文字中           2 step（分界点，用于判断使用）
    ET_finish      // 已经编辑完成且入栈    2 step
};

struct PaintNode
{
    Node node;
    PaintShapeType pst = PaintShapeType::PST_empty;             // 当前绘画的图案枚举
    bool bShow = false;                                         // true-在 paintEvent 中绘画;反之则不绘画

    int     id = -1;                                            // PST_rect/PST_ellipse/PST_arrow/PST_manual_curve?/PST_marker_pen/PST_mosaic/PST_serial
    int     pixelatedFuzzy;                                     // PST_mosaic: mosaic、 blur    模糊值
    int     smoothFuzzy;
    int     markerPenWidth;                                     // 记号笔的宽度

    QPixmap pixmap;
                                                                // PST_text is TBD: 后面单独设计为一个富文本编辑框所需要的元素  ???
    SerialNode   serialNode;                                    // PST_serial 序号相关
//
    XTextEdit *xTextEdit = nullptr;                             // true-显示； false-（如被撤销）就隐藏了
    EditType editType = EditType::ET_nullptr;

    QPen pen;
    QBrush brush;

    void printf() const;
    // 默认构造函数
    PaintNode()
        : pst(PaintShapeType::PST_empty)
        , bShow(false)
        , id(-1)
        , pixelatedFuzzy(10)
        , smoothFuzzy(10)
        , markerPenWidth(25)
        , editType(EditType::ET_nullptr)
        , pen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
        , brush(Qt::red, Qt::SolidPattern)
    {
        // 在此添加其他成员变量的初始化（如果有需要的话）
    }
};


XTextEdit* showCreatorRichText(const QTextDocument *doc, const QRect &rect, QWidget *w);
void drawShape(const PaintNode& paintNode, QPainter& pa, const bool& isFinishScreenShot = false);      // 绘画当某一步骤的图案

void drawArrow(QPainter& pa, const QPoint& p1, const QPoint& p2, int arrowSize = 20);
void pixelatedMosaic(QPixmap &pixmap, const int& px = 20);
void smoothMosaic(QPixmap& pixmap, int radius = 10);
QPixmap monitoredDesktopPixmap(const QRect& rect);

void drawBorderSunny(QPainter& pa, const QRect& rt, const bool& bCorner) ;
void drawBorderMacOS(QPainter& pa, const QRect& rt, int num = 8) ;
void drawBorderDeepin(QPainter& pa, const QRect& rt, int num = 8) ;
void drawBorder(QPainter& pa, const QRect& rt, int num = 8) ;
void drawCrosshair(QPainter& pa, const QPoint& pt, const QRect& vdRt);

class CaptureHelper : public QObject
{
    Q_OBJECT
public:
    explicit CaptureHelper(QObject *parent = nullptr);

    static QRect rectFromTowPos(const QPoint& p1, const QPoint& p2);
};



#endif // CAPTUREHELPER_H
