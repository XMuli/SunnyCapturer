// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "xocrtextedit.h"

XOcrTextEdit::XOcrTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    initUI();
}

XOcrTextEdit::XOcrTextEdit(const QString &text, QWidget *parent)
    : QTextEdit(text, parent)
{
    initUI();
}

void XOcrTextEdit::initUI()
{
//    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::NoContextMenu);
//    setWindowFlags(Qt::FramelessWindowHint);
}
