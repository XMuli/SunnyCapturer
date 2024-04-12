// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "pin.h"
#include "ui_pin.h"
#include "communication.h"

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
    CJ_SET("pin.opacity", arg1);
}


void Pin::on_sbMaxSize_valueChanged(int arg1)
{
    CJ_SET("pin.maximum_size", arg1);
}

void Pin::onBtnResetClicked(bool checked)
{
    ui->sbOpacity->setValue(100);
    ui->sbMaxSize->setValue(10000);
}

void Pin::initUI()
{
    ui->sbOpacity->setValue(CJ_GET("pin.opacity").get<int>());
    ui->sbMaxSize->setValue(CJ_GET("pin.maximum_size").get<int>());
    connect(&COMM, &Communication::sigLanguageChange, this, [this]() { ui->retranslateUi(this);});
}
