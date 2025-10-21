#ifndef WATEMARKPLUGIN_H
#define WATEMARKPLUGIN_H

#include <QWidget>
#include "../../src/pluginsinterface/iplugininterface.h"

class WateMarkPlugin : public QObject, IPluginInterface  // 必须多继承自 QObject 和抽象类；
{
    Q_OBJECT
    // Q_INTERFACES 的作用是将所实现的插件接口通知给元类型系统，参数是抽象类类名；
    // Q_PLUGIN_METADATA 用于声明元数据，IID 是必须且唯一的，FILE 是可选的，后面跟着一个json文件，用于描述插件的相关数据信息；
    Q_INTERFACES(IPluginInterface)
    Q_PLUGIN_METADATA(IID "XMuli.PicShot.PluginsInterface" FILE "watemark.json")
public:
    explicit WateMarkPlugin(QObject *parent = nullptr);

public:
    virtual const QString plugName() const override;
    virtual QWidget *itemWidget() override;
};

#endif // WATEMARKPLUGIN_H
