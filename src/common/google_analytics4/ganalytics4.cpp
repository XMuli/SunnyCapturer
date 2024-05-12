#include "ganalytics4.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QSslSocket>
#include <QString>
#include <QString>
#include <QUuid>
#include "ganalytics/ganalytics.h"
#include "qmetaobject.h"

#define GA_MEASUREMENT_ID "G-KRWGN5P6R2"            // "YOUR_MEASUREMENT_ID"
#define GA_API_SECRET     "xyHF9QotR3m_mgm50PRKnA"  // "YOUR_API_SECRET"

void GAnalytics4::sendEvent(const GAEvent &tEventName, const json &jParams)
{
#if GOOGLE_ANALYTICS_4
    json params;
    if (jParams == "") {
        // 不填写的默认所获取的数值
        if (tEventName == GAEvent::E_tray_launch) {
        } else if (tEventName == GAEvent::E_tray_close) {
        } else if (tEventName == GAEvent::E_tray_launch) {
        } else if (tEventName == GAEvent::E_chatui_launch) {
        } else if (tEventName == GAEvent::E_chatui_close) {
        } else if (tEventName == GAEvent::E_settingui_launch) {
        } else if (tEventName == GAEvent::E_tray_menu_launch) {
        } else if (tEventName == GAEvent::E_send_chat) {
        } else if (tEventName == GAEvent::E_os_info) {
            params = setOSInfoJParams();
        } else if (tEventName == GAEvent::E_geographic_info) {
        } else if (tEventName == GAEvent::e_unknow) {
        } else {
        }
    } else {
        params = jParams;
    }

    if (params.contains("params"))
        params["params"]["time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();
    const QString& eventName = enumToString(tEventName);
    const auto& uniqueId = QSysInfo::machineUniqueId().toStdString();

    json data;  // 初始化 JSON 对象内容
    data["client_id"] = uniqueId;
    data["events"] = params; // 和 name 顺序不可颠倒
    data["events"]["name"] = eventName.toStdString();

    ga->sendEvent(eventName, jToQJsonObject(data));

#endif
}

void GAnalytics4::sendEvent(const GAEvent &gaEvent, const std::map<std::string, std::string> &map)
{
    const json& j = map.size() > 0 ? mapToJParams(map) : "";
    sendEvent(gaEvent, j);
}

json GAnalytics4::setGeoJParams(const json& j)
{
    json event;  // 外面统一添加 event["name"]，此里面不添加
    event["params"] = j;
    return event;
}

json GAnalytics4::setTrayLaunchJParams()
{
    json event;
    return event;
}

json GAnalytics4::setOSInfoJParams()
{
    const auto& uniqueId = QSysInfo::machineUniqueId().toStdString();
    const auto& hostName = QSysInfo::machineHostName().toStdString();

    const auto& kernelType = QSysInfo::kernelType().toStdString();
    const auto& os = QSysInfo::prettyProductName().toStdString();
    const auto& kernel = QSysInfo::kernelVersion().toStdString();

    // 创建事件对象
    json event;
    event["params"] = {
        {"engagement_time_msec", 100},
        {"hostName", hostName},
        {"kernelType", kernelType},
        {"os", os},
        {"kernel", kernel}
    };

    return event;
}

json GAnalytics4::mapToJParams(const std::map<std::string, std::string> &map)
{
    json event;
    json j;

    int i = 0;
    for (const auto& it : map) {
        if (i++ > 25) break;
        else j[it.first] = it.second;
    }

    event["params"] = j;
    return event;
}


void GAnalytics4::init()
{
    // 查詢Qt支持的版本
    bool bSupp = QSslSocket::supportsSsl();
    QString buildVersion = QSslSocket::sslLibraryBuildVersionString();
    QString version = QSslSocket::sslLibraryVersionString();
    qDebug() << bSupp << buildVersion << version;

    // 查看当前网络请求支持协议
    QNetworkAccessManager *accessManager = new QNetworkAccessManager();
    qDebug() << accessManager->supportedSchemes();


    // Init GA
    QString clientID; // You can read ClientID from config
    if (clientID.isEmpty()) {
        clientID = QUuid::createUuid().toString();
        // You can save ClientID from config
    }
    ga->setClientID(clientID);
    ga->setMeasurementId(GA_MEASUREMENT_ID);
    ga->setApiSecret(GA_API_SECRET);
    ga->enable(true);
    ga->setAnonymizeIPs(true);
    ga->setSendInterval(3000);
    ga->setLogLevel(GAnalytics::Debug);
}

const QString GAnalytics4::enumToString(GAEvent gaEvent) const
{
    QMetaEnum metaEnum = QMetaEnum::fromType<GAnalytics4::GAEvent>();
    const auto& ret = metaEnum.valueToKey(gaEvent);            //! enum转string

    return QString(ret);
}

const QJsonObject GAnalytics4::jToQJsonObject(const json &j) const
{
    QJsonObject ret;
    QString jsonString = QString::fromStdString(j.dump());
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error: " << error.errorString();
        return ret;
    }

    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
        return ret;
    }

    ret = jsonDoc.object();
    return ret;
}

GAnalytics4::GAnalytics4(QObject *parent)
    : QObject(parent)
{
    init();
}
