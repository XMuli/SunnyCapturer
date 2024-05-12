#ifndef GOOGLEGEO_H
#define GOOGLEGEO_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPointer>
#include <QEventLoop>
#include "json.hpp"
using json = nlohmann::json;

struct GeoInfo
{
    // Google API -----------------
    bool ok_google_api = false;
    double g_lat = 0;
    double g_lng = 0;
    double g_accuracy = 0;
    QStringList g_compound_code;

    // http://ip-api.com/json -----------------
    bool ok_ip_api = false;
    std::string country;
    std::string countryCode;
    std::string region;
    std::string regionName;
    std::string city;
//    std::string lat;
//    std::string lon;
    std::string timezone;
    std::string isp;
    std::string org;
    std::string as;
    std::string query;

    json toJson()
    {
        json params;  // 创建事件对象
        params = {
            {"ok_ip_api", ok_ip_api},
            {"country", country},
            {"countryCode", countryCode},
            {"region", region},
            {"regionName", regionName},
            {"city", city},
            {"timezone", timezone},
            {"isp", isp},
            {"org", org},
            {"as", as},
            {"query", query},
            {"ok_google_api", ok_google_api}
        };

        int i = 0;
        for (const auto& it : g_compound_code) {
            if (i <= 3)
                params["g_compound_code" + std::to_string(i++)] = it.toStdString();
        }

        return params;
    }
};

class GoogleGeo : public QObject
{
    Q_OBJECT
public:
    enum class RESP_TYPE {
        RT_Unknow,         // 忘记头文件加此，则解析默认为这个
        RT_Geolocation,   // google 转 经纬度(地理编码)
        RT_Geocoding,     // google 经纬度转实际位置(反向地理编码)
        RT_IpIPA,         // 比较靠谱的 http://ip-api.com/json
        RT_GeoSpare1      // 随便找的 http://api.k780.com/?app=ip.local&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json
    };

    explicit GoogleGeo(QObject *parent = nullptr);
    virtual ~GoogleGeo() = default;


    bool getGeo() const;

    GeoInfo geoInfo() const;

private:
    void getGeographic();

    void sendGeolocation();
    void sendGeocoding();
    void sendGeographyByIpApi();
    const QStringList rawHeader(const QNetworkReply* reply) const;
    void dealGeolocationRequest(QNetworkReply* reply);
    void dealGeocodingRequest(QNetworkReply* reply);
    void dealGeographyByIpApi(QNetworkReply* reply);

    void onSomeRecvGeoInfo();

signals:
    void sigRecvGeoInfo(const GeoInfo& geoInfo);

public slots:
    void onRequestFinished(QNetworkReply* reply);

private:
    QPointer<QNetworkAccessManager>      m_netManager;
    QEventLoop*                          m_eventLoop;
    QString                              m_key;
    GeoInfo                              m_geoInfo;

    // 定义计数器和时间间隔
    int                                 m_attemptCount;
    int                                 m_interval;
};

#endif // GOOGLEGEO_H
