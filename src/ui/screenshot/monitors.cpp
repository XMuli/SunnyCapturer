#include "monitors.h"
#include <QGuiApplication>

Monitors::Monitors(QObject *parent)
    : QObject{parent}
//    , m_primaryScreen(qGuiApp->primaryScreen())
//    , m_screens(qGuiApp->screens())
{
}

//QScreen *Monitors::primaryScreen() const
//{
//    return m_primaryScreen;
//}

//QList<QScreen *> Monitors::screens() const
//{
//    return m_screens;
//}
