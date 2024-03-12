// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "xocrdlg.h"
#include "qdebug.h"
#include "ui_xocrdlg.h"


#include <QShortcut>

XOcrDlg::XOcrDlg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XOcrDlg)
{
    ui->setupUi(this);
    auto& lable = ui->label;
    lable->setBackgroundRole(QPalette::Base);
    lable->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);   // 会扩展拉伸
//    lable->setAlignment(Qt::AlignCenter);
    lable->setScaledContents(true);

    auto& scrollArea = ui->scrollArea;
//    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVisible(true);

//    ui->splitter->setStretchFactor(0, 4);
//    ui->splitter->setStretchFactor(0, 1);
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus),  this, [this](){zoomIn();});
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Equal),  this, [this](){zoomIn();});
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Underscore),  this, [this](){zoomOut();});
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus),  this, [this](){zoomOut();});
//    new QShortcut(QKeySequence::ZoomIn,  this, [this](){zoomIn();});
//    new QShortcut(QKeySequence::ZoomOut,  this, [this](){zoomOut();});
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N),  this, [this](){normalSize();});
}

XOcrDlg::~XOcrDlg()
{
    delete ui;
}

void XOcrDlg::setLeftPixmap(const QPixmap &pix)
{
    if (pix.isNull()) return;
    ui->label->setPixmap(pix);

//    ui->label->setPixmap(QPixmap("C:/Users/Venn/Pictures/111.jpg"));
    m_scaleFactor = 1.0;
//    ui->label->adjustSize();

    const auto& text = QString("pix: %1 * %2  label: %3 * %4")
                           .arg(pix.width()).arg(pix.height())
                           .arg(ui->label->rect().width()).arg(ui->label->rect().height());
    setWindowTitle(text);
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

void XOcrDlg::setOcrSize()
{
    // 设置Splitter的大小比例
    const auto& width =  rect().width() - ui->mainLayout->margin() * 2; // ui->splitter->width();
    QList<int> sizes;
    sizes <<  width * 0.75 << width * 0.25;

    ui->splitter->setSizes(sizes);
}

void XOcrDlg::zoomIn()
{
    scaleImage(1.25);
}

void XOcrDlg::zoomOut()
{
    scaleImage(0.8);
}

void XOcrDlg::normalSize()
{
    ui->label->adjustSize();
    m_scaleFactor = 1.0;
}

void XOcrDlg::scaleImage(double factor)
{
    m_scaleFactor *= factor;
    ui->label->resize(m_scaleFactor * ui->label->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);

//    zoomInAct->setEnabled(m_scaleFactor < 3.0);
//    zoomOutAct->setEnabled(m_scaleFactor > 0.333);

    const auto& text = QString("pix: %1 * %2  label: %3 * %4")
                           .arg(ui->label->pixmap()->rect().width()).arg(ui->label->pixmap()->rect().height())
                           .arg(ui->label->rect().width()).arg(ui->label->rect().height());
    setWindowTitle(text);
}

void XOcrDlg::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}
