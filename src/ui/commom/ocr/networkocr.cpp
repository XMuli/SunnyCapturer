// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "networkocr.h"
#include <QByteArray>
#include <QFile>
#include <QImage>
#include <QString>
#include <QUrlQuery>
#include <QUuid>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <QStringList>
#include <QMetaObject>
#include <QMetaEnum>
#include <QMetaMethod>
#include <QHttpPart>
#include <QHttpPart>
#include <QHttpPart>
#include "authv3util.h"
#include "communication.h"
#include "json.hpp"
using json = nlohmann::json;


NetworkOCR::NetworkOCR(QObject *parent)
    : QObject{parent}
    , m_networkManager(new QNetworkAccessManager(this))
//    , m_eventLoop(new QEventLoop(this))
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &NetworkOCR::onRequestFinished);
    sendBaiDuAccessToken();
}

void NetworkOCR::sendBaiDuOcrRequest(const OcrData &data, const QString &path)
{
    // OTP_standard,                      // 通用文字识别（标准版）           1000 次/month
    // OCR_standard_location,             // 通用文字识别（标准含位置版）      1000 次/month
    // OCR_high_precision,                // 通用文字识别（高精度版）         1000 次/month
    // OCR_high_precision_location        // 通用文字识别（高精度含位置版）     500 次/month

    QString para1 = "accurate";
    if (data.pipeline == OcrChannel::OCR_high_precision_location) para1 = "accurate";
    else if (data.pipeline == OcrChannel::OCR_high_precision) para1 = "accurate_basic";
    else if (data.pipeline == OcrChannel::OCR_standard_location) para1 = "general";
    else if (data.pipeline == OcrChannel::OCR_standard) para1 = "general_basic";
    ;
    // 从文件中读取图像数据并进行base64编码
    QUrl url("https://aip.baidubce.com/rest/2.0/ocr/v1/" + para1 + "?access_token=" + m_baiDuToken);
    QNetworkRequest request(url);
    QByteArray postData = "image=" + QUrl::toPercentEncoding(base64FromFileContent(path)) + "&detect_direction=false&vertexes_location=false&paragraph=false&probability=false";
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Accept", "application/json");
    request.setAttribute(QNetworkRequest::User, int(RESP_TYPE::RT_baidu_text));
    request.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 1), QVariant::fromValue<OcrData>(data));   // 当前 发送的请求线路

    m_networkManager->post(request, postData);
}

void NetworkOCR::sendBaiDuImgTranslateRequest(const ImgTranslateData &data, const QString &path)
{
    QUrl url("https://aip.baidubce.com/file/2.0/mt/pictrans/v1");
    QUrlQuery query;
    query.addQueryItem("access_token", m_baiDuToken);
    url.setQuery(query);

    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("multipart/form-data; boundary=---------------------------1234567890"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\"; filename=\"" + QFileInfo(path).fileName() + "\""));

    QFile *file = new QFile(path);
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(imagePart);

    QUrlQuery params;
    params.addQueryItem("from", "zh");
    params.addQueryItem("to", "en");
    params.addQueryItem("v", "3");
    params.addQueryItem("paste", "1");

    url.setQuery(params);

    QNetworkRequest request(url);

    QNetworkReply *reply = m_networkManager->post(request, multiPart);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        qDebug() << "---#1>responseData:" << responseData;
    } else {
        qDebug() << "Error: " << reply->errorString();
    }

}

