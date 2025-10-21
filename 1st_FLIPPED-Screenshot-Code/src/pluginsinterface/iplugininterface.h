// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef IPLUGININTERFACE_H
#define IPLUGININTERFACE_H

#include <QWidget>
#include <QString>

class IPluginInterface
{
public:
    virtual ~IPluginInterface() {};

    virtual const QString plugName() const { return QString("test plugName");}
    virtual QWidget*itemWidget() { return nullptr;}
};


QT_BEGIN_NAMESPACE
#define IPluginInterface_iid "XMuli.Flipped.PluginsInterface"  // 唯一标识符
Q_DECLARE_INTERFACE(IPluginInterface, IPluginInterface_iid)
QT_END_NAMESPACE

#endif // IPLUGININTERFACE_H


