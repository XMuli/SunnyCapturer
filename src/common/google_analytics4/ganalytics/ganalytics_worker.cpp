#include "ganalytics.h"
#include "ganalytics_worker.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QGuiApplication>
#include <QScreen>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkProxy>

const QLatin1String GAnalyticsWorker::dateTimeFormat("yyyy,MM,dd-hh:mm::ss:zzz");

GAnalyticsWorker::GAnalyticsWorker(GAnalytics *parent)
    : QObject(parent), q(parent), m_logLevel(GAnalytics::Error)
{
    m_appName = QCoreApplication::instance()->applicationName();
    m_appVersion = QCoreApplication::instance()->applicationVersion();
    m_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    m_request.setHeader(QNetworkRequest::UserAgentHeader, getUserAgent());

    m_language = QLocale::system().name().toLower().replace("_", "-");
    m_screenResolution = getScreenResolution();

    m_timer.setInterval(m_timerInterval);
    connect(&m_timer, &QTimer::timeout, this, &GAnalyticsWorker::postMessage);
}

void GAnalyticsWorker::enable(bool state)
{
    // state change to the same is not valid.
    if(m_isEnabled == state)
    {
        return;
    }

    m_isEnabled = state;
    if(m_isEnabled)
    {
        // enable -> start doing things :)
        m_timer.start();
    }
    else
    {
        // disable -> stop the timer
        m_timer.stop();
    }
}

void GAnalyticsWorker::logMessage(GAnalytics::LogLevel level, const QString &message)
{
    if (m_logLevel > level)
    {
        return;
    }

    qDebug() << "[Analytics]" << message;
}

/**
 * Get primary screen resolution.
 * @return      A QString like "800x600".
 */
QString GAnalyticsWorker::getScreenResolution()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize size = screen->size();

    return QString("%1x%2").arg(size.width()).arg(size.height());
}

/**
 * Try to gain information about the system where this application
 * is running. It needs to get the name and version of the operating
 * system, the language and screen resolution.
 * All this information will be send in POST messages.
 * @return agent        A QString with all the information formatted for a POST message.
 */
QString GAnalyticsWorker::getUserAgent()
{
    return QString("%1/%2").arg(m_appName).arg(m_appVersion);
}

/**
 * The message queue contains a list of QueryBuffer object.
 * QueryBuffer holds a QUrlQuery object and a QDateTime object.
 * These both object are freed from the buffer object and
 * inserted as QString objects in a QList.
 * @return dataList     The list with concartinated queue data.
 */
QList<QString> GAnalyticsWorker::persistMessageQueue()
{
    QList<QString> dataList;
    foreach (QueryBuffer buffer, m_messageQueue)
    {
		dataList << QJsonDocument(buffer.postQuery).toJson();
        dataList << buffer.time.toString(dateTimeFormat);
    }
    return dataList;
}

/**
 * Reads persistent messages from a file.
 * Gets all message data as a QList<QString>.
 * Two lines in the list build a QueryBuffer object.
 */
void GAnalyticsWorker::readMessagesFromFile(const QList<QString> &dataList)
{
    QListIterator<QString> iter(dataList);
    while (iter.hasNext())
    {
        QString queryString = iter.next();
        QString dateString = iter.next();
        QDateTime dateTime = QDateTime::fromString(dateString, dateTimeFormat);
        QueryBuffer buffer;

		QJsonDocument jsonDocument = QJsonDocument::fromJson(queryString.toUtf8());
		if (jsonDocument.isNull()) {
			qDebug() << "===> please check the string " << queryString.toUtf8();
		}
		QJsonObject jsonObject = jsonDocument.object();

        buffer.postQuery = jsonObject;
        buffer.time = dateTime;
        m_messageQueue.enqueue(buffer);
    }
}

/**
 * Takes a QUrlQuery object and wrapp it together with
 * a QTime object into a QueryBuffer struct. These struct
 * will be stored in the message queue.
 */
void GAnalyticsWorker::enqueQueryWithCurrentTime(const QJsonObject &query)
{
    QueryBuffer buffer;
    buffer.postQuery = query;
    buffer.time = QDateTime::currentDateTime();

    m_messageQueue.enqueue(buffer);
}

/**
 * This function is called by a timer interval.
 * The function tries to send a messages from the queue.
 * If message was successfully send then this function
 * will be called back to send next message.
 * If message queue contains more than one message then
 * the connection will kept open.
 * The message POST is asyncroniously when the server
 * answered a signal will be emitted.
 */
void GAnalyticsWorker::postMessage()
{
    if (m_messageQueue.isEmpty())
    {
        // queue empty -> try sending later
        m_timer.start();
        return;
    }
    else
    {
        // queue has messages -> stop timer and start sending
        m_timer.stop();
    }

    QString connection = "close";
    if (m_messageQueue.count() > 1)
    {
        connection = "keep-alive";
    }

    QueryBuffer buffer = m_messageQueue.head();
    QDateTime sendTime = QDateTime::currentDateTime();
    qint64 timeDiff = buffer.time.msecsTo(sendTime);

    if (timeDiff > fourHours)
    {
        // too old.
        m_messageQueue.dequeue();
        emit postMessage();
        return;
    }

	QByteArray requestJSon = QJsonDocument(buffer.postQuery).toJson(QJsonDocument::Compact);
    m_request.setRawHeader("Connection", connection.toUtf8());
    m_request.setHeader(QNetworkRequest::ContentLengthHeader, requestJSon.length());

	if (m_measurementId.isEmpty()) {
		logMessage(GAnalytics::Error, "google analytics measurement id was not set!");
	}
	if (m_apiSecret.isEmpty()) {
		logMessage(GAnalytics::Error, "google analytics api seceret was not set!");
	}

	QString requestUrl = "https://www.google-analytics.com/mp/collect?measurement_id=%1&api_secret=%2";
	requestUrl = requestUrl.arg(m_measurementId).arg(m_apiSecret);
	m_request.setUrl(QUrl(requestUrl));

//    logMessage(GAnalytics::Debug, "GA Query string = " + requestJSon);

    // Create a new network access manager if we don't have one yet
    if (networkManager == NULL)
    {
        networkManager = new QNetworkAccessManager(this);
        QNetworkProxy* proxy = new QNetworkProxy();
        proxy->setType(QNetworkProxy::NoProxy);
        networkManager->setProxy(*proxy);
    }

    QNetworkReply *reply = networkManager->post(m_request, requestJSon);
    connect(reply, SIGNAL(finished()), this, SLOT(postMessageFinished()));
}

/**
 * NetworkAccsessManager has finished to POST a message.
 * If POST message was successfully send then the message
 * query should be removed from queue.
 * SIGNAL "postMessage" will be emitted to send next message
 * if there is any.
 * If message couldn't be send then next try is when the
 * timer emits its signal.
 */
void GAnalyticsWorker::postMessageFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    int httpStausCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (httpStausCode < 200 || httpStausCode > 299)
    {
        logMessage(GAnalytics::Error, QString("Error posting message: %1").arg(reply->errorString()));

        // An error ocurred. Try sending later.
        m_timer.start();
        return;
    }
    else
    {
//        logMessage(GAnalytics::Debug, "Message sent");
    }

    m_messageQueue.dequeue();
    postMessage();
    reply->deleteLater();
}
