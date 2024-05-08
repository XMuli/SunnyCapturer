#include "xocrwidget.h"
#include "ui_xocrwidget.h"
#include <QShortcut>

XOcrWidget::XOcrWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XOcrWidget)
{
    ui->setupUi(this);

    // 设置Splitter的大小比例
    const auto& width =  rect().width() - ui->horizontalLayout->margin() * 2; // ui->splitter->width();
    QList<int> sizes;
    sizes <<  width * 0.75 << width * 0.25;
    ui->splitter->setSizes(sizes);

    //    ui->splitter->setStretchFactor(0, 1);
    // new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus),  this, [this](){onZoomIn();});
    // new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Equal),  this, [this](){onZoomIn();});
    // new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Underscore),  this, [this](){onZoomOut();});
    // new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus),  this, [this](){onZoomOut();});          // QKeySequence::ZoomIn  ZoomOut
    // new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N),  this, [this](){onNormalSize();});
}

XOcrWidget::~XOcrWidget()
{
    delete ui;
}

void XOcrWidget::setLeftPixmap(const QPixmap &pix)
{
    if (pix.isNull()) return;
    ui->Image->setPixmap(pix);
}

void XOcrWidget::setRightText(const QString &text)
{
    if (text.isEmpty()) return;
    ui->textEdit->setText(text);
}

void XOcrWidget::appendRightText(const QString &text)
{
    // ui->textEdit->append(text);
    ui->textEdit->insertPlainText(text);
}
