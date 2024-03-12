#include "ximagedlg.h"
#include "ui_ximagedlg.h"

XImageDlg::XImageDlg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XImageDlg)
{
    ui->setupUi(this);

    auto& lable = ui->label;
    lable->setBackgroundRole(QPalette::Base);
    lable->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);   // 会扩展拉伸
    //    lable->setAlignment(Qt::AlignCenter);
    lable->setScaledContents(true);
}

XImageDlg::~XImageDlg()
{
    delete ui;
}

void XImageDlg::setPixmap(const QPixmap &pix)
{
    if (pix.isNull()) return;
    ui->label->setPixmap(pix);
}

QLabel* XImageDlg::label()
{
    return ui->label;
}

QScrollArea *XImageDlg::scrollArea()
{
    return ui->scrollArea;
}
