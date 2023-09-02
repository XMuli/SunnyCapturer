#include "communication.h"

Communication::Communication(QObject *parent)
    : QObject{parent}
{
    init();
}

QWidget *Communication::screenShotPtr() const
{
    return m_screenShotPtr;
}

Communication &Communication::instance()
{
    static Communication ins;
    return ins;
}

void Communication::init()
{
    connect(this, &Communication::sigScreenShotPtr, this, &Communication::onScreenShotPtr);
}

void Communication::onScreenShotPtr(QWidget *w)
{
    m_screenShotPtr = w;
}
