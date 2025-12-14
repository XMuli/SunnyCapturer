// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "output.h"
#include "ui_output.h"

#include <QDesktopServices>
#include <QStandardPaths>
#include <QFileDialog>
#include <QUrl>
#include "communication.h"
#include "../../data/configmanager.h"
#include "../paint_bar/toolbar_level/paintbarhelper.h"

Output::Output(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Output)
{
    ui->setupUi(this);
    initUI();
}

Output::~Output()
{
    delete ui;
}

void Output::initUI()
{
    ui->cbQuickSaveEnable->setChecked(CJ_GET("output.quick_save_enable").get<bool>());
    ui->cbAutoSaveEnable->setChecked(CJ_GET("output.auto_save_enable").get<bool>());
    onLanguageChange("");

    connect(ui->cbQuickSaveEnable, &QGroupBox::clicked, this, [](bool checked = false){ CJ_SET("output.quick_save_enable", checked);});
    connect(ui->cbAutoSaveEnable, &QGroupBox::clicked, this, [](bool checked = false){ CJ_SET("output.auto_save_enable", checked);});
    connect(ui->btnConfigOpen, &QPushButton::released, this, &Output::onSleletedDir);
    connect(ui->btnQuickSaveSelect, &QPushButton::released, this, &Output::onSleletedDir);
    connect(ui->btnAutoSaveSelect, &QPushButton::released, this, &Output::onSleletedDir);
    connect(&COMM, &Communication::sigLanguageChange, this, [this]() {
        ui->retranslateUi(this);
        onLanguageChange("");
    });
}

void Output::on_sbImageQuailty_valueChanged(int arg1)
{
    CJ_SET("output.image_quailty", arg1);
}

void Output::on_leFileName_textChanged(const QString &arg1)
{
    const auto& prewview = tr("Preview: ");
    const auto& tFileName = formatToFileName(arg1);
    ui->leFileName->setToolTip(prewview + tFileName);
    CJ_SET("output.flie_name", arg1.toStdString());
}

void Output::on_leConfigPath_textChanged(const QString &arg1)
{
    CJ_SET("output.config_path", arg1.toStdString());
}

void Output::on_leQuickSavePath_textChanged(const QString &arg1)
{
    CJ_SET("output.quick_save_path", arg1.toStdString());
}

void Output::on_leAutoSavePath_textChanged(const QString &arg1)
{
    CJ_SET("output.auto_save_path", arg1.toStdString());
}

void Output::onSleletedDir()
{
    auto btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString selDir;
    QString defDir;
    const QString title(tr("Select a path"));
    if (btn == ui->btnConfigOpen) {

        defDir = ui->leConfigPath->text();
        QDesktopServices::openUrl(QUrl::fromLocalFile(defDir));  // 调用系统资源管理器以打开指定路径

    } else if (btn == ui->btnQuickSaveSelect) {

        defDir = ui->leQuickSavePath->text();
        selDir = QFileDialog::getExistingDirectory(this, title, defDir, QFileDialog::ShowDirsOnly);
        if (!selDir.isEmpty() && defDir != selDir)
            ui->leQuickSavePath->setText(selDir);

    } else if (btn == ui->btnAutoSaveSelect) {
        defDir = ui->leAutoSavePath->text();
        selDir = QFileDialog::getExistingDirectory(this, title, defDir, QFileDialog::ShowDirsOnly);

        if (!selDir.isEmpty() && defDir != selDir)
            ui->leAutoSavePath->setText(selDir);
    } else {
        qDebug() << "btn is not bullptr, but does not any know select dit btn!";
    }
}


void Output::onBtnResetClicked(bool checked)
{
    const ordered_json& j = CJ.defaultConfigJson();
    CJ.setJ("output", j["output"]);
    CJ.initOutputDefaulValue();

    ui->sbImageQuailty->setValue(CJ_GET("output.image_quailty"));
    ui->leFileName->setText(CJ_GET_QSTR("output.flie_name"));
    ui->leConfigPath->setText(CJ_GET_QSTR("output.config_path"));
    ui->cbQuickSaveEnable->setChecked(CJ_GET("output.quick_save_enable").get<bool>());
    ui->leQuickSavePath->setText(CJ_GET_QSTR("output.quick_save_path"));
    ui->cbAutoSaveEnable->setChecked(CJ_GET("output.auto_save_enable").get<bool>());
    ui->leAutoSavePath->setText(CJ_GET_QSTR("output.auto_save_path"));
}

void Output::onLanguageChange(const QString qmName)
{
    const auto& fileName = CJ_GET_QSTR("output.flie_name");
    const auto& configPath= CJ_GET_QSTR("output.config_path");
    const auto& quickSavePath = CJ_GET_QSTR("output.quick_save_path");
    const auto& autoSavePath = CJ_GET_QSTR("output.auto_save_path");

    ui->sbImageQuailty->setValue(CJ_GET("output.image_quailty").get<int>());
    ui->leFileName->setText(fileName);
    ui->leConfigPath->setText(configPath);
    ui->leQuickSavePath->setText(quickSavePath);
    ui->leAutoSavePath->setText(autoSavePath);

    const auto& prewview = tr("Preview: ");
    const auto& tFileName = formatToFileName(fileName);
    ui->leFileName->setToolTip(prewview + tFileName);
    ui->leQuickSavePath->setToolTip(prewview + formatToFileName(quickSavePath + "/" + fileName));
    ui->leAutoSavePath->setToolTip(prewview + formatToFileName(autoSavePath + "/" + fileName));
}

