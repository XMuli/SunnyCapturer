// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef PAINTBAR_H
#define PAINTBAR_H
#include <QWidget>
#include <QPointer>
#include <QBoxLayout>
#include "xblureffect.h"
#include "toolbar_level/painttoolbar.h"
#include "toolbar_level/paintctrlbar.h"

class PaintBar : public QWidget
{
    Q_OBJECT
public:
    explicit PaintBar(const Qt::Orientation& orie = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~PaintBar();

    void transposePaintBar(const bool& bTranspose = false);  // true-PaintToolBar在上，PaintCtrlBar在下; false- 颠倒一下顺序
    bool hadDrawBtnsChecked() const;
    void setLowerBlurEffect(const QPixmap &pix, int radius);
    void disableBlurEffect();
    void runImgTranslate();
    Qt::Orientation orie() const;

private:
    void initUI();
    void initConnect();

signals:
    void sigUpdatePaintBarBlurPixmap();
    void sigTextCtrlToggled(const TextFlags& flages);
    void sigPointCtrlReleased(const int& id);                                          // PaintCtrlBar PointCtrl 被按下
    void sigMosaicSliderValueChanged(int id, int val);                                 // PaintCtrlBar Slider 数值被发生改变时候
    void sigPickedColor(const QColor& color);                                          // PaintCtrlBar 调色盘 选取新颜色时候

    void sigPaintToolBtnsRelease(const PaintType& type, const bool& isCheckable, const bool& isChecked);      // paintToolBar 按钮被按下
    void sigPaintCtrlIdReleased(const int& id);                                        // PaintCtrlBar 按钮被按下
    void sigTextFontSizeChanged(const QString &text);                                  // PaintCtrlBar 拉列表的字体的大小被修改
    void sigTextFontFamilyChanged(const QFont &font);                                  // PaintCtrlBar 字体 被发生改变时候
    void sigSetTextFontSizeComboBoxValue(const QString &fontSize);                     // sync 修改下拉列表的字体的大小
    void sigImgTranslate(const ImgTranslateData& data);                  // PaintCtrlBar OCR 翻译

    void sigAutoDisableUndoAndRedo(const bool& undoDisable, const bool& redoDisable);  // ScreenShot 控制图标置灰
    void sigScreenshotUpdate(); // 截图窗口刷新，光标进入和离开时，隐藏准星

private slots:
    void onPaintToolBtnsRelease(const PaintType& type, const bool& isCheckable, const bool &isChecscked);

protected:
    void resizeEvent(QResizeEvent *e) override;
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

private:
    QBoxLayout*                     m_layout;
    Qt::Orientation                 m_orie;
    QPixmap                         m_blurPixmap;    // 模糊透明效果
    QPointer<PaintToolBar>          m_toolBar;
    QPointer<PaintCtrlBar>          m_ctrlBar;
protected:
};

#endif // PAINTBAR_H

