#include "about.h"
#include "ui_about.h"
#include <QPointer>
#include "licenseui.h"

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
    const QString& version = QString("Version %1 %2 (%3)").arg(XPROJECT_VERSION).arg(XCOMPILER_ID).arg(XBUILD_TIME);
    ui->labProject->setText(project);
    ui->labVersion->setText(version);
}

void About::on_btnLicenses_released()
{
    static QPointer<LicenseUI> licenseUI = nullptr;
    if (!licenseUI) {
        licenseUI = new LicenseUI(nullptr);
        if (!licenseUI->isVisible()) licenseUI->show();
    }
}

