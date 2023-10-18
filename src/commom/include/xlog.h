#ifndef XLOG_H
#define XLOG_H

#include <QObject>
#include <QtGlobal>
#include <QString>
#include <QByteArray>
#include <QMessageLogContext>

void XMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);


#endif // XLOG_H
