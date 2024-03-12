#include "ximageviewer.h"
#include "ui_ximageviewer.h"

#include <QShortcut>
#include <QKeySequence>

XImageViewer::XImageViewer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XImageViewer)
    , m_scaleFactor(1.0)
{
    ui->setupUi(this);
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus),  this, [this](){zoomIn();});
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Equal),  this, [this](){zoomIn();});
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Underscore),  this, [this](){zoomOut();});
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus),  this, [this](){zoomOut();});
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N),  this, [this](){normalSize();});
}

XImageViewer::~XImageViewer()
{
    delete ui;
}

void XImageViewer::setLeftPixmap(const QPixmap &pix)
{
    ui->imgDlg->setPixmap(pix);
}

void XImageViewer::setRightText(const QString &text)
{
    ui->textEdit->setText(text);
}

void XImageViewer::appendRightText(const QString &text)
{
    ui->textEdit->append(text);
}

void XImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void XImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void XImageViewer::normalSize()
{
    ui->imgDlg->label()->adjustSize();

}

void XImageViewer::scaleImage(double factor)
{
    QLabel* label = ui->imgDlg->label();
    QScrollArea* scrollArea = ui->imgDlg->scrollArea();
    m_scaleFactor *= factor;
    label->resize(m_scaleFactor * label->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    // zoomInAct->setEnabled(m_scaleFactor < 3.0);
    // zoomOutAct->setEnabled(m_scaleFactor > 0.333);
}

void XImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
