#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "xwidgets.h"
//#include "xwidgets_global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 使用 class 示例
    Xwidgets xwidgets;
    int ret = xwidgets.addition(10, 20);
    qDebug() << "10 + 20 = " << ret;

    // 使用 普通函数 示例
    ret = subtraction(10, 3);
    qDebug() << "10 - 3 = " << ret;

    Widget w;
    w.show();
    return a.exec();
}