void NetworkOCR::sendYouDaoImgTranslateRequest(const ImgTranslateData &data, const QString &path)
{
    QString base64DataUtf8 = base64FromFileContent(path);

    // 构造POST请求
    QNetworkRequest request(QUrl("https://openapi.youdao.com/ocrtransapi"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, int(RESP_TYPE::RT_youdao_img_translate));

    QUrlQuery postData;
    postData.addQueryItem("q", QUrl::toPercentEncoding(base64DataUtf8)); // fix 【重要】: 是 Qt 框架中用于将字符串转换为百分号编码（percent encoding）格式的函数。这个编码方式也称为 URL 编码或 URI 编码。它将字符串中的特殊字符以 % 符号后跟两位十六进制数字的形式进行编码，
    postData.addQueryItem("from", data.from);
    postData.addQueryItem("to", data.to);
    postData.addQueryItem("render", data.render);
    postData.addQueryItem("type", "1");

    // 您的 YouDao 应用 ID / 应用密钥
    QString APP_KEY = CONF_MANAGE.decryptString(CONF_GET_PROPERTY(XTokens_youdao_app_id).toByteArray());
    QString APP_SECRET = CONF_MANAGE.decryptString(CONF_GET_PROPERTY(XTokens_youdao_secret_key).toByteArray());

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

const QStringList NetworkOCR::rawHeader(const QNetworkReply *reply) const
{
    QStringList list;
    if (!reply)
        return list;

    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QString reasonPhrase = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

    list << QString::number(statusCode)  // Status Code
         << reasonPhrase;                // Reason Phrase

#if 0
    qDebug() << "Raw Header****:\n" << reply->rawHeaderList() << "\n";

    QList<QByteArray> headerList = reply->rawHeaderList();
    for (const QByteArray& header : headerList)
        qDebug() << header << ": " << reply->rawHeader(header);

#endif

    return list;
}

const QString NetworkOCR::replyErrorShowText(const QStringList &dataHead)
{
    return tr("Status Code: ") + dataHead.at(0) + tr("\nReason Phrase: ") + dataHead.at(1);
}

QString NetworkOCR::base64FromFileContent(const QString &path, bool urlencoded)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return "";

    QByteArray fileData = file.readAll();
    file.close();

    QString base64Content = fileData.toBase64();

//    qDebug().noquote() << "fileData:" << fileData << Qt::endl << Qt::endl;
//    qDebug().noquote() << "base64Content:" << base64Content;

    if (urlencoded)
        base64Content = QUrl(base64Content).toEncoded();

    return base64Content;
}

const bool NetworkOCR::validityBaiDuKey(const QString &client_id, const QString &client_secret) const
{
    if (client_id.isEmpty() || client_secret.isEmpty()) {
        const QString title(tr("Warning"));
        const QString msg(tr("Your BaiDu [APIKey Or SecretKey] is empty, please check and re-enter it!"));

        qDebug() << title + msg;
//        QMessageBox::warning(nullptr, title, msg); // 暂时屏蔽
        return false;
    } else {
        return true;
    }
}

void NetworkOCR::sendBaiDuAccessToken(const QString &client_id, const QString &client_secret)
{
    if (!validityBaiDuKey(client_id, client_secret)) return;
    if (!m_baiDuToken.isEmpty()) return;

    QNetworkRequest request(QUrl("https://openapi.baidu.com/oauth/2.0/token"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, int(RESP_TYPE::RT_baidu_access_token));
    QUrlQuery params;
    params.addQueryItem("client_id", client_id);
    params.addQueryItem("client_secret", client_secret);
    params.addQueryItem("grant_type", "client_credentials");

    m_networkManager->post(request, params.query().toUtf8());
//    m_eventLoop->exec();   // 阻塞
}

void NetworkOCR::dealBaiDuAccessToken(QNetworkReply *reply)
{
    const QStringList& dataHead = rawHeader(reply);
    QByteArray response = reply->readAll();
//    qDebug().noquote() << " dealBaiDuAccessToken reply -> Content:" << response;
    if (response.isEmpty()) {
        qWarning() << "reply->readAll() is empty!  retrn;!!!";
        return;
    }

    if (dataHead.at(0) == "200") {
        json j;
        try {
            j = json::parse(response.toStdString());
        } catch (const std::exception& e) {
            qDebug() << "Failed to parse JSON:" << e.what();
            return;
        }

        if (!j.empty() && j.contains("access_token")) {
            m_baiDuToken = QString::fromStdString(j["access_token"]);
            qDebug() << "m_baiDuToken:" << m_baiDuToken;
//            m_eventLoop->quit();
        }
    } else {
        qWarning() << replyErrorShowText(dataHead);
    }
}

void NetworkOCR::dealBaiDuOcrRequest(QNetworkReply *reply)
{
    const QStringList& dataHead = rawHeader(reply);
    QByteArray response = reply->readAll();
        qDebug().noquote() << " dealBaiDuOcrRequest reply -> Content:" << response;
    if (response.isEmpty()) {
        qWarning() << "reply->readAll() is empty!  retrn;!!!";
        return;
    }

    if (dataHead.at(0) == "200") {
        const OcrData& ocrTextData = reply->request().attribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 1)).value<OcrData>();
        emit COMM.sigOCRTextGenerateFinsh(response, ocrTextData);
    } else {
        qWarning() << replyErrorShowText(dataHead);
    }
}

