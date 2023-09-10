#include "../include/xlog.h"
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QApplication>
#include <QMutex>
#include <QMutexLocker>

// 自定义的 qDebug、 QWaring 等输出，在 main() 的前面添加 qInstallMessageHandler(XMessageOutput);
void XMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    QString msgType("");
    switch (type) {
    case QtDebugMsg: {
        msgType = "Debug";
        break;
    } case QtInfoMsg: {
        msgType = "Info";
        break;
    } case QtWarningMsg: {
        msgType = "Warning";
        break;
    } case QtCriticalMsg: {
        msgType = "Critical";
        break;
    } case QtFatalMsg: {
        msgType = "Fatal";
        break;
    } default:
        break;
    }

    // 设置输出信息格式
    const QString& time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ssss");


#ifdef LOG_MORE_INFO_DEVELOP
    QString logMsg = QString("[%1] [%2] [%3] File: %4  Line: %5 Function: %6\n")
                         .arg(time).arg(msgType).arg(msg.toLocal8Bit().constData()).arg(context.file).arg(context.line).arg(context.function);
#else
    QString logMsg = QString("[%1] [%2] %3\n")
                         .arg(time).arg(msgType).arg(msg.toLocal8Bit().constData());
#endif



    if (logMsg.isEmpty()) return;
    const QString appDir = qApp->applicationDirPath();

    QDir dir(appDir);
    if (!dir.exists()) dir.mkdir(appDir);
    QFile file(appDir + "/Sunny.log");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Append)) {  // 输出信息至文件中（读写、追加形式）
        file.close();
        return;
    }

    QTextStream stream(&file);
    stream << logMsg;
    file.flush();
    file.close();
}
