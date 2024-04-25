// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XMAGNIFYINGGLASS_H
#define XMAGNIFYINGGLASS_H

#include <QWidget>

namespace Ui {
class XMagnifyingGlass;
}

// 取色器 + 放大镜
class XMagnifyingGlass : public QWidget
{
    Q_OBJECT

public:
    void setPixmap(const QPoint& pt);   // MagnifyingGlass
    explicit XMagnifyingGlass(QWidget *parent = nullptr);
    ~XMagnifyingGlass();

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    Ui::XMagnifyingGlass *ui;
};

#endif // XMAGNIFYINGGLASS_H
