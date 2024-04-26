// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XMAGNIFYINGGLASS_H
#define XMAGNIFYINGGLASS_H

#include <QWidget>
#include <QColor>

namespace Ui {
class XMagnifyingGlass;
}

// 取色器 + 放大镜
class XMagnifyingGlass : public QWidget
{
    Q_OBJECT

public:
    explicit XMagnifyingGlass(QWidget *parent = nullptr);
    ~XMagnifyingGlass();
    void setPixmap(const QPoint& pt);   // MagnifyingGlass

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    Ui::XMagnifyingGlass *ui;
    QColor m_color;     // 取色器中的颜色
};

#endif // XMAGNIFYINGGLASS_H
