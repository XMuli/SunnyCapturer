#include <QApplication>
#include "sidesettingui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SideSettingUI w;
    w.show();
    return a.exec();
}
