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
    , m_scaleFactor(1.0)
{
    ui->setupUi(this);

    auto& lable = ui->label;
    lable->setBackgroundRole(QPalette::Base);
    // lable->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);   // 会扩展拉伸
    lable->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);   // 会扩展拉伸
    lable->setAlignment(Qt::AlignCenter);

    auto& scrollArea = ui->scrollArea;
//    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVisible(true);

    // 设置Splitter的大小比例
    const auto& width =  rect().width() - ui->mainLayout->margin() * 2; // ui->splitter->width();
    QList<int> sizes;
    sizes <<  width * 0.75 << width * 0.25;
    ui->splitter->setSizes(sizes);

//    ui->splitter->setStretchFactor(0, 1);
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus),  this, [this](){onZoomIn();});
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Equal),  this, [this](){onZoomIn();});
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Underscore),  this, [this](){onZoomOut();});
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus),  this, [this](){onZoomOut();});          // QKeySequence::ZoomIn  ZoomOut
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N),  this, [this](){onNormalSize();});
}

XOcrDlg::~XOcrDlg()
{
    delete ui;
}

void XOcrDlg::setLeftPixmap(const QPixmap &pix)
{
    if (pix.isNull()) return;
    ui->label->setPixmap(pix);

   // ui->label->setPixmap(QPixmap("C:/Users/Venn/Pictures/2023.10.29_224643.png_proc.jpg"));
    m_scaleFactor = 1.0;
    ui->label->adjustSize();
    updateTitleText();
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


void XOcrDlg::onZoomIn()
{
    scaleImage(1.25);
}

void XOcrDlg::onZoomOut()
{
    scaleImage(0.75);
}

void XOcrDlg::onNormalSize()
{
    ui->label->adjustSize();
    m_scaleFactor = 1.0;
    pixmapMoveCenter();
}

void XOcrDlg::pixmapMoveCenter()
{
    auto& label = ui->label;
    const auto& size = label->size();
    label->setScaledContents(true);

    const auto& p1 = ui->scrollArea->rect().center();
    label->move(p1 - QPoint(size.width() / 2, size.height() / 2));
    updateTitleText();
}

void XOcrDlg::updateTitleText()
{
    const auto& text = QString(tr("original [%1 * %2]   current [%3 * %4]   zoom: %5%"))
                           .arg(ui->label->pixmap()->rect().width()).arg(ui->label->pixmap()->rect().height())
                           .arg(ui->label->rect().width()).arg(ui->label->rect().height())
                           .arg(int(m_scaleFactor * 100));
    setWindowTitle(text);
}

void XOcrDlg::scaleImage(double factor)
{
    m_scaleFactor *= factor;
    ui->label->resize(m_scaleFactor * ui->label->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);

    pixmapMoveCenter();
}

void XOcrDlg::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}

// 滚轮缩放此图
void XOcrDlg::wheelEvent(QWheelEvent *e)
{
    QPoint degrees = e->angleDelta() / 8;
    if (degrees.isNull()) return;
    QPoint step = degrees / 15;

    QWidget *childWidget = childAt(e->pos());
    // qDebug() << "wheelEvent childWidget:" << childWidget << e->pos();
    if (childWidget && (childWidget == ui->scrollAreaWidgetContents || childWidget == ui->label))
       step.y() > 0 ? onZoomIn() : onZoomOut();

    e->ignore();
}


