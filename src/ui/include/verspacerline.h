// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef VERSPACERLINE_H
#define VERSPACERLINE_H

#include <QFrame>

class VerSpacerLine : public QFrame
{
    Q_OBJECT
public:
    VerSpacerLine(int length, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

};

#endif // VERSPACERLINE_H
