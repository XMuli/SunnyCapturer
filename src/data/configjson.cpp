#include "configjson.h"

#include <QByteArray>
#include <QFile>
#include <QApplication>
#include <QPen>
#include <QBrush>
#include <QString>
#include <QCryptographicHash>
#include "qaesencryption.h"


void ConfigJson::initJson()
{
    m_j = {
        {"general", {
                        {"language", "English"},
                        {"themes", "default"},
                        {"log_level", "Debug"},
                        {"font", "Microsoft YaHei,9"},
                        {"autostart", false}
                    }},
        {"interface", {
                          {"style", "Sunny"},
                          {"orientation", "Horizontal"},
                          {"highlight", "#0081ff"},
                          {"border_width", 3},
                          {"crosshair", "#df4187"},
                          {"crosshair_width", 2},
                          {"custom_size_enable", true},
                          {"topleft_enable", true},
                          {"size_enable", true},
                          {"delay_enable", true},
                          {"custom_rect_left", 100},
                          {"custom_rect_top", 100},
                          {"custom_rect_width", 640},
                          {"custom_rect_height", 480},
                          {"custom_dealy", 6},
                          {"acrylic_effect", true},
                          {"auto_detect_windows", true},
                          {"auto_copy_to_clipbaoard", true},
                          {"crosshair_show", false}
                      }},
        {"output", {
                       {"image_quailty", -1},
                       {"flie_name", "Sunny_$yyyyMMdd_hhmmss$.png"},
                       {"quick_save_enable", true},
                       {"auto_save_enable", false},
                       {"config_path", ""},
                       {"quick_save_path", ""},
                       {"auto_save_path", ""}
                   }},
        {"pin", {
                    {"opacity", 100},
                    {"maximum_size", 10000}
                }},
        {"hotkeys", {
                        {"capture", "F6"},
                        {"delay_capture", "Ctrl+F6"},
                        {"custom_capture", "Shift+F6"}
                    }},
        {"tokens", {
                       {"youdao_app_id", ""},
                       {"youdao_secret_key", ""},
                       {"iamge_translate_from_youdao", "auto"},
                       {"iamge_translate_to_youdao", "zh-CHS"},
                       {"iamge_translate_from_baidu", "auto"},
                       {"iamge_translate_to_baidu", "zh"},
                       {"baidu_api_key", ""},
                       {"baidu_secret_key", ""},
                       {"ocr_channel", "high"},
                       {"iamge_translate_channel", "baidu"}
                   }},
        {"advanced_control", {
                              {"blur_effect_adius", 20},
                              {"highlight_iridescence", "#DF4187, #FF5D00, #F8CB00, #23C400, #00A48A, #0081FF, #3C02FF, #8C00D4, #4D4D4D"},
                              {"crosshair_iridescence", "#000000, #7f7f7f, #880015, #ed1c24, #ff7f27, #fff200, #22b14c, #00a2e8, #3f48cc, #a349a4, #ffffff, #c3c3c3, #b97a57, #ffaec9, #ffc90e, #efe4b0, #b5e61d, #99d9ea, #7092be, #c8bfe7"},
                              {"show_develop_ui_log", false}
                          }},
        {"paint_bar_status", {
                                {"rect", false},
                                {"ellipse", false},
                                {"arrow", false},
                                {"penciler", false},
                                {"marker_pen", false},
                                {"mosaic", false},
                                {"serial", false},
                                {"rectintType", 0},
                                {"ellipseType", 0},
                                {"arrowType", 0},
                                {"marker_penType", 0},
                                {"mosaicType", 0},
                                {"pixelatedMosaic", 10},
                                {"smoothMosaic", 10},
                                {"textBold", false},
                                {"textItalic", false},
                                {"textOutline", false},
                                {"textStrikeout", false},
                                {"textUnderline", false},
                                {"fontFamily", "Microsoft YaHei"},
                                {"fontSize", 16},
                                {"serialType", 0},
                                {"serialNumber", 0},
                                {"serialLetter", "@Variant(\\0\\0\\0\\a\\0\\x61)"},
                                {"point", 0},
                                {"paPen", "@Variant(\\0\\0\\0M\\xa1@\\0\\0\\0\\0\\0\\0\\0\\x1\\x1\\xff\\xff\\xff\\xff\\0\\0\\0\\0\\0\\0@\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0)"},
                                {"paBrush", "@Variant(\\0\\0\\0\\x42\\x1\\x1\\xff\\xff\\xff\\xff\\0\\0\\0\\0\\0\\0)"}
                            }},
        {"developer_data", {
                           {"manual_save_image_dir", ""},              // 测试
                           {"detection_min_windows_level_depth", true}
                       }}
    };

    QString jsonString = QString::fromStdString(m_j.dump());
    jsonString.replace("\\", ""); // Remove escape characters
    qDebug().noquote() << "ConfigJson content:" << jsonString;
}

