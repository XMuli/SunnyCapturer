#include "interface.h"
#include "ui_interface.h"
#include <map>
#include "../../data/configmanager.h"

Interface::Interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Interface)
{
    ui->setupUi(this);
    initUI();
}

Interface::~Interface()
{
    delete ui;
}

void Interface::initUI()
{
    std::map<QString, StyleType> styles = {  {"Sunny", StyleType::ST_sunny}
                                           , {"MacOS", StyleType::ST_macos}
                                           , {"DDE", StyleType::ST_dde}};

    const auto& currStyle = CONF_MANAGE.property("XInterface_style").toString();
    for (const auto& it : styles)
        ui->cbbStyle->addItem(it.first, QVariant::fromValue(it.second));

    ui->cbbStyle->setCurrentText(currStyle);
    ui->cpHighlight->setCurrPickedColor(CONF_MANAGE.property("XInterface_highlight").toString());
    ui->cpCrosshair->setCurrPickedColor(CONF_MANAGE.property("XInterface_crosshair").toString());
    ui->sbBorderWidth->setValue(CONF_MANAGE.property("XInterface_border_width").toInt());
    ui->sbCrosshairWidth->setValue(CONF_MANAGE.property("XInterface_crosshair_width").toInt());
    ui->cbAcrylicEffect->setChecked(CONF_MANAGE.property("XInterface_acrylic_effect").toBool());
    ui->cbAutoDetectWindows->setChecked(CONF_MANAGE.property("XInterface_auto_detect_windows").toBool());
    ui->cbAutoCopyToClipboard->setChecked(CONF_MANAGE.property("XInterface_auto_copy_to_clipbaoard").toBool());
    ui->cbCrosshairShow->setChecked(CONF_MANAGE.property("XInterface_crosshair_show").toBool());

    connect(ui->cpHighlight, &ColorPicker::sigPickedColor, this, &Interface::onHighlightPickedColor);
    connect(ui->cpCrosshair, &ColorPicker::sigPickedColor, this, &Interface::onCrosshairPickedColor);
}

void Interface::on_cbbStyle_currentTextChanged(const QString &arg1)
{
    ui->sbBorderWidth->setEnabled(arg1 != "MacOS");
    CONF_MANAGE.setProperty("XInterface_style", arg1);
}

void Interface::onHighlightPickedColor(const QColor &color)
{
    CONF_MANAGE.setProperty("XInterface_highlight", color.name());
}

void Interface::onCrosshairPickedColor(const QColor &color)
{
    CONF_MANAGE.setProperty("XInterface_crosshair", color.name());
}


void Interface::on_sbBorderWidth_valueChanged(int arg1)
{
    CONF_MANAGE.setProperty("XInterface_border_width", arg1);
}


void Interface::on_sbCrosshairWidth_valueChanged(int arg1)
{
    CONF_MANAGE.setProperty("XInterface_crosshair_width", arg1);
}

void Interface::on_cbAcrylicEffect_clicked(bool checked)
{
    CONF_MANAGE.setProperty("XInterface_acrylic_effect", checked);
}


void Interface::on_cbAutoDetectWindows_clicked(bool checked)
{
    CONF_MANAGE.setProperty("XInterface_auto_detect_windows", checked);
}


void Interface::on_cbAutoCopyToClipboard_clicked(bool checked)
{
    CONF_MANAGE.setProperty("XInterface_auto_copy_to_clipbaoard", checked);
}


void Interface::on_cbCrosshairShow_clicked(bool checked)
{
    CONF_MANAGE.setProperty("XInterface_crosshair_show", checked);
}

void Interface::onBtnResetClicked(bool checked)
{
    ui->cbbStyle->setCurrentText("Sunny");
    ui->cpHighlight->setCurrPickedColor("#387bfd");
    ui->cpCrosshair->setCurrPickedColor("#d13840");
    ui->sbBorderWidth->setValue(3);
    ui->sbCrosshairWidth->setValue(2);
    ui->cbAutoDetectWindows->setChecked(true);
    ui->cbAutoCopyToClipboard->setChecked(true);
    ui->cbCrosshairShow->setChecked(false);
}

