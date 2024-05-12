#include "googlegeo.h"

#include <QStringList>
#include <QTimer>
#include <QUrl>


GoogleGeo::GoogleGeo(QObject *parent)
    : QObject(parent)
    , m_netManager(new QNetworkAccessManager(this))
    , m_eventLoop(new QEventLoop(this))
    , m_key("AIzaSyA8TaJuh1124drEb2Bzt-hDl-HY92qw18U")
    , m_attemptCount(0)
    , m_interval(1)
{
//    static QTimer timer;
//    QObject::connect(&timer, &QTimer::timeout, [this]() {
//        getGeographic();
//        qDebug() << "---------0->m_attemptCount:" << m_attemptCount << "m_interval:" << m_interval;
//        if ((m_geoInfo.ok_ip_api && m_geoInfo.ok_google_api) || m_attemptCount >= 3) {
//            timer.stop();
//            emit sigRecvGeoInfo(m_geoInfo);

//            qDebug() << "---------1->m_attemptCount:" << m_attemptCount << "m_interval:" << m_interval;
//        } else {
//            ++m_attemptCount;
//            if (m_attemptCount == 1) m_interval = 10;
//            else if (m_attemptCount == 2) m_interval = 15;
//            else if (m_attemptCount == 3) m_interval = 20;
//            else m_interval = 25;

//            // 打印日志或其他操作
//            qDebug() << "尝试次数: " << m_attemptCount;
//            timer.start(m_interval  * 1000); // 将分钟转换为毫秒

//            qDebug() << "---------2->m_attemptCount:" << m_attemptCount << "m_interval:" << m_interval;
//        }
//    });
//    timer.start(m_interval  * 1000); // 将分钟转换为毫秒

    connect(m_netManager, &QNetworkAccessManager::finished, this, &GoogleGeo::onRequestFinished);
    getGeographic();
}

void GoogleGeo::getGeographic()
{
    if (!m_geoInfo.ok_google_api)
        sendGeolocation();
    if (!m_geoInfo.ok_ip_api)
        sendGeographyByIpApi();
}

void GoogleGeo::sendGeolocation()
{
    const QUrl url("https://www.googleapis.com/geolocation/v1/geolocate?key=" + m_key);
    QNetworkRequest request(url);
    request.setRawHeader("Content-Type", "application/json");
    request.setAttribute(QNetworkRequest::User, static_cast<int>(RESP_TYPE::RT_Geolocation));

    m_netManager->post(request, "");
}

void GoogleGeo::sendGeocoding()
{
    const QString& url =QString("https://maps.googleapis.com/maps/api/geocode/json?latlng=%1,%2&key=%3")
        .arg(m_geoInfo.g_lat).arg(m_geoInfo.g_lng).arg(m_key);
    QNetworkRequest request((QUrl(url)));
    request.setRawHeader("Content-Type", "application/json");
    request.setAttribute(QNetworkRequest::User, static_cast<int>(RESP_TYPE::RT_Geocoding));

    m_netManager->post(request, "");
}

void GoogleGeo::sendGeographyByIpApi()
{
    QNetworkRequest request((QUrl("http://ip-api.com/json?lang=zh-CN")));
    request.setRawHeader("Content-Type", "application/json");
    request.setAttribute(QNetworkRequest::User, static_cast<int>(RESP_TYPE::RT_IpIPA));

    m_netManager->post(request, "");
}

const QStringList GoogleGeo::rawHeader(const QNetworkReply *reply) const
{
    QStringList list;
    if (!reply)
        return list;

    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QString reasonPhrase = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

    list << QString::number(statusCode)  // Status Code
         << reasonPhrase;                // Reason Phrase

#if 1
    // qDebug() << "Raw Header****:\n" << reply->rawHeaderList() << "\n";

    // QList<QByteArray> headerList = reply->rawHeaderList();
    // for (const QByteArray& header : headerList)
        // qDebug() << header << ": " << reply->rawHeader(header);

#endif

    return list;
}


void GoogleGeo::dealGeolocationRequest(QNetworkReply *reply)
{
    const auto & data = reply->readAll().toStdString();
    json j = json::parse(data);

    if (!j.empty()) {

        if (j.contains("location")) {
            json jLoc = j["location"];
            if (jLoc.contains("lat"))
                m_geoInfo.g_lat = jLoc["lat"];
            if (jLoc.contains("lng"))
                m_geoInfo.g_lng = jLoc["lng"];
        }

        if (j.contains("accuracy")) m_geoInfo.g_accuracy = j["accuracy"];
    }


    sendGeocoding();
}

