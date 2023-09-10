#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QWidget>

#define COMM Communication::instance()

class Communication : public QObject
{
    Q_OBJECT

public:
    static Communication& instance(); // signle
    void init();



private:
    explicit Communication(QObject *parent = nullptr);
    virtual ~Communication() = default;
    Communication(const Communication&) = delete;
    Communication& operator=(const Communication&) = delete;
    Communication(Communication&&) = delete;
    Communication& operator=(Communication&&) = delete;
};

#endif // COMMUNICATION_H
