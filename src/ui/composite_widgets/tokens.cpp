// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "tokens.h"
#include "ui_tokens.h"
#include <QIcon>
#include "../../data/configjson.h"
#include "../paint_bar/toolbar_level/paintbarhelper.h"

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
    const QString& youdao_app_id = CJ.decryptString(CJ_GET_STR("tokens.account.youdao.app_id"));
    const QString& youdao_secret_key = CJ.decryptString(CJ_GET_STR("tokens.account.youdao.secret_key"));
    const QString& baidu_api_key = CJ.decryptString(CJ_GET_STR("tokens.account.baidu.api_key"));
    const QString& baidu_secret_key = CJ.decryptString(CJ_GET_STR("tokens.account.baidu.secret_key"));
    const int& ocr_channel = CJ_GET("tokens.ocr.channel");
    const int& iamge_translate_channel = CJ_GET("tokens.iamge_translate.channel");

    ui->leYDAppID->setText(youdao_app_id);
    ui->leYDApiSecret->setText(youdao_secret_key);
    ui->leBdApiKey->setText(baidu_api_key);
    ui->leBdSecretKey->setText(baidu_secret_key);

    const QStringList list = {tr("standard location"), tr("high location"), tr("high"), tr("standard")}; // 和 OcrChannel 顺序保持一致即可
    for (int i = 0; i < list.count(); ++i) {
        const QString& text = QString("%1 %2").arg(i).arg(list.at(i));
        ui->cbbOcr->addItem(text, i);
        ui->cbbOcr->setItemIcon(i, QIcon(":/resources/icons/setting/tokens/baidu.svg"));
    }
    ui->cbbOcr->setCurrentIndex(ocr_channel);

    const QStringList imgTranslate = {tr("baidu"), tr("youdao")};
    for (int i = 0; i < imgTranslate.count(); ++i) {
        const QString& text = QString("%1 %2").arg(i).arg(imgTranslate.at(i));
        ui->cbbImgTranslate->addItem(text, i);
        QString iconName = "";
        if (ImageTranslateChannel(i) == ImageTranslateChannel::ITC_baidu) iconName = "baidu";
        else if (ImageTranslateChannel(i) == ImageTranslateChannel::ITC_youdao) iconName = "youdao";
        ui->cbbImgTranslate->setItemIcon(i, QIcon(QString(":/resources/icons/setting/tokens/%1.svg").arg(iconName)));
    }
    ui->cbbImgTranslate->setCurrentIndex(iamge_translate_channel);
}

void Tokens::on_cbbOcr_currentIndexChanged(int index)
{
    const int& channel = ui->cbbOcr->currentData().toInt();
    CJ_SET("tokens.ocr.channel", channel);
}

void Tokens::on_cbbImgTranslate_currentIndexChanged(int index)
{
    const int& channel = ui->cbbImgTranslate->currentData().toInt();
    CJ_SET("tokens.iamge_translate.channel", channel);
}

void Tokens::on_leYDAppID_textChanged(const QString &arg1)
{
    CJ_SET("tokens.account.youdao.app_id", CJ.encryptString(arg1));
}

void Tokens::on_leYDApiSecret_textChanged(const QString &arg1)
{
    CJ_SET("tokens.account.youdao.secret_key", CJ.encryptString(arg1));

}

void Tokens::on_leBdApiKey_textChanged(const QString &arg1)
{
    CJ_SET("tokens.account.baidu.api_key", CJ.encryptString(arg1));
}

void Tokens::on_leBdSecretKey_textChanged(const QString &arg1)
{
    CJ_SET("tokens.account.baidu.secret_key", CJ.encryptString(arg1));
}

void Tokens::onBtnResetClicked(bool checked)
{
    Q_UNUSED(checked)
    const ordered_json& j = CJ.defaultConfigJson();
    CJ.setJ("tokens", j["tokens"]);

    const QString& ocr_channel = CJ_GET_QSTR("tokens.ocr.channel");
    const QString& iamge_translate_channel = CJ_GET_QSTR("tokens.iamge_translate.channel");
    const QString& youdao_app_id = CJ.decryptString(CJ_GET_STR("tokens.account.youdao.app_id"));
    const QString& youdao_secret_key = CJ.decryptString(CJ_GET_STR("tokens.account.youdao.secret_key"));
    const QString& baidu_api_key = CJ.decryptString(CJ_GET_STR("tokens.account.baidu.api_key"));
    const QString& baidu_secret_key = CJ.decryptString(CJ_GET_STR("tokens.account.baidu.secret_key"));

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

