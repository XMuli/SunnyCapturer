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
    const auto& themes = themesMap();

    std::map<QString, QtMsgType> logLevel = {  {"Debug", QtDebugMsg}
                                             , {"Info", QtInfoMsg}
                                             , {"Warning", QtWarningMsg}
                                             , {"Critical", QtCriticalMsg}
                                             , {"Fatal", QtFatalMsg}};

    const auto& currLanguage = CONF_MANAGE.property("XGeneral_language").toString();
    const auto& currTheme = CONF_MANAGE.property("XGeneral_themes").toString();
    const auto& currLogLevel = CONF_MANAGE.property("XGeneral_log_level").toString();
    for (const auto& it : languages) ui->cbbLanguage->addItem(it.first, it.second);
    for (const auto& it : themes) ui->cbbThemes->addItem(it.first, it.second);
    for (const auto& it : logLevel) ui->cbbLogLevel->addItem(it.first, it.second);

    ui->cbbLanguage->setCurrentText(currLanguage);
    ui->cbbThemes->setCurrentText(currTheme);
    ui->cbbLogLevel->setCurrentText(currLogLevel);
    ui->btnFont->setText(CONF_MANAGE.property("XGeneral_font").toString());
    ui->cbAutostart->setChecked(CONF_MANAGE.property("XGeneral_autostart").toBool());
    ui->btnFont->resize(ui->cbbLanguage->size());

#if defined(Q_OS_WIN)
#else
    ui->cbAutostart->hide();
#endif

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

void General::on_cbbThemes_currentTextChanged(const QString &arg1)
{
    const auto themes = themesMap();
    const auto& it = themes.find(arg1);
    const QString& path = it->second ? arg1 : qApp->applicationDirPath() + QString("/resources/qss/%1.qss").arg(it->first);

    if (it != themes.cend()) {
        if (it->second) {  // 系统自带的 style
            COMM.loadCustomQss("");

            QString style;
#if defined(Q_OS_WIN)
            style = "WindowsVista";
#elif  defined(Q_OS_LINUX)
            style = "Fusion";
#elif  defined(Q_OS_MAC)
            style = "Macintosh";
#endif

            qApp->setStyle(it->first == "default" ? style : it->first);
        } else {  // 三方 qss 样式
            qApp->setStyle("");
            COMM.loadCustomQss(path);
        }
    }

    qDebug() << "--->" << it->first << "  " << it->second << "  " <<path << "arg1:" << arg1;
    CONF_MANAGE.setProperty("XGeneral_themes", arg1);
}

