#ifndef NETWORKOCR_H
#define NETWORKOCR_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QPointer>

class NetworkOCR : public QObject
{
    Q_OBJECT
public:
    explicit NetworkOCR(QObject *parent = nullptr);
    virtual ~NetworkOCR() = default;
    void sendOCRRequest(const QString& path = "C:/Users/Venn/Desktop/Sunny_20231030_114240.png");               // 待翻译图片路径

private slots:
    void onRequestFinished(QNetworkReply* reply);


private:
    QPointer<QNetworkAccessManager>      m_networkManager;
};

#endif // NETWORKOCR_H
