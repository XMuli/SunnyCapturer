#include "resetui.h"
#include "ui_resetui.h"

ResetUI::ResetUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResetUI)
{
    ui->setupUi(this);
}

ResetUI::~ResetUI()
{
    delete ui;
}
