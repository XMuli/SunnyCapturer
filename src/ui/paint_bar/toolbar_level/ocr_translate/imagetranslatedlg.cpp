#include "imagetranslatedlg.h"
#include "ui_imagetranslatedlg.h"

ImageTranslateDlg::ImageTranslateDlg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImageTranslateDlg)
{
    ui->setupUi(this);
}

ImageTranslateDlg::~ImageTranslateDlg()
{
    delete ui;
}

void ImageTranslateDlg::setLeftPixmap(const QPixmap &pix)
{
    if (!pix.isNull())
        ui->labLeft->setPixmap(pix);
}

void ImageTranslateDlg::setRightPixmap(const QSize &size, const QString &path)
{
    if (size.isEmpty())
        ui->labRight->setText(path);
    else
        ui->labRight->setPixmap(QPixmap(path));

}
