#include "paste.h"
#include "ui_paste.h"

Paste::Paste(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paste)
{
    ui->setupUi(this);
}

Paste::~Paste()
{
    delete ui;
}
