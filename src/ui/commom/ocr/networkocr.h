// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef NETWORKOCR_H
#define NETWORKOCR_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QPointer>
#include <QEventLoop>
#include "../../data/configmanager.h"
#include "../paint_bar/toolbar_level/paintbarhelper.h"

// youdao API: https://ai.youdao.com/DOCSIRMA/html/trans/api/tpfy/index.html
// 在线测试图片的 base64: https://tool.jisuapi.com/base642pic.html

//class OcrTranslateData;
//class OcrTextData;
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
    void sendBaiDuOcrTextRequest(const OcrTextData& data, const QString& path);           // 待翻译图片路径

    void sendBaiDuOcrTranslateRequest(const OcrTranslateData& data, const QString& path);     // 待翻译图片路径   https://cloud.baidu.com/doc/MT/s/mki483xpu 【还在写中】
    void sendYouDaoOcrTranslateRequest(const OcrTranslateData& data, const QString& path);     // 待翻译图片路径

private:
    // base
    const QStringList rawHeader(const QNetworkReply* reply) const;                                 // 解析头部
    const QString replyErrorShowText(const QStringList& dataHead);
    QString base64FromFileContent(const QString& path, bool urlencoded = false);                   // 图片转 base64
    // BaiDu
    const bool validityBaiDuKey(const QString& client_id, const QString& client_secret) const;     // 校验 BaiDu key 的有效性
    void sendBaiDuAccessToken(const QString& client_id = CONF_MANAGE.decryptString(CONF_GET_PROPERTY(XTokens_baidu_api_key).toByteArray())
                              , const QString& client_secret = CONF_MANAGE.decryptString(CONF_GET_PROPERTY(XTokens_baidu_secret_key).toByteArray()));  // 获取 BaiDu 的鉴权

    void dealBaiDuAccessToken(QNetworkReply* reply);
    void dealBaiDuOcrTextRequest(QNetworkReply* reply);
    void dealYouDaoOcrTranslateRequest(QNetworkReply* reply);


private slots:
    void onRequestFinished(QNetworkReply* reply);

private:
    QString                              m_baiDuToken;
    QPointer<QNetworkAccessManager>      m_networkManager;
//    QPointer<QEventLoop>                 m_eventLoop;  // 获取 m_token 后才能够使用相关 API
};

#endif // NETWORKOCR_H
