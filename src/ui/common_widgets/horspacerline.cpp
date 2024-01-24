// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "horspacerline.h"

//frameRect:    QRect       //控件矩形
//frameWidth:   const int   //框架的宽度
//frameShadow:  Shadow      //有无阴影
//lineWidth:    int         //线宽度
//frameShape:   Shape       //框架形状
//midLineWidth: int         //中间线的宽度

HorSpacerLine::HorSpacerLine(int length, QWidget *parent, Qt::WindowFlags f)
{
    setFrameShape(HLine);
    setFrameShadow(Sunken);
    setLineWidth(0);
    setMidLineWidth(2);
    setFixedWidth(length);
}
