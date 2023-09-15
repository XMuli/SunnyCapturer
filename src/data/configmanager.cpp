#include "configmanager.h"
#include <QApplication>





void ConfigManager::setIniValue(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
    m_settings->sync(); // 将更改立即写入到配置文件
}

QVariant ConfigManager::getIniValue(const QString &key, const QVariant &defaultValue) const
{
    return m_settings->value(key, defaultValue);
}

//void ConfigManager::initIniFile()
//{

//}

//void ConfigManager::init()
//{


//}

ConfigManager::ConfigManager(QObject *parent)
    : QObject{parent}
    , m_settings(new QSettings(qApp->applicationDirPath() + "/xconfig.ini", QSettings::IniFormat, this))
{
    m_settings->setIniCodec("UTF-8"); // 禁用自动转义

    init();
}
