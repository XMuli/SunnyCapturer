#include "pin.h"
#include "ui_pin.h"

Pin::Pin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pin)
{
    ui->setupUi(this);
}

Pin::~Pin()
{
    delete ui;
}
