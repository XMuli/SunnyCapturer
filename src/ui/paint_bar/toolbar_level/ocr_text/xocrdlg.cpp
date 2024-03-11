// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "xocrdlg.h"
#include "ui_xocrdlg.h"

XOcrDlg::XOcrDlg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XOcrDlg)
{
    ui->setupUi(this);

    auto& splitter = ui->splitter;
    // 设置Splitter的大小比例
    QList<int> sizes;
    sizes << 4 * splitter->width() / 5 << splitter->width() / 5;
    ui->splitter->setSizes(sizes);

    ui->splitter->setStretchFactor(0, 4);
    ui->splitter->setStretchFactor(1, 1);
}

XOcrDlg::~XOcrDlg()
{
    delete ui;
}

void XOcrDlg::setLeftPixmap(const QPixmap &pix)
{
    if (pix.isNull()) return;
    ui->label->setPixmap(pix);
}

void XOcrDlg::setRightText(const QString &text)
{
    if (text.isEmpty()) return;
    ui->textEdit->setText(text);
}

void XOcrDlg::appendRightText(const QString &text)
{
    ui->textEdit->append(text);
}
