#include "xocrwidget.h"
#include "ui_xocrwidget.h"
#include <QShortcut>
#include "communication.h"

XOcrWidget::XOcrWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XOcrWidget)
{
    ui->setupUi(this);

    // 设置Splitter的大小比例
    const auto& width =  rect().width() - ui->horizontalLayout->margin() * 2; // ui->splitter->width();
    QList<int> sizes;
    const int& left = CJ_GET("advanced.non_ui_user_experience.ocr_splitter_left").get<int>();
    const int& right = CJ_GET("advanced.non_ui_user_experience.ocr_splitter_right").get<int>();
    const int all = left + right;
    sizes <<  width * double(1.0 * left/all) << width * double(1.0 * right/all);
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

void XOcrWidget::on_splitter_splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);

    const auto& splitter = ui->splitter;
    QList<int> sizes = splitter->sizes();

    int left = 3;
    int right = 1;
    if (sizes.count() == 2) {
        left = sizes[0];
        right = sizes[1];
    }

    CJ_SET("advanced.non_ui_user_experience.ocr_splitter_left", left);
    CJ_SET("advanced.non_ui_user_experience.ocr_splitter_right", right);
}

