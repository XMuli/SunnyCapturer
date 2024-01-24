// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "verspacerline.h"

VerSpacerLine::VerSpacerLine(int length, QWidget *parent, Qt::WindowFlags f)
{
    setFrameShape(VLine);
    setFrameShadow(Sunken);
    setLineWidth(0);
    setMidLineWidth(2);
    setFixedHeight(length);
}
