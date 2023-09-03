#include "communication.h"

Communication::Communication(QObject *parent)
    : QObject{parent}
{
    init();
}

Communication &Communication::instance()
{
    static Communication ins;
    return ins;
}

void Communication::init()
{
}
