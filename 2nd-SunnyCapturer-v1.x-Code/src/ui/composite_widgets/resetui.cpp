// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "resetui.h"
#include "ui_resetui.h"
#include "communication.h"

ResetUI::ResetUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResetUI)
{
    ui->setupUi(this);

    connect(ui->btnHint, &QToolButton::clicked, this, &ResetUI::sigBtnHintClicked);
    connect(ui->btnReset, &QToolButton::clicked, this, &ResetUI::sigBtnResetClicked);
    connect(&COMM, &Communication::sigLanguageChange, this, [this]() { ui->retranslateUi(this);});
}

ResetUI::~ResetUI()
{
    delete ui;
}
