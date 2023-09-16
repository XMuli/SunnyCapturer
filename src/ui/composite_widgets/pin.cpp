#include "pin.h"
#include "ui_pin.h"
#include "../../data/configmanager.h"

Pin::Pin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pin)
{
    ui->setupUi(this);
    initUI();
}

Pin::~Pin()
{
    delete ui;
}

void Pin::on_sbOpacity_valueChanged(int arg1)
{
    CONF_MANAGE.setProperty("XPin_opacity", arg1);
}


void Pin::on_sbMaxSize_valueChanged(int arg1)
{
    CONF_MANAGE.setProperty("XPin_maximum_size", arg1);
}

void Pin::onBtnResetClicked(bool checked)
{
    ui->sbOpacity->setValue(100);
    ui->sbMaxSize->setValue(10000);
}

void Pin::initUI()
{
    ui->sbOpacity->setValue(CONF_MANAGE.property("XPin_opacity").toInt());
    ui->sbMaxSize->setValue(CONF_MANAGE.property("XPin_maximum_size").toInt());
}
