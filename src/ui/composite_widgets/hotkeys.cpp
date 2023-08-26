#include "hotkeys.h"
#include "ui_hotkeys.h"

Hotkeys::Hotkeys(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hotkeys)
{
    ui->setupUi(this);
}

Hotkeys::~Hotkeys()
{
    delete ui;
}
