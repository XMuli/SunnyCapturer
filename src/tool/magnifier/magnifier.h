// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>
#ifndef MAGNIFIER_H
#define MAGNIFIER_H

#include <QWidget>

class Magnifier : public QWidget
{
    Q_OBJECT
public:
    explicit Magnifier(QWidget* parent = nullptr);
    ~Magnifier() = default;
};

#endif // MAGNIFIER_H
