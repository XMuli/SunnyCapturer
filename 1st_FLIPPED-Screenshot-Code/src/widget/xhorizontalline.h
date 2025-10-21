// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XHORIZONTALLINE_H
#define XHORIZONTALLINE_H

#include <QFrame>

// 水平分割线
class XHorizontalLine : public QFrame
{
    Q_OBJECT
public:
    XHorizontalLine(int width, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif // XHORIZONTALLINE_H
