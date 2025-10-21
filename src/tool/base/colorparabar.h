// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>
#ifndef COLORPARABAR_H
#define COLORPARABAR_H

#include <QColor>
#include <QMap>
#include <QString>
#include <QWidget>

enum class ColorParaBarMode
{
    CPB_ParaBar,                                 // 默认的 GridLayout 布局，用途为 draw bar 的取色器
    CPB_HighLight                                // QHBoxLayout 的水平布局，为活动色选择
};

class XLabel;
class QLayout;
class ColorParaBar : public QWidget
{
    Q_OBJECT
public:
    ColorParaBar(ColorParaBarMode mode = ColorParaBarMode::CPB_ParaBar, Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~ColorParaBar() = default;

    void setOrientations(Qt::Orientations orien = Qt::Horizontal);
    QColor setCurColor(const QColor col);
    const QColor getCurColor();

signals:
    void sigColorChange(const QColor&);

private:
    void onUpdateSel(const QColor& col);

public:
    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_scal;
    Qt::Orientations m_orien;
    QLayout* m_layout;
    QMap<QString, QString> m_labMap;
    XLabel* m_curXLab;
    QColor m_curCol;
};

#endif // COLORPARABAR_H
