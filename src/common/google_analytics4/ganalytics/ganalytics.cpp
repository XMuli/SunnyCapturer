#include "ganalytics.h"
#include "ganalytics_worker.h"

#include <QDataStream>
#include <QDebug>
#include <QLocale>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QQueue>
#include <QSettings>
#include <QTimer>
#include <QUrlQuery>
#include <QUuid>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

GAnalytics::GAnalytics(QObject *parent) : QObject(parent)
{
    d = new GAnalyticsWorker(this);
}

/**
 * Destructor of class GAnalytics.
 */
GAnalytics::~GAnalytics()
{
    delete d;
}

GAnalytics *GAnalytics::instance()
{
    static GAnalytics* instance = nullptr;
    if(instance == nullptr)
    {
        instance = new GAnalytics();
    }

    return instance;
}

void GAnalytics::setLogLevel(GAnalytics::LogLevel logLevel)
{
    d->m_logLevel = logLevel;
}

GAnalytics::LogLevel GAnalytics::logLevel() const
{
    return d->m_logLevel;
}

// SETTER and GETTER
void GAnalytics::setViewportSize(const QString &viewportSize)
{
    d->m_viewportSize = viewportSize;
}

QString GAnalytics::viewportSize() const
{
    return d->m_viewportSize;
}

void GAnalytics::setLanguage(const QString &language)
{
    d->m_language = language;
}

QString GAnalytics::language() const
{
    return d->m_language;
}

void GAnalytics::setAnonymizeIPs(bool anonymize)
{
    d->m_anonymizeIPs = anonymize;
}

bool GAnalytics::anonymizeIPs()
{
    return d->m_anonymizeIPs;
}

void GAnalytics::setSendInterval(int milliseconds)
{
    d->m_timer.setInterval(milliseconds);
}

int GAnalytics::sendInterval() const
{
    return (d->m_timer.interval());
}

bool GAnalytics::isEnabled()
{
    return d->m_isEnabled;
}

void GAnalytics::setMeasurementId(const QString &measurementId)
{
    d->m_measurementId = measurementId;
}

void GAnalytics::setApiSecret(const QString & apiSecret)
{
	d->m_apiSecret = apiSecret;
}

void GAnalytics::setClientID(const QString &clientID)
{
    d->m_clientID = clientID;
}

void GAnalytics::enable(bool state)
{
    d->enable(state);
}

int GAnalytics::version()
{
    return d->m_version;
}

void GAnalytics::setVertion(int version)
{
    d->m_version = version;
}

void GAnalytics::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager)
{
    if (d->networkManager != networkAccessManager)
    {
        // Delete the old network manager if it was our child
        if (d->networkManager && d->networkManager->parent() == this)
        {
            d->networkManager->deleteLater();
        }

        d->networkManager = networkAccessManager;
    }
}

QNetworkAccessManager *GAnalytics::networkAccessManager() const
{
    return d->networkManager;
}

static void appendCustomValues(QUrlQuery &query, const QVariantMap &customValues)
{
    for (QVariantMap::const_iterator iter = customValues.begin(); iter != customValues.end(); ++iter)
    {
        query.addQueryItem(iter.key(), iter.value().toString());
    }
}

/**
 * This method is called whenever a button was pressed in the application.
 * A query for a POST message will be created to report this event. The
 * created query will be stored in a message queue.
 */
void GAnalytics::sendEvent(const QString &category, const QString &eventName, const QString &eventParamName, const QString &eventParamValue)
{
	/*
    // event body example
	{
		"client_id": "XXXXXXXXXX.YYYYYYYYYY",
		"events": [{
			"name": "refund",
			"params": {
				"currency": "USD",
				"value": "9.99",
				"transaction_id": "ABC-123"
			}
		}]
	}
	*/

	QJsonObject root;
	root["client_id"] = d->m_clientID;
	QJsonObject event;

	if (!eventParamValue.isEmpty()) {
		event["name"] = eventName + "_" + eventParamValue;
	}
	else {
		event["name"] = eventName;
	}

	QJsonObject eventParams;
	eventParams["category"] = category;

    // In order for user activity to display in standard reports like Realtime, engagement_time_msec and session_id must be supplied as part of the params for an event.
    // https://developers.google.com/analytics/devguides/collection/protocol/ga4/sending-events?client_type=gtag#optional_parameters_for_reports
    eventParams["engagement_time_msec"] = 100;
    eventParams["session_id"] = d->m_clientID;

    // operation system
#ifdef Q_OS_WIN
    eventParams["os"] = "Windows";
#endif
#ifdef Q_OS_LINUX
    eventParams["os"] = "Linux";
#endif
#ifdef Q_OS_MAC
    eventParams["os"] = "MAC";
#endif
	event["params"] = eventParams;

	QJsonArray eventArray;
	eventArray.append(event);
	root["events"] = eventArray;

    d->enqueQueryWithCurrentTime(root);
}

void GAnalytics::sendEvent(const QString & eventName)
{
    sendEvent("", eventName, "", "");
}

//#include "../../systeminfo.h"
#include <QSysInfo>
//qInfo() << QString("bootUniqueId[%1]\n").arg(QSysInfo::bootUniqueId().data())
//        << QString("buildAbi[%1]\n").arg(QSysInfo::buildAbi())
//        << QString("buildCpuArchitecture[%1]\n").arg(QSysInfo::buildCpuArchitecture())
//        << QString("currentCpuArchitecture[%1]\n").arg(QSysInfo::currentCpuArchitecture())
//        << QString("kernelType[%1]\n").arg(QSysInfo::kernelType())
//        << QString("kernelVersion[%1]\n").arg(QSysInfo::kernelVersion())
//        << QString("machineHostName[%1]\n").arg(QSysInfo::machineHostName())
//        << QString("machineUniqueId[%1]\n").arg(QSysInfo::machineUniqueId().data())
//        << QString("prettyProductName[%1]\n").arg(QSysInfo::prettyProductName())
//        << QString("productType[%1]\n").arg(QSysInfo::productType())
//        << QString("productVersion[%1]\n").arg(QSysInfo::productVersion());

void GAnalytics::sendEvent(const QString &eventName, const QJsonObject &json)
{

/* vent body example
{
    "client_id": "{1a450965-9dc6-4920-abdb-2af1130d906d}",
    "events": [{
        "name": "E_tray_launch",
        "params": {
            "category": "",
            "engagement_time_msec": 100,
            "os": "Windows",
            "session_id": "{1a450965-9dc6-4920-abdb-2af1130d906d}"
        }
    }]
}
*/
    d->enqueQueryWithCurrentTime(json);
}

/**
 * Qut stream to persist class GAnalytics.
 */
QDataStream &operator<<(QDataStream &outStream, const GAnalytics &analytics)
{
    outStream << analytics.d->persistMessageQueue();

    return outStream;
}

/**
 * In stream to read GAnalytics from file.
 */
QDataStream &operator>>(QDataStream &inStream, GAnalytics &analytics)
{
    QList<QString> dataList;
    inStream >> dataList;
    analytics.d->readMessagesFromFile(dataList);

    return inStream;
}
