// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "interface.h"
#include "ui_interface.h"
#include <map>
#include "communication.h"
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

std::map<QString, QString> orientationMaps()
{
    std::map<QString, QString> orie = {  { "Horizontal", QObject::tr("Horizontal")}
                                       , { "Vertical" , QObject::tr("Vertical")}};


    return orie;
}

void Interface::initUI()
{
    std::map<QString, StyleType> styles = {  {"Sunny", StyleType::ST_sunny}
                                           , {"MacOS", StyleType::ST_macos}
                                           , {"DDE", StyleType::ST_dde}};

    const auto& currStyle = CJ_GET_QSTR("interface.style");
    for (const auto& it : styles) ui->cbbStyle->addItem(it.first, QVariant::fromValue(it.second));
    ui->cbbStyle->setCurrentText(currStyle);
    onLanguageChange("");
    ui->cpHighlight->setCurrPickedColor(CJ_GET_QSTR("interface.highlight"));
    ui->cpCrosshair->setCurrPickedColor(CJ_GET_QSTR("interface.crosshair"));
    ui->sbBorderWidth->setValue(CJ_GET("interface.border_width"));
    ui->sbCrosshairWidth->setValue(CJ_GET("interface.crosshair_width"));

    ui->gbCustomSizeEnable->setChecked(CJ_GET("interface.custom_size_enable"));
    ui->cbTopLeft->setChecked(CJ_GET("interface.topleft_enable"));
    ui->cbSize->setChecked(CJ_GET("interface.size_enable"));
    ui->cbDelay->setChecked(CJ_GET("interface.delay_enable"));
    ui->sbLeft->setValue(CJ_GET("interface.custom_rect_left"));
    ui->sbTop->setValue(CJ_GET("interface.custom_rect_top"));
    ui->sbWidth->setValue(CJ_GET("interface.custom_rect_width"));
    ui->sbHeight->setValue(CJ_GET("interface.custom_rect_height"));
    ui->dsbDelay->setValue(CJ_GET("interface.custom_dealy").get<double>());

    ui->cbAcrylicEffect->setChecked(CJ_GET("interface.acrylic_effect"));
    ui->cbAutoDetectWindows->setChecked(CJ_GET("interface.auto_detect_windows"));
    ui->cbAutoCopyToClipboard->setChecked(CJ_GET("interface.auto_copy_to_clipbaoard"));
    ui->cbCrosshairShow->setChecked(CJ_GET("interface.crosshair_show"));


    connect(ui->cpHighlight, &ColorPicker::sigPickedColor, this, &Interface::onHighlightPickedColor);
    connect(ui->cpCrosshair, &ColorPicker::sigPickedColor, this, &Interface::onCrosshairPickedColor);
    connect(&COMM, &Communication::sigLanguageChange, this, [this]() {
        ui->retranslateUi(this);
        onLanguageChange("");
    });
}

void Interface::on_cbbStyle_currentTextChanged(const QString &arg1)
{
    ui->sbBorderWidth->setEnabled(arg1 != "MacOS");
    CJ_SET("interface.style", arg1.toStdString());
}

void Interface::onHighlightPickedColor(const QColor &color)
{
    CJ_SET("interface.highlight", color.name(QColor::HexArgb).toStdString());
}

void Interface::onCrosshairPickedColor(const QColor &color)
{
    CJ_SET("interface.crosshair", color.name(QColor::HexArgb).toStdString());
}

void Interface::on_sbBorderWidth_valueChanged(int arg1)
{
    CJ_SET("interface.border_width", arg1);
}

void Interface::on_sbCrosshairWidth_valueChanged(int arg1)
{
    CJ_SET("interface.crosshair_width", arg1);
}

void Interface::on_cbAcrylicEffect_clicked(bool checked)
{
    CJ_SET("interface.acrylic_effect", checked);
}

void Interface::on_cbAutoDetectWindows_clicked(bool checked)
{
    CJ_SET("interface.auto_detect_windows", checked);
}

void Interface::on_cbAutoCopyToClipboard_clicked(bool checked)
{
    CJ_SET("interface.auto_copy_to_clipbaoard", checked);
}

