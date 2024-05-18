#ifndef DBANALYTICS_H
#define DBANALYTICS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <json.hpp>

using json = nlohmann::json;

class DbAnalytics : public QObject {
    Q_OBJECT

public:
    explicit DbAnalytics(QObject *parent = nullptr);
    void sendData();

private slots:
    const json creatorData();
    QString getMotherboardUUID();
    void onReplyFinished(QNetworkReply *reply);
    void onErrorOccurred(QNetworkReply::NetworkError code);

private:
    QNetworkAccessManager *manager;
};

#endif // DBANALYTICS_H
