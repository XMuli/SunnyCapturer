#include "general.h"
#include "ui_general.h"
#include <QFontDialog>
#include <map>
#include "../../data/configmanager.h"

General::General(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::General)
{
    ui->setupUi(this);
    initUI();
}

General::~General()
{
    delete ui;
}

void General::initUI()
{
    std::map<QString, QString> languages = {  {"English", "en_US"}
                                            , {"简体中文", "zh_CN"}
                                            , {"繁体中文", "zh_TW"}};

    std::map<QString, QtMsgType> logLevel = {  {"Debug", QtDebugMsg}
                                             , {"Info", QtInfoMsg}
                                             , {"Warning", QtWarningMsg}
                                             , {"Critical", QtCriticalMsg}
                                             , {"Fatal", QtFatalMsg}};

    const auto& currLanguage = CONF_MANAGE.property("XGeneral_language").toString();
    const auto& currLogLevel = CONF_MANAGE.property("XGeneral_log_level").toString();
    for (const auto& it : languages) ui->cbbLanguage->addItem(it.first, it.second);
    for (const auto& it : logLevel) ui->cbbLogLevel->addItem(it.first, it.second);

    ui->cbbLanguage->setCurrentText(currLanguage);
    ui->cbbLogLevel->setCurrentText(currLogLevel);
    ui->btnFont->setText(CONF_MANAGE.property("XGeneral_font").toString());
    ui->cbAutostart->setChecked(CONF_MANAGE.property("XGeneral_autostart").toBool());
    ui->btnFont->setFixedHeight(ui->cbbLanguage->height());

}

void General::on_cbbLanguage_currentTextChanged(const QString &arg1)
{
    CONF_MANAGE.setProperty("XGeneral_language", arg1);
}


void General::on_cbbLogLevel_currentTextChanged(const QString &arg1)
{
    CONF_MANAGE.setProperty("XGeneral_log_level", arg1);
}


void General::on_btnFont_released()
{
    bool ok = false;
    const auto fontFamily = ui->btnFont->text();
    QFont font = QFontDialog::getFont(&ok, QFont(fontFamily.split(',').at(0)), this, tr("Select Font"));
    if (!ok) return;

    qApp->setFont(font);
    QString text = QString("%1,%2").arg(font.family()).arg(font.pointSize());
    ui->btnFont->setText(text);

    CONF_MANAGE.setProperty("XGeneral_font", text);
    qDebug("当前选择的字体是[%s]-是否加粗[%d]-是否倾斜[%d]-字号[%d]", font.family().toUtf8().data(), font.bold(), font.italic(), font.pointSize());
}


void General::on_cbAutostart_clicked(bool checked)
{
    CONF_MANAGE.setProperty("XGeneral_autostart", checked);
}

