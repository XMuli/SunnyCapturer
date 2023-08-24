#ifndef XWIDGETS_H
#define XWIDGETS_H

#include "xwidgets_global.h"

// 对于类， .h 声明， .cpp 实现; 在外部函数中，是可以直接调用的；
class XWIDGETS_EXPORT Xwidgets
{
public:
    Xwidgets();

    int addition(int a, int b);
};

// 对普通的函数， .h 声明， .cpp 实现; 在外部函数中，是会调用失败； 但是可以在 .h 里面声明+实现，则外部可以直接调用
extern "C" __declspec(dllexport) int subtraction(int a, int b)
{
    qDebug() << "call subtraction(), does not class member subtraction:";
    return a - b;
}

#endif // XWIDGETS_H
