// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "about.h"
#include "ui_about.h"
#include <QPointer>
#include "licenseui.h"
#include "communication.h"


About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    initUI();
}

About::~About()
{
    delete ui;
}

void About::initUI()
{
    onLanguageChange("");


    connect(&COMM, &Communication::sigLanguageChange, this, [this]() {
        ui->retranslateUi(this);
        onLanguageChange("");
    });
}

void About::on_btnLicenses_released()
{
    static QPointer<LicenseUI> licenseUI = nullptr;
    if (!licenseUI) {
        licenseUI = new LicenseUI(nullptr);
        if (!licenseUI->isVisible()) licenseUI->show();
    }
}

void About::onLanguageChange(const QString qmName)
{
    const QString& version = tr("Version") + QString(" %1 %2 (%3)").arg(XPROJECT_VERSION).arg(XCOMPILER_ID).arg(XBUILD_TIME);
    ui->labVersion->setText(version);
}


void About::on_tbnBuildInfo_released()
{
    COMM.showBuildInfoWidget();
}

