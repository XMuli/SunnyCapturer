#include "general.h"
#include "ui_general.h"
#include <QDir>
#include <QFontDialog>
#include <QTranslator>
#include <map>
#include "communication.h"
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
    const auto& languages = languageMap();

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
    ui->btnFont->resize(ui->cbbLanguage->size());

    connect(ui->cbbLanguage, &QComboBox::currentTextChanged, this, &General::onCbbLanguageCurrentTextChanged);
    connect(&COMM, &Communication::sigLanguageChange, this, [this]() { ui->retranslateUi(this);});
}

void General::setAutoStart(const bool &enable)
{
#if defined(Q_OS_WIN)
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (enable) {
        QString strAppPath = QDir::toNativeSeparators(qApp->applicationFilePath());
        strAppPath.replace(QChar('/'), QChar('\\'), Qt::CaseInsensitive);
        reg.setValue(XPROJECT_NAME, strAppPath);
    } else {
        reg.setValue(XPROJECT_NAME, "");
    }
#else
#endif
}

void General::onCbbLanguageCurrentTextChanged(const QString &arg1)
{
    COMM.loadTranslation(arg1);
    ui->retranslateUi(this);

    ui->btnFont->setText(CONF_MANAGE.property("XGeneral_font").toString()); // fix: 切换语言后会被刷新掉
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

    QString text = QString("%1,%2").arg(font.family()).arg(font.pointSize());
    COMM.setAppFont(text);
    ui->btnFont->setText(text);

    CONF_MANAGE.setProperty("XGeneral_font", text);
    qDebug("当前选择的字体是[%s]-是否加粗[%d]-是否倾斜[%d]-字号[%d]", font.family().toUtf8().data(), font.bold(), font.italic(), font.pointSize());
}


void General::on_cbAutostart_clicked(bool checked)
{
    setAutoStart(checked);
    CONF_MANAGE.setProperty("XGeneral_autostart", checked);
}

void General::onBtnResetClicked(bool checked)
{
    ui->cbbLanguage->setCurrentText("English");
    ui->cbbLogLevel->setCurrentText("Debug");
    ui->btnFont->setText("Microsoft YaHei,9");
    ui->cbAutostart->setChecked(false);
}

