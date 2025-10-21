// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef PARAMETERBAR_H
#define PARAMETERBAR_H

#include "../screen/drawhelper.h"
#include <memory>
#include <QPointer>
#include "base/colorparabar.h"
#include "base/managebar.h"

class BlurWidget;
class QBoxLayout;
class QAbstractButton;
class QToolButton;

class ParameterBar : public QWidget
{
    Q_OBJECT
public:
    explicit ParameterBar(Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~ParameterBar();
    void setBlurBackground(const QPixmap& pix, double blurRadius);

private:
    void initUI();
    void addWidget(QWidget *w);
    void addSpacer();
    QString amendSvgShow(QString path);

    void creatorParaBar(QPointer<ManageBar>& manageBar, const QString& path, const QStringList& items, const bool exclusive = true, const int defaultCheck = 0);
    void initRectBar();
    void initEllipseBar();
    void initArrowBar();
    void initMosaicBar();
    void initTextBar();
    void initSerialnumberBar();
    void initLineWidthBar();
    void removeAllBar();

signals:
    void sigParaBtnId(DrawShape shap, QToolButton* tb);    // ToolButton 发射的信号
    void sigSelColor(QColor);                              // XLabel 发射的信号

public slots:
    void onTBReleased(QAbstractButton* btn);
    void onEnableDraw(bool enable);
    void onSelShape(DrawShape shape, bool checked);

protected:
    void enterEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    double m_scal;
    Qt::Orientations m_orien;
    QBoxLayout* m_layout;
    std::unique_ptr<BlurWidget> m_blur;

    QPointer<ManageBar>    m_rectBar;                      // 矩形
    QPointer<ManageBar>    m_ellipseBar;                   // 圆形
    QPointer<ManageBar>    m_arrowBar;                     // 箭头
    QPointer<ManageBar>    m_mosaicBar;                    // 马赛克
    QPointer<ManageBar>    m_textBar;                      // 文本
    QPointer<ManageBar>    m_serialnumberShape;            // 序号图形
    QPointer<ManageBar>    m_serialnumberType;             // 序号类型
    QPointer<ManageBar>    m_lienWidthBar;                 // 线宽
    QPointer<ColorParaBar> m_colorBar;                     // 取色板
};

#endif // PARAMETERBAR_H
