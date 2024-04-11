#ifndef CONFIGJSON_H
#define CONFIGJSON_H

#include <QObject>
#include <QDebug>
#include "isingleton.h"
#include "json.hpp"
using ordered_json = nlohmann::ordered_json;

#define CJ ConfigJson::instance()
#define CJ_SET(name, val) CJ.setKeyValue(#name, val)    // 直接设置单例里面的键值对
#define CJ_GET(name) CJ.getKeyValue(#name)              // 直接获取单例里面的键值对


class ConfigJson : public QObject, public ISingleton<ConfigJson>
{
    Q_OBJECT
    FRIEND_CREAT_SINGLETION(ConfigJson);

public:
    // 便于开发测试，才将在这三个设置为 public 的
    void initJson();
    void readFromFile();
    void writeToFile();

    void onSyncToFile();
    void setKeyValue(const QString& key, const ordered_json& val); // 设置多层键值对
    ordered_json getKeyValue(const QString& key);    // 获取多层键值对

    // 加密解密 API-Key
    std::string encryptString(const QString& input);
    QString decryptString(const std::string& input);

public:
//    PaintBarStatus m_paintBarStatus;           // 初始工具栏的状态  -> 当关闭时刻，需要写入一边到文本保存？; 可写也可以不写。嘎

private:
    explicit ConfigJson(QObject *parent = nullptr);
    virtual ~ConfigJson() = default;

private:
    ordered_json m_j;
    QString m_jsonFile;
    QString m_key;
    QString m_iv;
};

#endif // CONFIGJSON_H
