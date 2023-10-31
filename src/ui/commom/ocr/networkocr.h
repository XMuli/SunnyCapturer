#ifndef NETWORKOCR_H
#define NETWORKOCR_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QPointer>

// youdao API: https://ai.youdao.com/DOCSIRMA/html/trans/api/tpfy/index.html
// 在线测试图片的 base64: https://tool.jisuapi.com/base642pic.html


/*! BaiDu:
    AppID:       42093112
    API Key:     BFuLK0VlWbH8l2WeLlDapfhc
    Secret Key:  b2QwlwEb2uRCUt4s6XethYtjbNHpzdtR
 */

class NetworkOCR : public QObject
{
    Q_OBJECT
public:
    enum class RESP_TYPE {
        RT_empty,                      // 忘记头文件加此，则解析默认为这个
        RT_baidu_access_token,         // 获取 token
        RT_baidu_ocr_text,             // 百度 OCR 转换为文本
        RT_baidu_ocr_translate,        // 百度 OCR 转换为翻译后的图片

        RT_youdao_ocr_translate        // 有道 OCR 识别后转换为翻译的图片
    };
    Q_ENUM(RESP_TYPE)

    explicit NetworkOCR(QObject *parent = nullptr);
    virtual ~NetworkOCR() = default;
    void sendBaiDuOcrTextRequest(const QString& path);           // 待翻译图片路径
    void sendYouDaoOcrTranslateRequest(const QString& path);     // 待翻译图片路径

private:
    // base
    const QStringList rawHeader(const QNetworkReply* reply) const;                                 // 解析头部
    const QString replyErrorShowText(const QStringList& dataHead);
    QString base64FromFileContent(const QString& path, bool urlencoded = false);                   // 图片转 base64
    // BaiDu
    const bool validityBaiDuKey(const QString& client_id, const QString& client_secret) const;             // 校验 BaiDu key 的有效性
    void sendBaiDuAccessToken(const QString& client_id = "BFuLK0VlWbH8l2WeLlDapfhc", const QString& client_secret = "b2QwlwEb2uRCUt4s6XethYtjbNHpzdtR");  // 获取 BaiDu 的鉴权

    void dealBaiDuAccessToken(QNetworkReply* reply);
    void dealBaiDuOcrTextRequest(QNetworkReply* reply);
    void dealYouDaoOcrTranslateRequest(QNetworkReply* reply);


private slots:
    void onRequestFinished(QNetworkReply* reply);

private:
    QString                              m_baiDuToken;
    QPointer<QNetworkAccessManager>      m_networkManager;
};

#endif // NETWORKOCR_H
