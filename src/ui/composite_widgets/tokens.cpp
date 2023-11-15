#include "tokens.h"
#include "ui_tokens.h"
#include "../../data/configmanager.h"

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
    const QString& youdao_app_id = CONF_MANAGE.decryptString(CONF_GET_PROPERTY(XTokens_youdao_app_id).toByteArray());
    const QString& youdao_secret_key = CONF_MANAGE.decryptString(CONF_GET_PROPERTY(XTokens_youdao_secret_key).toByteArray());

    const QString& baidu_api_key = CONF_MANAGE.decryptString(CONF_GET_PROPERTY(XTokens_baidu_api_key).toByteArray());
    const QString& baidu_secret_key = CONF_MANAGE.decryptString(CONF_GET_PROPERTY(XTokens_baidu_secret_key).toByteArray());

    ui->leYDAppID->setText(youdao_app_id);
    ui->leYDApiSecret->setText(youdao_secret_key);
    ui->leBdApiKey->setText(baidu_api_key);
    ui->leBdSecretKey->setText(baidu_secret_key);
}

void Tokens::onBtnResetClicked(bool checked)
{
    Q_UNUSED(checked)
    ui->leYDAppID->clear();
    ui->leYDApiSecret->clear();
    ui->leBdApiKey->clear();
    ui->leBdSecretKey->clear();
}

void Tokens::on_leYDAppID_textChanged(const QString &arg1)
{
    CONF_SET_PROPERTY(XTokens_youdao_app_id, CONF_MANAGE.encryptString(arg1));
}

void Tokens::on_leYDApiSecret_textChanged(const QString &arg1)
{
    CONF_SET_PROPERTY(XTokens_youdao_secret_key, CONF_MANAGE.encryptString(arg1));
}

void Tokens::on_leBdApiKey_textChanged(const QString &arg1)
{
    CONF_SET_PROPERTY(XTokens_baidu_api_key, CONF_MANAGE.encryptString(arg1));
}

void Tokens::on_leBdSecretKey_textChanged(const QString &arg1)
{
    CONF_SET_PROPERTY(XTokens_baidu_secret_key, CONF_MANAGE.encryptString(arg1));
}




