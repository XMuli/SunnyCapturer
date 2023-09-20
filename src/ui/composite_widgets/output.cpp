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
    const auto& fileName = CONF_MANAGE.property("XOutput_flie_name").toString();
    const auto& configPath= CONF_MANAGE.property("XOutput_config_path").toString();
    const auto& quickSavePath = CONF_MANAGE.property("XOutput_quick_save_path").toString();
    const auto& autoSavePath = CONF_MANAGE.property("XOutput_auto_save_path").toString();

    ui->sbImageQuailty->setValue(CONF_MANAGE.property("XOutput_image_quailty").toInt());
    ui->leFileName->setText(fileName);
    ui->leConfigPath->setText(configPath);
    ui->cbQuickSaveEnable->setChecked(CONF_MANAGE.property("XOutput_quick_save_enable").toBool());
    ui->leQuickSavePath->setText(quickSavePath);
    ui->cbAutoSaveEnable->setChecked(CONF_MANAGE.property("XOutput_auto_save_enable").toBool());
    ui->leAutoSavePath->setText(autoSavePath);

    const auto& prewview = tr("Preview: ");
    ui->leFileName->setToolTip(prewview + formatToFileName(fileName));
    ui->leQuickSavePath->setToolTip(prewview + formatToFileName(quickSavePath + "/" + fileName));
    ui->leAutoSavePath->setToolTip(prewview + formatToFileName(autoSavePath + "/" + fileName));

    connect(ui->cbQuickSaveEnable, &QGroupBox::clicked, this, [](bool checked = false){ CONF_MANAGE.setProperty("XOutput_quick_save_enable", checked);});
    connect(ui->cbAutoSaveEnable, &QGroupBox::clicked, this, [](bool checked = false){ CONF_MANAGE.setProperty("XOutput_auto_save_enable", checked);});
    connect(ui->btnConfigOpen, &QPushButton::released, this, &Output::onSleletedDir);
    connect(ui->btnQuickSaveSelect, &QPushButton::released, this, &Output::onSleletedDir);
    connect(ui->btnAutoSaveSelect, &QPushButton::released, this, &Output::onSleletedDir);
    connect(&COMM, &Communication::sigLanguageChange, this, [this]() { ui->retranslateUi(this);});
}

void Output::on_sbImageQuailty_valueChanged(int arg1)
{
    CONF_MANAGE.setProperty("XOutput_image_quailty", arg1);
}


void Output::on_leFileName_textChanged(const QString &arg1)
{
    CONF_MANAGE.setProperty("XOutput_flie_name", arg1);
}


void Output::on_leConfigPath_textChanged(const QString &arg1)
{
    CONF_MANAGE.setProperty("XOutput_config_path", arg1);
}

void Output::on_leQuickSavePath_textChanged(const QString &arg1)
{
    CONF_MANAGE.setProperty("XOutput_quick_save_path", arg1);
}

void Output::on_leAutoSavePath_textChanged(const QString &arg1)
{
    CONF_MANAGE.setProperty("XOutput_auto_save_path", arg1);
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
    ui->sbImageQuailty->setValue(-1);
    ui->leFileName->setText(QString("%1_$yyyyMMdd_hhmmss$.png").arg(XPROJECT_NAME));
    ui->leConfigPath->setText(qApp->applicationDirPath());
    ui->cbQuickSaveEnable->setChecked(false);
    ui->leQuickSavePath->setText(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first());
    ui->cbAutoSaveEnable->setChecked(false);
    ui->leAutoSavePath->setText(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first());
}

