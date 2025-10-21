// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XQTLOG_H
#define XQTLOG_H

#include <QObject>

void CustomOutputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg);

class XQtLog : public QObject
{
    Q_OBJECT
public:
    explicit XQtLog(QObject *parent = nullptr);
};

#endif // XQTLOG_H
