#ifndef TRAY_H
#define TRAY_H

#include <QObject>

class Tray : public QObject
{
    Q_OBJECT
public:
    explicit Tray(QObject *parent = nullptr);

signals:

};

#endif // TRAY_H
