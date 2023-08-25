#ifndef MONITORS_H
#define MONITORS_H

#include <QObject>
#include <QScreen>
#include <QList>

// 获取原始监视器的信息，然后都存储起来
class Monitors : public QObject
{
    Q_OBJECT
public:
    explicit Monitors(QObject *parent = nullptr);
    virtual ~Monitors() = default;

//    QScreen *primaryScreen() const;
//    QList<QScreen *> screens() const;


};

#endif // MONITORS_H
