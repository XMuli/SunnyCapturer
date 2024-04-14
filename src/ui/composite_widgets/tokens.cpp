// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "tokens.h"
#include "ui_tokens.h"
#include <QIcon>
#include "../../data/configjson.h"

Tokens::Tokens(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tokens)
{
    ui->setupUi(this);
    initUI();
}

Tokens::~Tokens()
{
    delete ui;
}

void Tokens::initUI()
{
    const QString& youdao_app_id = CJ.decryptString(CJ_GET_STR("tokens.youdao_app_id"));
    const QString& youdao_secret_key = CJ.decryptString(CJ_GET_STR("tokens.youdao_secret_key"));
    const QString& baidu_api_key = CJ.decryptString(CJ_GET_STR("tokens.baidu_api_key"));
    const QString& baidu_secret_key = CJ.decryptString(CJ_GET_STR("tokens.baidu_secret_key"));
    const QString& ocr_channel = CJ_GET_QSTR("tokens.ocr_channel");
    const QString& iamge_translate_channel = CJ_GET_QSTR("tokens.iamge_translate_channel");

    ui->leYDAppID->setText(youdao_app_id);
    ui->leYDApiSecret->setText(youdao_secret_key);
    ui->leBdApiKey->setText(baidu_api_key);
    ui->leBdSecretKey->setText(baidu_secret_key);

    const QStringList list = {"high", "high_location", "standard", "standard_location"};
    for (int i = 0; i < list.count(); ++i) {
        const QString& text = QString("%1 %2").arg(i).arg(list.at(i));
        ui->cbbOcr->addItem(text, list.at(i));
        ui->cbbOcr->setItemIcon(i, QIcon(":/resources/icons/setting/tokens/baidu.svg"));
    }
    ui->cbbOcr->setCurrentIndex(list.indexOf(ocr_channel));

    const QStringList imgTranslate = {"baidu", "youdao"};  /*"baidu_fanyi",*/
    for (int i = 0; i < imgTranslate.count(); ++i) {
        const QString& text = QString("%1 %2").arg(i).arg(imgTranslate.at(i));
        ui->cbbImgTranslate->addItem(text, imgTranslate.at(i));

        QString iconName = "baidu";
        if (text.contains("baidu")) iconName =  "baidu";
        else if (text.contains("youdao")) iconName =  "youdao";
        ui->cbbImgTranslate->setItemIcon(i, QIcon(":/resources/icons/setting/tokens/" + iconName + ".svg"));
    }
    ui->cbbImgTranslate->setCurrentIndex(imgTranslate.indexOf(iamge_translate_channel));
}

void Tokens::on_cbbOcr_currentIndexChanged(int index)
{
    const QString& channel = ui->cbbOcr->currentData().toString();
    CJ_SET("tokens.ocr_channel", channel.toStdString());
}

void Tokens::on_cbbImgTranslate_currentIndexChanged(int index)
{
    const QString& channel = ui->cbbImgTranslate->currentData().toString();
    CJ_SET("tokens.iamge_translate_channel", channel.toStdString());
}

void Tokens::on_leYDAppID_textChanged(const QString &arg1)
{
    CJ_SET("tokens.youdao_app_id", CJ.encryptString(arg1));
}

void Tokens::on_leYDApiSecret_textChanged(const QString &arg1)
{
    CJ_SET("tokens.youdao_secret_key", CJ.encryptString(arg1));

}

void Tokens::on_leBdApiKey_textChanged(const QString &arg1)
{
    CJ_SET("tokens.baidu_api_key", CJ.encryptString(arg1));
}

void Tokens::on_leBdSecretKey_textChanged(const QString &arg1)
{
    CJ_SET("tokens.baidu_secret_key", CJ.encryptString(arg1));
}

void Tokens::onBtnResetClicked(bool checked)
{
    Q_UNUSED(checked)
    const ordered_json& j = CJ.defaultConfigJson();
    CJ.setJ("tokens", j["tokens"]);

    const QString& ocr_channel = CJ_GET_QSTR("tokens.ocr_channel");
    const QString& iamge_translate_channel = CJ_GET_QSTR("tokens.iamge_translate_channel");
    const QString& youdao_app_id = CJ.decryptString(CJ_GET_STR("tokens.youdao_app_id"));
    const QString& youdao_secret_key = CJ.decryptString(CJ_GET_STR("tokens.youdao_secret_key"));
    const QString& baidu_api_key = CJ.decryptString(CJ_GET_STR("tokens.baidu_api_key"));
    const QString& baidu_secret_key = CJ.decryptString(CJ_GET_STR("tokens.baidu_secret_key"));

    const QStringList list = {"high", "high_location", "standard", "standard_location"};
    ui->cbbOcr->setCurrentIndex(list.indexOf(ocr_channel));

    const QStringList imgTranslate = {"baidu", "youdao"};  /*"baidu_fanyi",*/
    ui->cbbImgTranslate->setCurrentIndex(imgTranslate.indexOf(iamge_translate_channel));

    ui->leYDAppID->setText(youdao_app_id);
    ui->leYDApiSecret->setText(youdao_secret_key);
    ui->leBdApiKey->setText(baidu_api_key);
    ui->leBdSecretKey->setText(baidu_secret_key);
}


void Tokens::on_btnNext0_released()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Tokens::on_btnNext1_released()
{
    ui->stackedWidget->setCurrentIndex(0);
}