void GoogleGeo::dealGeocodingRequest(QNetworkReply *reply)
{
    const std::string& data = reply->readAll().toStdString();
   // qDebug().noquote() << "dealGeocodingRequest data:" << QString::fromStdString(data);
    json j = json::parse(data);

    if (!j.empty() && j.contains("plus_code") && j["plus_code"].contains("compound_code")) {
        json jCompound_code = j["plus_code"]["compound_code"];
        QStringList lists = QString::fromStdString(jCompound_code.dump()).trimmed().split(',');

        for (int i = lists.count() - 1; i >=0 ; --i) {
            const auto& it = lists.at(i).trimmed();
            qDebug () << "it:" << it;
            m_geoInfo.g_compound_code << it;
        }

        m_geoInfo.ok_google_api = true;
    } else {
        m_geoInfo.ok_google_api = false;
        m_geoInfo.g_compound_code << "null";
    }

    onSomeRecvGeoInfo();
}

void GoogleGeo::dealGeographyByIpApi(QNetworkReply *reply)
{
    const std::string& data = reply->readAll().toStdString();
   qDebug()/*.noquote()*/ << "dealGeographyByIpApi data:" << QString::fromStdString(data);
    json j = json::parse(data);

    if (!j.empty() && j.contains("status") && j["status"] == "success") {
        if (j.contains("country")) m_geoInfo.country = j["country"];
        if (j.contains("countryCode")) m_geoInfo.countryCode = j["countryCode"];
        if (j.contains("region")) m_geoInfo.region = j["region"];
        if (j.contains("region")) m_geoInfo.region = j["region"];
        if (j.contains("regionName")) m_geoInfo.regionName = j["regionName"];
        if (j.contains("countryCode")) m_geoInfo.countryCode = j["countryCode"];
        if (j.contains("city")) m_geoInfo.city = j["city"];
//        if (j.contains("lat")) m_geoInfo.lat = j["lat"];
//        if (j.contains("lon")) m_geoInfo.lon = j["lon"];
        if (j.contains("timezone")) m_geoInfo.timezone = j["timezone"];
        if (j.contains("isp")) m_geoInfo.isp = j["isp"];
        if (j.contains("org")) m_geoInfo.org = j["org"];
        if (j.contains("as")) m_geoInfo.as = j["as"];
        if (j.contains("query")) m_geoInfo.query = j["query"];
        m_geoInfo.ok_ip_api = true;
    } else {
        m_geoInfo.ok_ip_api = false;
    }

    onSomeRecvGeoInfo();
}

void GoogleGeo::onSomeRecvGeoInfo()
{
    if (m_geoInfo.ok_ip_api && m_geoInfo.g_compound_code.size() > 0)
        emit sigRecvGeoInfo(m_geoInfo);
}

void GoogleGeo::onRequestFinished(QNetworkReply *reply)
{
    const auto& head = rawHeader(reply);
//    qDebug() << "head:" << head << "  head.at(0):" << head.at(0) << "  head.at(1):" << head.at(1);

    if (reply->error() == QNetworkReply::NoError) {
                const RESP_TYPE& type = static_cast<RESP_TYPE>(reply->request().attribute(QNetworkRequest::User, int(RESP_TYPE::RT_Unknow)).toInt());
                if(static_cast<RESP_TYPE>(type) == RESP_TYPE::RT_Geolocation) {
                    dealGeolocationRequest(reply);
                } else if (static_cast<RESP_TYPE>(type) == RESP_TYPE::RT_Geocoding) {
                    dealGeocodingRequest(reply);
                } else if (static_cast<RESP_TYPE>(type) == RESP_TYPE::RT_IpIPA) {
                    dealGeographyByIpApi(reply);
                } else {
                    qDebug() << "***************** RESP_TYPE::RT_Unknow not match resp type! If you forget to set request->setAttribute(QNetworkRequest::User, nType);, it will look like this*****************";
                }


    } else {  // QNetworkReply::UnknownContentError 处理错误
        QStringList list;
        list << head.at(0) << reply->errorString();  // reply->errorString() 比 head.at(1) 更详细
        qDebug() << "onRequestFinished()  list:" << list;
    }

    if (reply) reply->deleteLater();
}

GeoInfo GoogleGeo::geoInfo() const
{
    return m_geoInfo;
}
