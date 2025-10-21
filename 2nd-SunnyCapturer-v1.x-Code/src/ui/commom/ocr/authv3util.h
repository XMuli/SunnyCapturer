// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef AUTHV3UTIL_H
#define AUTHV3UTIL_H
#include <QString>
#include <QMap>

class AuthV3Util
{
public:
    static void addAuthParams(const QString& appKey, const QString& appSecret, QMap<QString, QString>& params);
    static QString calculateSign(const QString& appKey, const QString& appSecret, const QString& q, const QString& salt, const QString& curtime);
    static QString encrypt(const QString& strSrc);
    static QString getInput(const QString& input);
};

#endif // AUTHV3UTIL_H
