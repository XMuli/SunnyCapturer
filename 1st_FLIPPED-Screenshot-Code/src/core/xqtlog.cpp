// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "xqtlog.h"
#include <iostream>
#include <mutex>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <QMutex>
#include <QStandardPaths>
#include <QMessageBox>

#define LOG_FILE_KEEP_NUM 7

// https://blog.csdn.net/lileiyu1/article/details/78279514
// https://blog.csdn.net/liang19890820/article/details/51838379
// https://www.coologic.cn/2018/01/880/
void CustomOutputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    // Lock to prevent qdebug from crashing too often in multiple threads
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString msgType;
    switch (type) {
        case QtDebugMsg: {
            msgType = "Debug";
            break;
            }
        case QtInfoMsg: {
            msgType = "Info";
            break;
            }
        case QtWarningMsg: {
            msgType = "Warning";
            break;
            }
        case QtCriticalMsg: {
            msgType = "Critical";
            break;
            }
        case QtFatalMsg: {
            msgType = "Fatal";
            QMessageBox::critical(nullptr, msgType, QString("[%1] [%2] [%3] Function: %4  File: %5  Line: %6\n").arg(curTime).arg(msgType).arg(msg).arg(context.function).arg(context.file).arg(context.line));
            abort();
            break;
            }
        default:
            break;
    }

    QString logMsg = QString("[%1] [%2] [%3] Function: %4  File: %5  Line: %6\n").arg(curTime).arg(msgType).arg(msg).arg(context.function).arg(context.file).arg(context.line);
    if (logMsg.isEmpty())
        return;

    // Create and write files
    // QString path = QString::fromStdString(QStandardPaths::standardLocations(QStandardPaths::TempLocation).first().toStdString()) + "/flipped_log";
    QString path = qApp->applicationDirPath();

    QDir dir(path);
    if (!dir.exists())
        dir.mkdir(path);

    QString currentDate = QDateTime::currentDateTime().toString("yyyyMMdd");
    QString logName = "log_flipped_" + currentDate + ".txt";
    QString logFileName = path + "/" + logName;

    static std::once_flag flag1;
    std::call_once(flag1, [logFileName](){ qDebug() << "log path:" << logFileName; });

    if (!QFile::exists(logFileName)) {
        QFileInfoList fileList = dir.entryInfoList(QStringList() << "*.txt", QDir::NoFilter, QDir::Time);
        if (fileList.size() >= LOG_FILE_KEEP_NUM) {
            int i = 1;
            foreach(QFileInfo fileInfo, fileList) {
                if (i >= LOG_FILE_KEEP_NUM) {
                    QString fileName = fileInfo.absoluteFilePath();
                    QFile::remove(fileName);
                }
                i++;
            }
        }
    }

    QFile file(logFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append /*QIODevice::Truncate*/ | QIODevice::Text)) {
        file.close();
        return;
    }

    QTextStream ts(&file);
    ts << logMsg;
    file.close();
}

XQtLog::XQtLog(QObject *parent)
    : QObject{parent}
{

}
