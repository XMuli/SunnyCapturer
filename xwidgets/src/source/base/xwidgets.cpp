#include "xwidgets.h"

Xwidgets::Xwidgets()
{
}

int Xwidgets::addition(int a, int b)
{
    qDebug() << "call Xwidgets::addition()";
    return a + b;
}

int subtraction(int a, int b)
{
    qDebug() << "call subtraction(), does not class member subtraction:";
    return a - b;
}
