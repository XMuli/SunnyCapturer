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
        E_tray_launch,
        E_tray_close,
        E_chatui_launch,
        E_chatui_close,
        E_settingui_launch,
        E_tray_menu_launch,
        E_send_chat,
        E_send_tts,
        E_send_stt,

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
