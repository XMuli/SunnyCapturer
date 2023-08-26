#include "abssettingui.h"
#include "ui_abssettingui.h"

AbsSettingUI::AbsSettingUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbsSettingUI)
{
    ui->setupUi(this);
}

AbsSettingUI::~AbsSettingUI()
{
    delete ui;
}

void AbsSettingUI::insertWidget(int index, QWidget *widget, int stretch, Qt::Alignment alignment)
{
    if (widget) ui->vLayout->insertWidget(index, widget, stretch, alignment);
}
