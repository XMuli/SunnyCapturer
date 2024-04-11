#ifndef CONFIGJSON_H
#define CONFIGJSON_H

#include <QObject>
#include <QPen>
#include <QBrush>
#include <QString>
#include <QDebug>
#include <QApplication>
#include <QStandardPaths>
#include <QDebug>
#include "isingleton.h"
#include "json.hpp"
using ordered_json = nlohmann::ordered_json;

#define CJ ConfigJson::instance()
#define CJ_SET(name, val) CJ.setKeyValue(QStringLiteral(name), val)
#define CJ_GET(name) CJ.getKeyValue(QStringLiteral(name))                                        // #define CJ_GET(name) CJ.getKeyValue(#name)   1.不要用 #name; 2.不要使用 dump() 会造成引号有两次
#define CJ_GET_STR(name) CJ.getKeyValue(QStringLiteral(name)).get<std::string>()
#define CJ_GET_QSTR(name) QString::fromStdString(CJ.getKeyValue(QStringLiteral(name)).get<std::string>())    // 直接获取单例里面的键值对
//-------------------------------------------------------------
#define CJ_CD CJ.m_cd  // 获取 ContextData m_cd

struct ContextData
{
    // 一级工具栏的状态
    bool rect = false;
    bool ellipse = false;
    bool arrow = false;
    bool penciler = false;
    bool marker_pen = false;
    bool mosaic = false;
    bool text = false;
    bool serial = false;

    // 二级工具栏的状态
    int rectintType = -1;
    int ellipseType = -1;
    int arrowType = -1;
    int marker_penType = -1;

    int mosaicType = -1;
    int pixelatedMosaic = -1;
    int smoothMosaic = -1;

    bool textBold = false;
    bool textItalic = false;
    bool textOutline = false;
    bool textStrikeout = false;
    bool textUnderline = false;
    QString fontFamily = "Microsoft YaHei";
    int fontSize = 16;

    int serialType = -1;
    int serialNumber = -1;
    QChar serialLetter = ' ';
    int pointType = -1;

    QPen paPen = QPen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QBrush paBrush = QBrush(Qt::red, Qt::SolidPattern);
};



/***************************************************************************************************************

*.ui  ⇆  ConfigJson::instance().ordered_json m_j  ⇆  xconfig.json

***************************************************************************************************************/

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
   ContextData m_cd;           // 初始工具栏的状态  -> 当关闭时刻，需要写入一边到文本保存？; 可写也可以不写。嘎

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
