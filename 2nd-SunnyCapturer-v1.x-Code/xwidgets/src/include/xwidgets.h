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

// 对普通的函数， .h 声明， .cpp 实现; 或者在 .h 中声明且实现; 则在外部函数中，是会调用均可以成功
extern "C" Q_DECL_EXPORT int subtraction(int a, int b);


#endif // XWIDGETS_H
