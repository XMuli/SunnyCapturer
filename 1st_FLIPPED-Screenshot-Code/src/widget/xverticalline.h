// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>
#ifndef XVERTICALLINE_H
#define XVERTICALLINE_H

#include <QFrame>

class XVerticalLine : public QFrame
{
    Q_OBJECT
public:
    XVerticalLine(int height, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif // XVERTICALLINE_H
