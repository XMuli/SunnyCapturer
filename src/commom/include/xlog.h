// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XLOG_H
#define XLOG_H

#include <QObject>
#include <QtGlobal>
#include <QString>
#include <QByteArray>
#include <QMessageLogContext>

void easylogingppMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);   // easylogingpp 的

void customQtMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);               // 自定义封装的

#endif // XLOG_H
