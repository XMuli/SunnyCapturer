// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QWidget>
#include <QScreen>
#include <QList>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QRect>
#include <QPointer>
#include "tips.h"
#include "xtextedit.h"
#include "xocrtextedit.h"
#include "capturehelper.h"
#include "../paint_bar/paintbar.h"
#include "../commom/ocr/networkocr.h"
#include "../../commom/communication/communication.h"
#include "windowsrect.h" // 引用位置在 Qt 库之后，避免 x11 库冲突

class ScreenShot : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenShot(const HotKeyType &type, const Qt::Orientation& orie = Qt::Horizontal, QWidget *parent = nullptr);  // Horizontal Vertical
    virtual ~ScreenShot();

    void capture();
private:
    void btnPin();
    void btnUndo();
    void btnRedo();
    void btnSave();
    void btnCancel();
    void btnFinish();
    QPixmap finishPixmap();
    QPixmap finishDrewPixmap(const QRect& rect = QRect(), const bool& isDrawOnOrigPix = false);

    void originalPixmap();
    void setMosaicPix();
    QString imageSavePath(const ImageSaveType& types);
    bool imageSave(const QString& path, const bool& bClose = true);
    void imageQuickSave();

    void setTextFontSize(const int& stepY, const int &width, const bool& bMouse, const bool& bShowPointTips = true);
    void autoDisableUndoAndRedo();

signals:
    void sigSetTextFontSizeComboBoxValue(const QString &fontSize);     // sync 修改下拉列表的字体的大小
    void sigAutoDisableUndoAndRedo(const bool& undoDisable, const bool& redoDisable);

public slots:
    void onPaintBtnRelease(const PaintType& type, const bool& isCheckable);
    void onPaintCtrlIdReleased(const int& id);

    void onTextCtrlToggled(const TextFlags& flages);
    void onPointCtrlReleased(const int& id);
    void onMosaicSliderValueChanged(int id, int val);
    void onHidePointTips();
    void onUpdateToolBarBlurPixmap();
    void onPickedColor(const QColor& color);
    void onTextFontFamilyChanged(const QFont &font);
    void onTextFontSizeChanged(const QString &fontSize);
    void onOcrTranslateCtrlIdReleased(const OcrTranslateData& data);                              // 开始 OCR Translate
    void onOCRImageGenerateFinsh(const QSize& size, const QString& path);                         // OCR Translate 后的图片已保存成功
    void onOCRTextCtrlIdReleased(const OcrTextData &data);                                        // 开始 OCR Text
    void onOCRTextGenerateFinsh(const QByteArray& response, const OcrTextData& ocrTextData);           // OCR Text 后的文本已解析成功
    void onOcrTranslateCtrlHide();
    void onOcrTextCtrlHide();

private:
    void initUI();
    void initConnect();
    void drawShadowOverlay(const QRect& fullRect, const QRect& pickedRect, QPainter& pa) const;

    void stashMosaicPixmap();           // 定格此刻的图片，用作打马赛克的素材
    void setCursorShape(const OrientationType &type, const QPoint &pt);
    void setMovePickedRect();
    void showCustomWidget(QWidget* w);
    void showCrosshair(QPainter &pa, const QPoint &pt, const QRect& vdRt) const;

    QScreen* currentScreen(const QPoint &pos = QCursor::pos()) const;
    void preDestruction();             // 销毁对象之前的清理工作
    void monitorsInfo() const;
    void rectNodesMapFromGlobal();
    void firstRectNodesAssignmentNode();  // 枚举遍历的窗口信息，将第一个赋值给 m_node
    QPoint customWidgetShowPositionRule(const CustomWidgetType& cwt);
    void showPointTips(const QString &text);
    void showPickedRectTips();

    void printfDevelopProjectInfo(QPainter &pa);
    void prinftWindowsRects(QPainter &pa);

    void dealMousePressEvent(QMouseEvent *e);
    void dealMouseReleaseEvent(QMouseEvent *e);
    void dealMouseMoveEvent(QMouseEvent *e);
    void adjustPickedRect(QKeyEvent* e);                                           // Move or Stretch

protected:
    // [核心: 窗口的绘画的坐标都是 (0, 0) 开始； 对于绝对坐标，可以考虑转换为 相对坐标来计算 ！！！ 统一]
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    void closeEvent(QCloseEvent *e) override;

private:
    QScreen*                 m_primaryScreen;
    QList<QScreen *>         m_screens;
    QPixmap                  m_origPix;            // 原始象图, 初次赋值后，设计为只读模式
    QPixmap                  m_mosaicPix;          // 准备使用马赛克/完成截图相关功能时候
    QPixmap                  m_ocrGeneratePix;     // OCR 翻译后的图片
    QRect                    m_vdRect;             // virtual Desktop Rect;

    bool                     m_bFistPressed;       // true-已经按下; false-还没有按过 是否按下过第一次
    bool                     m_bAutoDetectRect;    // 是否自动监测矩形
    HotKeyType               m_HotKeyType;         // 初始化进来的状态，采用何用截图方式
    ActionType               m_actionType;         // 当前的操作状态
    Node                     m_node;               // 一次操作的集合
    QPointer<PaintBar>       m_paintBar;           // PaintBar
    QPointer<NetworkOCR>     m_networkOCR;         // OCR 翻译类

    PaintNode                m_paintNode;          // 当前绘画元素
    std::vector<PaintNode>   m_undo;               // 撤销-图案元素
    std::vector<PaintNode>   m_redo;               // 重做-图案元素

    OrientationType          m_stretchPickedRectOrieType;
    Qt::Orientation          m_orie;
    std::vector<RectNode>    m_rectNodes;

    QPointer<XTextEdit>      m_edit;
    QPointer<XOcrTextEdit>   m_ocrTextEdit;

    QPointer<Tips>           m_pointTips;
    QPointer<Tips>           m_pickedRectTips;
    QTimer*                  m_timerPoint;
    static QFont             m_textFont;            // 绘画字体的时候，显示的
};


const QRect xrectToQRect(const XRECT &rect);

#if defined(Q_OS_WIN)
#elif defined(Q_OS_LINUX)
#elif defined(Q_OS_MAC)
#endif


#endif // SCREENSHOT_H

