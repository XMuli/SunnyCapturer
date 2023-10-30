#include "networkocr.h"
#include <QByteArray>
#include <QFile>
#include <QString>
#include <QUrlQuery>
#include <QUuid>
#include "authv3util.h"

// 您的应用 ID / 应用密钥
QString APP_KEY = "5a3aa1167eed698d";
QString APP_SECRET = "tgjKTMUqEsG5ZysptJMHOk7pIPwFCi9T";

NetworkOCR::NetworkOCR(QObject *parent)
    : QObject{parent}
    , m_networkManager(new QNetworkAccessManager(this))
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &NetworkOCR::onRequestFinished);
}

void NetworkOCR::sendOCRRequest(const QString &path)
{
    // 从文件中读取图像数据并进行base64编码
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file.";
        return;
    }

    QByteArray base64Data = file.readAll().toBase64();
//    qDebug().noquote() << "base64Data:" << base64Data << Qt::endl << Qt::endl;

    QString base64DataUtf8 = QString::fromUtf8(base64Data);
//    qDebug().noquote() << "base64DataUtf8:" << base64DataUtf8;

    // 构造POST请求
    QNetworkRequest request(QUrl("https://openapi.youdao.com/ocrtransapi"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("q", QUrl::toPercentEncoding(base64DataUtf8)); // fix 【重要】: 是 Qt 框架中用于将字符串转换为百分号编码（percent encoding）格式的函数。这个编码方式也称为 URL 编码或 URI 编码。它将字符串中的特殊字符以 % 符号后跟两位十六进制数字的形式进行编码，
    postData.addQueryItem("from", "auto");
    postData.addQueryItem("to", "zh-CHS");
    postData.addQueryItem("render", "1");
    postData.addQueryItem("type", "1");

    // 创建 QMap 并添加参数
    QMap<QString, QString> params;
    params["appKey"] = APP_KEY;
    params["salt"] = QUuid::createUuid().toString();
    params["curtime"] = QString::number(QDateTime::currentSecsSinceEpoch());
    params["signType"] = "v3";
    params["q"] = base64DataUtf8;

    // 添加身份验证参数
    AuthV3Util::addAuthParams(APP_KEY, APP_SECRET, params);

    for (const QString &key : params.keys()) {
        static int i = 1;

        if (key != "q")
            postData.addQueryItem(key, params[key]);

        qDebug().noquote() << "i:" << i++ << "key:" << key << params[key];
    }

    m_networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void NetworkOCR::onRequestFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        qDebug().noquote() << "responseData:" << responseData;
        //            // 反向 URL 解码数据以查看 +
        //            QString decodedData = QUrl::fromPercentEncoding(responseData);
        //            qDebug() << "decodedData:" << decodedData;

    } else {
        QStringList list;
        qDebug() << "Network request error: " << reply->errorString();
    }

    if (reply)
        reply->deleteLater();  // 此处统一释放
}


