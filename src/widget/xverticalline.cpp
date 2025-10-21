// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/FlippedCode

#include "xverticalline.h"

//frameRect : QRect//控件矩形
//frameWidth : const int//框架的宽度
//frameShadow : Shadow//有无阴影
//lineWidth : int//线宽度
//frameShape : Shape//框架形状
//midLineWidth : int//中间线的宽度

XVerticalLine::XVerticalLine(int height, QWidget *parent, Qt::WindowFlags f)
    : QFrame(parent, f)
{
    setFrameShape(VLine);
    setFrameShadow(Sunken);
    setLineWidth(0);
    setMidLineWidth(2);
	setFixedHeight(height);
}