void ConfigJson::readFromFile()
{
    QFile file(m_jsonFile);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading:" << file.errorString();
        return;
    }

    QByteArray data = file.readAll();
    if (data.isEmpty()) return;
    m_j = ordered_json::parse(data.toStdString()); // Convert JSON data to ordered_json
    file.close();


    QString jsonString = QString::fromStdString(m_j.dump());
    jsonString.replace("\\", ""); // Remove escape characters
    qDebug().noquote() << "ConfigJson content:" << jsonString;
}

void ConfigJson::writeToFile()
{
    QFile file(m_jsonFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return;
    }

    file.write(QString::fromStdString(m_j.dump(4)).toUtf8()); // 4 个空格缩进
    file.close();
}

void ConfigJson::onSyncToFile()
{
    writeToFile();
}


void ConfigJson::setKeyValue(const QString &key, const ordered_json &val)
{
    QStringList keys = key.split('.'); // 使用点号分隔键
    ordered_json* currentObj = &m_j;
    for (const QString& k : keys) {
        currentObj = &((*currentObj)[k.toStdString()]);
    }
    *currentObj = val;
}

ordered_json ConfigJson::getKeyValue(const QString &key)
{
    QStringList keys = key.split('.'); // 使用点号分隔键
    ordered_json* currentObj = &m_j;
    for (const QString& k : keys) {
        auto it = currentObj->find(k.toStdString());
        if (it == currentObj->end()) {
            return ordered_json(); // 如果键不存在，返回默认值
        }
        currentObj = &it.value();
    }
    return *currentObj;
}

std::string ConfigJson::encryptString(const QString &input)
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QByteArray hashKey = QCryptographicHash::hash(m_key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(m_iv.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray encodeText = encryption.encode(input.toLocal8Bit(), hashKey, hashIV);

    return QString(encodeText.toBase64()).toStdString(); // 保存为 base64 的加密
}

QString ConfigJson::decryptString(const std::string&input)
{
    QByteArray byte = QByteArray::fromBase64(QString::fromStdString(input).toUtf8());

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QByteArray hashKey = QCryptographicHash::hash(m_key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(m_iv.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray decodeText = encryption.decode(byte, hashKey, hashIV);
    QString decodedString = QString(encryption.removePadding(decodeText));

    return decodedString;
}

ConfigJson::ConfigJson(QObject *parent)
    : QObject(parent)
    , m_j()
    , m_jsonFile("")
    , m_key("key-s%Prup7BWa6Wn%pB")
    , m_iv("IV-Q#3VZjPbmS!wZfZL")
{
    QString xconfigDir = qApp->applicationDirPath();
#if defined(Q_OS_LINUX)
    xconfigDir = QString("/usr/local/%1").arg(XPROJECT_NAME);
#endif

    m_jsonFile = xconfigDir + "/xconfig.json";

    readFromFile();
}
