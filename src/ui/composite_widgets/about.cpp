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
    const QString& project = QString("<html><head/><body><p><span style=\" font-size:20pt;\">%1</span><span style=\" font-size:9pt; font-weight:400;\">(%2-bit)</span></p></body></html>")
                          .arg(XPROJECT_NAME).arg(XARCH_BIT);

    ui->btnProfessional->hide(); // 暂时隐藏
    ui->labProject->setText(project);
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

