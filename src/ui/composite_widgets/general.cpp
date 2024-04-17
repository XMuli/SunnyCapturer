// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "general.h"
#include "ui_general.h"
#include <QDir>
#include <QFontDialog>
#include <QTranslator>
#include <map>
#include "communication.h"
#include "../../data/configjson.h"

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

    const auto& currLanguage = CJ_GET_QSTR("general.language");
    const auto& currTheme = CJ_GET_QSTR("general.themes");
    const auto& currLogLevel = CJ_GET_QSTR("general.log_level");

    for (const auto& it : languages) ui->cbbLanguage->addItem(it.first, it.second);
    for (const auto& it : themes) ui->cbbThemes->addItem(it.first, it.second);
    for (const auto& it : logLevel) ui->cbbLogLevel->addItem(it.first, it.second);

    ui->cbbLanguage->setCurrentText(currLanguage);
    ui->cbbThemes->setCurrentText(currTheme);
    ui->cbbLogLevel->setCurrentText(currLogLevel);
    ui->btnFont->setText(CJ_GET_QSTR("general.font"));
    ui->cbAutostart->setChecked(CJ_GET("general.autostart").get<bool>());
    ui->btnFont->resize(ui->cbbLanguage->size());

#if defined(Q_OS_MACOS)
    ui->cbAutostart->hide();
#else

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
#elif defined (Q_OS_LINUX)
    // *.desktop 复制到 "~/.config/autostart/" 目录下实现开机自启动
    const QString desktopDir = "/usr/share/applications/tech.xmuli.sunny.desktop";
    const QString targetDir = QDir::homePath() + "/.config/autostart/";
    const QString targetFile = targetDir + "tech.xmuli.sunny.desktop";

    if (enable) {
        QDir().mkpath(targetDir);    // 创建目标目录（如果不存在）
        if (QFile::copy(desktopDir, targetFile)) qDebug() << "File tech.xmuli.sunny.desktop copied successfully to" << targetFile;
        else qDebug() << "Failed to copy file";
    } else {
        if (QFile::remove(targetFile)) qDebug() << "File removed successfully from" << targetFile;
        else qDebug() << "Failed tech.xmuli.sunny.desktop to remove file";
    }
#else
#endif
}

void General::onBtnResetClicked(bool checked)
{
    const ordered_json& j = CJ.defaultConfigJson();
    CJ.setJ("general", j["general"]);

    ui->cbbLanguage->setCurrentText(CJ_GET_QSTR("general.language"));
    ui->cbbThemes->setCurrentText(CJ_GET_QSTR("general.themes"));
    ui->cbbLogLevel->setCurrentText(CJ_GET_QSTR("general.log_level"));
    ui->btnFont->setText(CJ_GET_QSTR("general.font"));
    ui->cbAutostart->setChecked(CJ_GET("general.autostart").get<bool>());
}

void General::onCbbLanguageCurrentTextChanged(const QString &arg1)
{
    COMM.loadTranslation(arg1);
    ui->retranslateUi(this);

    ui->btnFont->setText(CJ_GET_QSTR("general.font")); // fix: 切换语言后会被刷新掉
    CJ_SET("general.language", arg1.toStdString());
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
    CJ_SET("general.themes", arg1.toStdString());
}

void General::on_cbbLogLevel_currentTextChanged(const QString &arg1)
{
    CJ_SET("general.log_level", arg1.toStdString());
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

    CJ_SET("general.font", text.toStdString());
    qDebug("当前选择的字体是[%s]-是否加粗[%d]-是否倾斜[%d]-字号[%d]", font.family().toUtf8().data(), font.bold(), font.italic(), font.pointSize());
}

void General::on_cbAutostart_clicked(bool checked)
{
    setAutoStart(checked);
    CJ_SET("general.autostart", checked);
}
