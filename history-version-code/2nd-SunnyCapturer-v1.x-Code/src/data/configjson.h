#ifndef CONFIGJSON_H
#define CONFIGJSON_H

#include <QObject>
#include <QPen>
#include <QChar>
#include <QBrush>
#include <QDebug>
#include <QString>
#include <QApplication>
#include <QStandardPaths>
#include <QFont>
#include "isingleton.h"
#include "json.hpp"
using ordered_json = nlohmann::ordered_json;

#define CJ ConfigJson::instance()
#define CJ_SET(name, val) CJ.setKeyValue(QStringLiteral(name), val)

// GET 多层时候采用 . 句号分割
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
    int rectintType = 0;           // 若是 -1 则开始没有这个数值
    int ellipseType = 0;
    int arrowType = 0;
    int pointType = 0;          // 三个点，对应 penciler 的顺序
    int marker_penType = 0;

    int mosaicType = 0;
    int pixelatedMosaic = 10;
    int smoothMosaic = 10;
    bool textBold = false;
    bool textItalic = false;
    bool textOutline = false;
    bool textStrikeout = false;
    bool textUnderline = false;

    int serialType = 0;
    int serialNumber = 0;
    QChar serialLetter = 'a';
    QFont font = QFont("Microsoft YaHei", 10);
    QPen pen = QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QBrush brush = QBrush(Qt::red, Qt::SolidPattern);

    // 其它特殊
    bool isShowCollimatorCursor = false;

    void cdReadFromFile();
};


void cdWritToFile(ContextData& cd);

/***************************************************************************************************************

*.ui  ⇆  ConfigJson::instance().ordered_json m_j  ⇆  xconfig.json

***************************************************************************************************************/


class ConfigJson : public QObject, public ISingleton<ConfigJson>
{
    Q_OBJECT
    FRIEND_CREAT_SINGLETION(ConfigJson);

public:  // 便于开发测试，才将在这三个设置为 public 的
    void initPrepare();             // 准备工作.比如创建文件夹之类
    void initJsonToFile();          // 初始化且写入一份默认的 .json 文件

    ordered_json defaultConfigJson();   // 一个原始的 json 初始化字符串
    void readFromFile();
    void writeToFile();

public:
    void initOutputDefaulValue();
    void setJ(const std::string& key, const ordered_json &newJ);

    void onSyncToFile();
    void setKeyValue(const QString& key, const ordered_json& val); // 设置多层键值对
    ordered_json getKeyValue(const QString& key);    // 获取多层键值对

    // 加密解密 API-Key
    std::string encryptString(const QString& input);
    QString decryptString(const std::string& input);

public:
   ContextData m_cd;           // 初始工具栏的状态  -> 当关闭时刻，需要写入一边到文本保存？; 可写也可以不写。嘎

private:
    void createDefaultConfigInLinux();
    ordered_json m_j;
    explicit ConfigJson(QObject *parent = nullptr);
    virtual ~ConfigJson() = default;

private:
    QString m_jFilePath;
    QString m_key;
    QString m_iv;
};

#endif // CONFIGJSON_H
