#include "watemarkplugin.h"
#include <QLabel>


WateMarkPlugin::WateMarkPlugin(QObject *parent)
{
    int a = 0;

}

const QString WateMarkPlugin::plugName() const
{
    return "WateMark";
}

QWidget *WateMarkPlugin::itemWidget()
{
    return new QLabel("test label");
}
