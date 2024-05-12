#ifndef GANALYTICS4_H
#define GANALYTICS4_H

#include <QObject>
#include <QDebug>
#include "isingleton.h"
#include "json.hpp"
using json = nlohmann::json;

#define GA4 GAnalytics4::instance()

class GoogleGeo;
class GAnalytics4 : public QObject, public ISingleton<GAnalytics4>
{
    Q_OBJECT
    FRIEND_CREAT_SINGLETION(GAnalytics4);

public:
    enum GAEvent {
        E_launch,
        E_launch_capture,
        E_tray_close,
        E_setting_launch,
        E_setting_close,
        E_send_ocr,
        E_send_image_translate,

        E_os_info,
        E_geographic_info,
        e_unknow
    };
    Q_ENUM(GAEvent)

    void sendEvent(const GAEvent& gaEvent, const json &jParams = "");
    void sendEvent(const GAEvent& gaEvent, const std::map<std::string, std::string>& map);

    json setGeoJParams(const json& j);
    json setTrayLaunchJParams();
    json setOSInfoJParams();

    static json mapToJParams(const std::map<std::string, std::string>& map);

private:
    void init();
    const QString enumToString(GAEvent gaEvent) const;
    const QJsonObject jToQJsonObject(const json& j) const;

private:
    GAnalytics4(QObject *parent = nullptr);
    virtual ~GAnalytics4() = default;
};

#endif // GANALYTICS4_H