void Interface::on_cbCrosshairShow_clicked(bool checked)
{
    CJ_SET("interface.crosshair_show", checked);
}

void Interface::onBtnResetClicked(bool checked)
{
    const ordered_json& j = CJ.defaultConfigJson();
    CJ.setJ("interface", j["interface"]);

    ui->cbbStyle->setCurrentText(CJ_GET_QSTR("interface.style"));
    ui->cbbOrientation->setCurrentText(CJ_GET_QSTR("interface.orientation"));
    ui->cpHighlight->setCurrPickedColor(CJ_GET_QSTR("interface.highlight"));
    ui->cpCrosshair->setCurrPickedColor(CJ_GET_QSTR("interface.crosshair"));
    ui->sbBorderWidth->setValue(CJ_GET("interface.border_width"));
    ui->sbCrosshairWidth->setValue(CJ_GET("interface.crosshair_width"));

    ui->gbCustomSizeEnable->setChecked(CJ_GET("interface.custom_size_enable").get<bool>());
    ui->cbTopLeft->setChecked(CJ_GET("interface.topleft_enable").get<bool>());
    ui->cbSize->setChecked(CJ_GET("interface.size_enable").get<bool>());
    ui->cbDelay->setChecked(CJ_GET("interface.delay_enable").get<bool>());
    ui->sbLeft->setValue(CJ_GET("interface.custom_rect_left"));
    ui->sbTop->setValue(CJ_GET("interface.custom_rect_top"));
    ui->sbWidth->setValue(CJ_GET("interface.custom_rect_width"));
    ui->sbHeight->setValue(CJ_GET("interface.custom_rect_height"));
    ui->dsbDelay->setValue(CJ_GET("interface.custom_dealy").get<double>());

    ui->cbAcrylicEffect->setChecked(CJ_GET("interface.acrylic_effect").get<bool>());
    ui->cbAutoDetectWindows->setChecked(CJ_GET("interface.auto_detect_windows").get<bool>());
    ui->cbAutoCopyToClipboard->setChecked(CJ_GET("interface.auto_copy_to_clipbaoard").get<bool>());
    ui->cbCrosshairShow->setChecked(CJ_GET("interface.crosshair_show").get<bool>());
}

void Interface::onLanguageChange(const QString qmName)
{
    Q_UNUSED(qmName)
    const auto orie = orientationMaps();
    const auto& currOrie = CJ_GET_QSTR("interface.orientation");
    ui->cbbOrientation->clear();
    for (const auto& it : orie) ui->cbbOrientation->addItem(it.second, it.first);
    for (const auto& it : orie) {
        if (it.first == currOrie) {
            ui->cbbOrientation->setCurrentText(it.second);
            break;
        }
    }
}


void Interface::on_gbCustomSizeEnable_clicked(bool checked)
{
    CJ_SET("interface.custom_size_enable", checked);
}

void Interface::on_cbTopLeft_clicked(bool checked)
{
    CJ_SET("interface.topleft_enable", checked);
}

void Interface::on_cbSize_clicked(bool checked)
{
    CJ_SET("interface.size_enable", checked);
}

void Interface::on_cbDelay_clicked(bool checked)
{
    CJ_SET("interface.delay_enable", checked);
}

void Interface::on_sbLeft_valueChanged(int arg1)
{
    CJ_SET("interface.custom_rect_left", arg1);
}

void Interface::on_sbTop_valueChanged(int arg1)
{
    CJ_SET("interface.custom_rect_top", arg1);
}

void Interface::on_sbWidth_valueChanged(int arg1)
{
    CJ_SET("interface.custom_rect_width", arg1);
}


void Interface::on_sbHeight_valueChanged(int arg1)
{
    CJ_SET("interface.custom_rect_height", arg1);
}

void Interface::on_dsbDelay_valueChanged(double arg1)
{
    CJ_SET("interface.custom_dealy", arg1);
}

void Interface::on_cbbOrientation_currentTextChanged(const QString &arg1)
{
    for (const auto& it : orientationMaps()) {
        if (it.second == arg1) {
            CJ_SET("interface.orientation", it.first.toStdString());
            break;
        }
    }
}

