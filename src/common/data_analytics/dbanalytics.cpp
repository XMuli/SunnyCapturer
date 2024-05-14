#include "DbAnalytics.h"
#include <QDateTime>
#include <QNetworkRequest>
#include <QGuiApplication>
#include <QDebug>
#include "systeminfo.h"
#include "../../data/configjson.h"

DbAnalytics::DbAnalytics(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &DbAnalytics::onReplyFinished);
}

void DbAnalytics::sendData() {
    QString urlString = "https://47.110.40.198:5000/add_data";
    // QString urlString = "https://127.0.0.1:5000/add_data";
    QNetworkRequest request;
    request.setUrl(QUrl(urlString));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Disable SSL verification
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    // sslConfig.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(sslConfig);

    json data = creatorData();
    QByteArray jsonData = QByteArray::fromStdString(data.dump());
    QNetworkReply *reply = manager->post(request, jsonData);
    connect(reply, &QNetworkReply::errorOccurred, this, &DbAnalytics::onErrorOccurred);
}

const json DbAnalytics::creatorData()
{
    QLocale locale = QLocale::system();
    QString language = QLocale::languageToString(locale.language());
    QLocale::Country countryEnum = locale.country();
    QString country = QLocale::countryToString(countryEnum);
    const auto priScren = qGuiApp->primaryScreen();
    QRect rect = priScren != nullptr ? priScren->geometry() : QRect();
    QString geometry = QString("(%1, %2 %3x%4)").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
    QSizeF physicalSizeF = priScren != nullptr ? priScren->physicalSize() : QSizeF();
    QString physical_size = QString("%1x%2").arg(physicalSizeF.width()).arg(physicalSizeF.height());

    json data;
    data["os_type"] = SYSINFO.getOperatingSystem().toStdString().data();
#if defined(Q_OS_WIN)/* && defined(_MSC_VER)*/
    QString edition = SYSINFO.getRegistryValue("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");     // Edition: "Windows 10 Pro"
    QString version = SYSINFO.getRegistryValue("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "DisplayVersion");  // Version: "22H2"
    QString currentBuild = SYSINFO.getRegistryValue("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentBuild");

    data["os_editon"] = edition.toStdString();
    data["os_version"] = QString(version + " " + currentBuild).toStdString();
#else
    data["os_editon"] = QSysInfo::prettyProductName().toStdString().data();
    data["os_version"] = QSysInfo::kernelVersion().toStdString().data();
#endif

    data["os_editon"] = QSysInfo::prettyProductName().toStdString().data();
    data["os_version"] = QSysInfo::kernelVersion().toStdString().data();

    data["os_language"] = language.toStdString().data();
    data["qt_version"] = QString(QT_VERSION_STR).toStdString().data();
    data["sunny_version"] = QString(XPROJECT_VERSION).toStdString().data();
    data["sunny_language"] = CJ_GET_QSTR("general.language").toStdString().data(); // 得在初始化
    data["sunny_path"] = qGuiApp->applicationDirPath().toStdString().data();
    data["arch_bit"] = XARCH_BIT;
    data["compiler"] = QString(XCOMPILER).toStdString().data();
    data["compiler_id"] = QString(XCOMPILER_ID).toStdString().data();
    data["memory"] = SYSINFO.getMemoryInfo().toStdString();
    data["cpu"] = SYSINFO.getCPUInfo().toStdString();
    data["user_name"] = SYSINFO.getUsername().toStdString();
    data["mac"] = SYSINFO.getMacInfo().toStdString();
    data["time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();  // 上报时间
    data["region"] = country.toStdString().data();
    data["city"] = "";
    data["monitor_count"] = qGuiApp->screens().count();
    data["geometry"] = rect.isValid() ? geometry.toStdString().data() : "";;
    data["scaling"] = SYSINFO.scaling(priScren);
    data["logical_dpi"] = priScren ? int(priScren->logicalDotsPerInch()) : 0;
    data["physical_dpi"] = priScren ? int(priScren->physicalDotsPerInch()) : 0;
    data["refresh_rate"] = priScren ? int(priScren->refreshRate()) : 0;
    data["physical_size"] = physicalSizeF.isValid() ?  physical_size.toStdString().data() : "";
    return data;
}

void DbAnalytics::onReplyFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response_data = reply->readAll();
        qDebug() << "Server Response:" << response_data;
    } else {
        qDebug() << "Error:" << reply->errorString();
    }
    reply->deleteLater();
}

void DbAnalytics::onErrorOccurred(QNetworkReply::NetworkError code) {
    qDebug() << "Network Error:" << code;
}
