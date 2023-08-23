#include "sidesettingui.h"
#include "ui_sidesettingui.h"

SideSettingUI::SideSettingUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideSettingUI)
{
    ui->setupUi(this);
}

SideSettingUI::~SideSettingUI()
{
    delete ui;
}
