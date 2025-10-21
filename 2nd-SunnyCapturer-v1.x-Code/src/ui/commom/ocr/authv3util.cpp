// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "authv3util.h"

#include <QUuid>
#include <QDateTime>
#include <QCryptographicHash>


void AuthV3Util::addAuthParams(const QString &appKey, const QString &appSecret, QMap<QString, QString> &params)
{
    QString q = params.value("q");
    if (q.isEmpty())
        q = params.value("img");

    QUuid uuid = QUuid::createUuid();
    QString uuidString = uuid.toString();
    QString salt = uuidString.mid(1, uuidString.length() - 2);  // 去掉首位的花括号
    QString curtime = QString::number(QDateTime::currentSecsSinceEpoch());

    QString sign = calculateSign(appKey, appSecret, q, salt, curtime);
    params["appKey"] = appKey;
    params["salt"] = salt;
    params["curtime"] = curtime;
    params["signType"] = "v3";
    params["sign"] = sign;
}

// QString strSrc = "5a3aa1167eed698d/9j/4AAQSk7840oAKKKKAP/Za8411026-7717-11ee-b342-d4d8537018851698665605tgjKTMUqEsG5ZysptJMHOk7pIPwFCi9T";
//  正确预期的结果应该为： '854e624bf786a916c1a8e6184ebacc5ba5e7bcdbf1151ef471c1899c9901c0c5'
QString AuthV3Util::calculateSign(const QString &appKey, const QString &appSecret, const QString &q, const QString &salt, const QString &curtime)
{
    QString strSrc = appKey + getInput(q) + salt + curtime + appSecret;
    QString strRet = encrypt(strSrc);   // [重点检查，看 Sha256 之后是否符合预期] 可尝试修改为如 strSrc.toStdString().c_str()
    return strRet;
}

QString AuthV3Util::encrypt(const QString &strSrc)
{
    QByteArray data = QCryptographicHash::hash(strSrc.toUtf8(), QCryptographicHash::Sha256);
    return QString(data.toHex());
}

QString AuthV3Util::getInput(const QString &input)
{
    if (input.isEmpty())
        return input;
    int inputLen = input.length();
    return (inputLen <= 20) ? input : (input.left(10) + QString::number(inputLen) + input.right(10));
}
