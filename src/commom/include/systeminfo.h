#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <QObject>
#include <QScreen>
#include <QGuiApplication>
#include <QList>
#include "isingleton.h"

#define SYSTEMINFO SystemInfo::instance()

class SystemInfo : public QObject, public ISingleton<SystemInfo>
{
    Q_OBJECT
    FRIEND_CREAT_SINGLETION(SystemInfo);
public:
    const QStringList detailedInfo() const;

    QString windowsVersionInfo();

    QStringList scrnsInfo() const;
    QString virGeometryInfo() const;
    QPixmap renderMonitorToPixmap();

private:
    double scaling(const QScreen *screen = qGuiApp->primaryScreen()) const;

#if defined(_MSC_VER)
    QString getRegistryValue(const QString& keyPath, const QString& valueName);
    QString getCPUInfo();
    QString getMemoryInfo();
#elif defined(__GNUC__)
#elif defined(__clang__)
#endif

private:
    SystemInfo(QObject *parent = nullptr);
    virtual ~SystemInfo() = default;

private:
    QList<QScreen *> m_scrns;
    QScreen* m_priScrn;
};

#endif // SYSTEMINFO_H
