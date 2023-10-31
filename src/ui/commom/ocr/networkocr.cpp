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
#include "authv3util.h"
#include "communication.h"
#include "json.hpp"
#include "qmetaobject.h"
using json = nlohmann::json;

// 您的应用 ID / 应用密钥
QString APP_KEY = "5a3aa1167eed698d";
QString APP_SECRET = "tgjKTMUqEsG5ZysptJMHOk7pIPwFCi9T";

NetworkOCR::NetworkOCR(QObject *parent)
    : QObject{parent}
    , m_networkManager(new QNetworkAccessManager(this))
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &NetworkOCR::onRequestFinished);

    sendBaiDuAccessToken();
}

void NetworkOCR::sendBaiDuOcrTextRequest(const QString &path)
{
    // 从文件中读取图像数据并进行base64编码
    QUrl url("https://aip.baidubce.com/rest/2.0/ocr/v1/accurate?access_token=24.220a36c4d45a235d7f7ae1d302f3f458.2592000.1701329540.282335-42093112"/*m_baiDuToken*/);
    QNetworkRequest request(url);
    QByteArray postData = "image=" + QUrl::toPercentEncoding(base64FromFileContent(path)) + "&detect_direction=false&vertexes_location=false&paragraph=false&probability=false";
//    qDebug() << "postData:" << postData;

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Accept", "application/json");
    request.setAttribute(QNetworkRequest::User, int(RESP_TYPE::RT_baidu_ocr_text));

    m_networkManager->post(request, postData);
}

void NetworkOCR::sendYouDaoOcrTranslateRequest(const QString &path)
{
    QString base64DataUtf8 = base64FromFileContent(path);

    // 构造POST请求
    QNetworkRequest request(QUrl("https://openapi.youdao.com/ocrtransapi"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, int(RESP_TYPE::RT_youdao_ocr_translate));

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
                QMessageBox::warning(nullptr, title, msg); // 暂时屏蔽
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
        }
    } else {
        qWarning() << replyErrorShowText(dataHead);
    }
}

void NetworkOCR::dealBaiDuOcrTextRequest(QNetworkReply *reply)
{
    const QStringList& dataHead = rawHeader(reply);
    QByteArray response = reply->readAll();
        qDebug().noquote() << " dealBaiDuOcrTextRequest reply -> Content:" << response;
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

//        if (!j.empty() && j.contains("access_token")) {
//            m_baiDuToken = QString::fromStdString(j["access_token"]);
//            qDebug() << "m_baiDuToken:" << m_baiDuToken;
//        }
    } else {
        qWarning() << replyErrorShowText(dataHead);
    }
}

void NetworkOCR::dealYouDaoOcrTranslateRequest(QNetworkReply *reply)
{
    const QStringList& dataHead = rawHeader(reply);
    QByteArray response = reply->readAll();
    qDebug().noquote() << " dealYouDaoOcrTranslateRequest reply -> Content:" << response;
    if (response.isEmpty()) {
        qWarning()<<"reply->readAll() is empty!  retrn;!!!";
        return;
    }

    if (dataHead.at(0) == "200") {
        // 解析 JSON 数据
        json j;
        try {
            j = json::parse(response.toStdString());
        } catch (const std::exception& e) {
            qDebug() << "Failed to parse JSON:" << e.what();
            return;
        }

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

        }

        //            // 反向 URL 解码数据以查看 +
        //            QString decodedData = QUrl::fromPercentEncoding(responseData);
        //            qDebug() << "decodedData:" << decodedData;

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
        } else if (static_cast<RESP_TYPE>(type) == RESP_TYPE::RT_baidu_ocr_text) {
            dealBaiDuOcrTextRequest(reply);
        } else if (static_cast<RESP_TYPE>(type) == RESP_TYPE::RT_youdao_ocr_translate) {
            dealYouDaoOcrTranslateRequest(reply);
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


