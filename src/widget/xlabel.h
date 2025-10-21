// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XLABEL_H
#define XLABEL_H

#include <QLabel>
#include <QColor>
#include <QToolButton>

class QWidget;
class QString;
class QWidget;
class QPainter;
class QMouseEvent;

class XLabel : public QToolButton
{
    Q_OBJECT
public:
    explicit XLabel(QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());
    virtual ~XLabel() = default;

    void setChecked(const bool& checked = false);
    void setColor(QColor c, double alpha = 1);
    void setRainbow(bool bRainbow = false);
private:
    void initUI();
    void setConicalGradientColor(QPainter &pa);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor m_color;
    double m_alpha;
    bool   m_bChecked;     // 当前被选中否？
    bool   m_bRainbow;      // false:纯色   true:彩色
};

Q_DECLARE_METATYPE(XLabel*); // 提供给 QVariant 使用; // 另外自定义类型中一定要有默认构造函数

#endif // XLABEL_H