void NetworkOCR::dealYouDaoImgTranslateRequest(QNetworkReply *reply)
{
    const QStringList& dataHead = rawHeader(reply);
    QByteArray response = reply->readAll();
    qDebug().noquote() << " dealYouDaoImgTranslateRequest reply -> Content:" << response;
    if (response.isEmpty()) {
        qWarning()<<"reply->readAll() is empty!  retrn;!!!";
        return;
    }

    // 解析 JSON 数据
    json j;
    try {
        j = json::parse(response.toStdString());
    } catch (const std::exception& e) {
        qDebug() << "Failed to parse JSON:" << e.what();
        return;
    }

    if (dataHead.at(0) == "200") {
        if (j.contains("errorCode") && j["errorCode"] == "0" && j.contains("render_image") && j.contains("image_size")) {
            QString render_image = QString::fromStdString(j["render_image"].dump());
            QString image_size = QString::fromStdString(j["image_size"].dump());
            QString tImageSize = image_size.mid(1, image_size.size() - 2);
            const auto& lists = tImageSize.split(',');

            QSize size(-1, -1);
            if (lists.count() == 2) {
                size.setWidth(lists.at(1).toInt());
                size.setHeight(lists.at(0).toInt());
            }

            QByteArray byteArray = QByteArray::fromBase64(render_image.toUtf8());

            // 将图像数据加载到QImage
            QImage image;
            if (image.loadFromData(byteArray)) {
                const QString dir = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first() + "/ocr/";
                QDir directory(dir);
                if (!directory.exists() && !directory.mkpath(dir)) {
                    qDebug() << "Failed to create directory: " << dir;
                }

                const auto& path = dir + QString("OCR_%1_%2.png").arg(XPROJECT_NAME).arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
                image.save(path);

                emit COMM.sigOCRImageGenerateFinsh(size, path);
                qDebug() << "OCR image size:" << size << "path:" << path;
            } else {
                qDebug() << "Failed to load image.";
            }

            //            // 反向 URL 解码数据以查看 +
            //            QString decodedData = QUrl::fromPercentEncoding(responseData);
            //            qDebug() << "decodedData:" << decodedData;

        } else {
            emit COMM.sigOCRImageGenerateFinsh(QSize(), QString::fromStdString(j.dump()));   // 返回具体的错误码
        }

    } else {
        qWarning() << replyErrorShowText(dataHead);
    }
}

void NetworkOCR::onRequestFinished(QNetworkReply *reply)
{
    const auto& head = rawHeader(reply);
//    if (head.count() >= 2)
//        qDebug() << "head:" << head << "  head.at(0):" << head.at(0) << "  head.at(1):" << head.at(1);

    if (reply->error() == QNetworkReply::NoError) {
        const RESP_TYPE& type = static_cast<RESP_TYPE>(reply->request().attribute(QNetworkRequest::User, int(RESP_TYPE::RT_empty)).toInt());
        QMetaEnum metaEnum = QMetaEnum::fromType<NetworkOCR::RESP_TYPE>();
        const auto& szType = metaEnum.valueToKey(int(type));            //! enum转string
        qDebug() << "***************** type:" << szType << (int)type;

        if (static_cast<RESP_TYPE>(type) == RESP_TYPE::RT_baidu_access_token) {
            dealBaiDuAccessToken(reply);
        } else if (static_cast<RESP_TYPE>(type) == RESP_TYPE::RT_baidu_text) {
            dealBaiDuOcrRequest(reply);
        } else if (static_cast<RESP_TYPE>(type) == RESP_TYPE::RT_youdao_img_translate) {
            dealYouDaoImgTranslateRequest(reply);
        } else {
            qDebug() << "***************** RESP_TYPE::RT_empty not match resp type! If you forget to set request->setAttribute(QNetworkRequest::User, nType);, it will look like this*****************";
        }

    } else {
        QStringList list;
        list << head.at(0) << reply->errorString();  // reply->errorString() 比 head.at(1) 更详细
    }

    if (reply)
        reply->deleteLater();  // 此处统一释放
}


