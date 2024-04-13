#include "configjson.h"
#include <Qt>
#include <QFile>
#include <QPen>
#include <QBrush>
#include <QString>
#include <QByteArray>
#include <QMutex>
#include <QMetaEnum>
#include <QApplication>
#include <QCryptographicHash>
#include "qaesencryption.h"

ordered_json ConfigJson::defaultConfigJson()
{
    ordered_json j = {
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
        {"painter_context_data", {
                                {"rect", false},
                                {"ellipse", false},
                                {"arrow", false},
                                {"penciler", false},
                                {"marker_pen", false},
                                {"mosaic", false},
                                {"text", false},
                                {"serial", false},
                                {"rectintType", 0},
                                {"ellipseType", 0},
                                {"arrowType", 0},
                                {"pointType", 0},
                                {"marker_penType", 0},
                                {"mosaicType", 0},
                                {"pixelatedMosaic", 10},
                                {"smoothMosaic", 10},
                                {"textBold", false},
                                {"textItalic", false},
                                {"textOutline", false},
                                {"textStrikeout", false},
                                {"textUnderline", false},
                                  {"serial_info", {
                                              {"type", 0},
                                              {"number", 0},
                                              {"letter", "a"}}},
                                  {"font", {
                                            {"family", "Microsoft YaHei"},
                                            {"pointSize", 16},
                                            {"bold", false},
                                            {"italic", false},
                                            {"underline", false},
                                            {"strikeOut", false}}},
                                  {"pen", {
                                              {"color", "#ffff0000"},
                                              {"width", 4},
                                              {"style", "SolidLine"},
                                              {"capStyle", "RoundCap"},
                                              {"joinStyle", "RoundJoin"},}},
                                  {"brush", {
                                             {"color", "#ffff0000"},
                                             {"style", "#SolidPattern"},
                                             }}
                                 }},
        {"developer_data", {
                            {"manual_save_image_dir", ""},              // 测试
                            {"detection_min_windows_level_depth", true}
                       }}
    };

    QString jsonString = QString::fromStdString(j.dump());
    qDebug().noquote() << "ordered_json j:" << jsonString;
    return j;
}

// void ConfigJson::initConfigJson()
// {
//     m_j = defaultConfigJson();
// }

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
    // qDebug().noquote() << "m_j content:" << jsonString;
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

void ConfigJson::initOutputDefaulValue()
{
    // fix: 初次读配置文件, Output path 为空; 此时单例还没有构造完成，不能用宏
    // setKeyValue("output.flie_name", QString("%1_$yyyyMMdd_hhmmss$.png").arg(XPROJECT_NAME).toStdString());
    setKeyValue("output.config_path", qApp->applicationDirPath().toStdString());
    setKeyValue("output.quick_save_path", QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first().toStdString());
    setKeyValue("output.auto_save_path", QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first().toStdString());
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
        currentObj = &((*currentObj)[k.toUtf8().constData()]);
    }
    *currentObj = val;
}

ordered_json ConfigJson::getKeyValue(const QString &key)
{
    QStringList keys = key.split('.'); // 使用点号分隔键
    ordered_json* currentObj = &m_j;
    for (const QString& k : keys) {
        auto it = currentObj->find(k.toStdString());
        if (it == currentObj->end())
            return ordered_json(); // 如果键不存在，返回默认值

        currentObj = &it.value();
    }

    return *currentObj;
}

std::string ConfigJson::encryptString(const QString &input)
{
    if (input.isEmpty()) return std::string();

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QByteArray hashKey = QCryptographicHash::hash(m_key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(m_iv.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray encodeText = encryption.encode(input.toLocal8Bit(), hashKey, hashIV);

    return QString(encodeText.toBase64()).toStdString(); // 保存为 base64 的加密
}

QString ConfigJson::decryptString(const std::string&input)
{
    if (input.empty()) return QString();

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


void cdWritToFile(ContextData &cd)
{
#define CJ_SET_CD(name) CJ.setKeyValue(QStringLiteral("painter_context_data.") + QStringLiteral(#name).mid(QStringLiteral(#name).lastIndexOf(".") + 1), name);
#define CJ_SET_QVariant(pre, name, var) CJ.setKeyValue(QStringLiteral("painter_context_data.") + pre + "." + name, var); // QPen QBrush 专属
//    CJ.setKeyValue("painter_context_data.rect", true);

    // 一级工具栏的状态
    CJ_SET_CD(cd.rect)
    CJ_SET_CD(cd.ellipse)
    CJ_SET_CD(cd.arrow)
    CJ_SET_CD(cd.penciler)
    CJ_SET_CD(cd.marker_pen)
    CJ_SET_CD(cd.mosaic)
    CJ_SET_CD(cd.text)

    // 二级工具栏的状态
    CJ_SET_CD(cd.rectintType)
    CJ_SET_CD(cd.ellipseType)
    CJ_SET_CD(cd.arrowType)
    CJ_SET_CD(cd.marker_penType)
    CJ_SET_CD(cd.pointType)

    CJ_SET_CD(cd.mosaicType)
    CJ_SET_CD(cd.pixelatedMosaic)
    CJ_SET_CD(cd.smoothMosaic)

    CJ_SET_CD(cd.textBold)
    CJ_SET_CD(cd.textItalic)
    CJ_SET_CD(cd.textOutline)
    CJ_SET_CD(cd.textStrikeout)
    CJ_SET_CD(cd.textUnderline)

    // serial_info 三个改写为Json对象了
    CJ_SET_QVariant("serial_info", "type", cd.serialType);
    CJ_SET_QVariant("serial_info", "number", cd.serialNumber);
    CJ_SET_QVariant("serial_info", "letter", QString(cd.serialLetter).toStdString());

    // QFont
    const QFont& font = cd.font;
    CJ_SET_QVariant("font", "family", font.family().toStdString())
    CJ_SET_QVariant("font", "pointSize", font.pointSize())
    CJ_SET_QVariant("font", "bold", font.bold())
    CJ_SET_QVariant("font", "italic", font.italic())
    CJ_SET_QVariant("font", "underline", font.underline())
    CJ_SET_QVariant("font", "strikeOut", font.strikeOut())
    // QPen
    CJ_SET_QVariant("pen", "color", cd.pen.color().name(QColor::HexArgb).toStdString())                           // 颜色
    CJ_SET_QVariant("pen", "width", cd.pen.width())                                                               // 线宽
    CJ_SET_QVariant("pen", "style", QMetaEnum::fromType<Qt::PenStyle>().valueToKey(cd.pen.style()))               // 线型
    CJ_SET_QVariant("pen", "capStyle", QMetaEnum::fromType<Qt::PenCapStyle>().valueToKey(cd.pen.capStyle()))      // 端点样式
    CJ_SET_QVariant("pen", "joinStyle", QMetaEnum::fromType<Qt::PenJoinStyle>().valueToKey(cd.pen.joinStyle()))   // 接合点样式
    // QBrush
    CJ_SET_QVariant("brush", "color", cd.brush.color().name(QColor::HexArgb).toStdString())                       // 颜色
    CJ_SET_QVariant("brush", "style", QMetaEnum::fromType<Qt::BrushStyle>().valueToKey(cd.brush.style()))         // 样式
}

void ContextData::cdReadFromFile()
{
#define CJ_GET_CONTEXT(name) name = CJ.getKeyValue(QStringLiteral("painter_context_data.") + #name)
#define CJ_GET_CONTEXT_OBJECT(pre, name) CJ.getKeyValue(QStringLiteral("painter_context_data.")  + pre + "." + name)
#define CJ_GET_CONTEXT_STR(pre, name) CJ_GET_CONTEXT_OBJECT(pre, name).get<std::string>()
#define CJ_GET_CONTEXT_QSTR(pre, name) QString::fromStdString(CJ_GET_CONTEXT_OBJECT(pre, name).get<std::string>())

    // 一级工具栏的状态
    rect = CJ.getKeyValue("painter_context_data.rect").get<bool>();
    CJ_GET_CONTEXT(rect).get<bool>();
    CJ_GET_CONTEXT(ellipse).get<bool>();
    CJ_GET_CONTEXT(arrow).get<bool>();
    CJ_GET_CONTEXT(penciler).get<bool>();
    CJ_GET_CONTEXT(marker_pen).get<bool>();
    CJ_GET_CONTEXT(mosaic).get<bool>();
    CJ_GET_CONTEXT(text).get<bool>();

    // 二级工具栏的状态
    CJ_GET_CONTEXT(rectintType);
    CJ_GET_CONTEXT(ellipseType);
    CJ_GET_CONTEXT(arrowType);
    CJ_GET_CONTEXT(marker_penType);
    CJ_GET_CONTEXT(pointType);

    CJ_GET_CONTEXT(mosaicType);
    CJ_GET_CONTEXT(pixelatedMosaic);
    CJ_GET_CONTEXT(smoothMosaic);

    CJ_GET_CONTEXT(textBold).get<bool>();
    CJ_GET_CONTEXT(textItalic).get<bool>();
    CJ_GET_CONTEXT(textOutline).get<bool>();
    CJ_GET_CONTEXT(textStrikeout).get<bool>();
    CJ_GET_CONTEXT(textUnderline).get<bool>();

    // serial_info 三个改写为数组了
    serialType = CJ_GET_CONTEXT_OBJECT("serial_info", "type");
    serialNumber = CJ_GET_CONTEXT_OBJECT("serial_info", "number");
    serialLetter = QString::fromStdString(CJ_GET_CONTEXT_OBJECT("serial_info", "letter").get<std::string>()).at(0);

    // QFont
//    auto t1 = QString::fromStdString(CJ_GET_CONTEXT_OBJECT("font", "family").get<std::string>());
    font.setFamily(CJ_GET_CONTEXT_QSTR("font", "family"));
    font.setPointSize(CJ_GET_CONTEXT_OBJECT("font", "pointSize"));
    font.setBold(CJ_GET_CONTEXT_OBJECT("font", "bold").get<bool>());
    font.setItalic(CJ_GET_CONTEXT_OBJECT("font", "italic").get<bool>());
    font.setUnderline(CJ_GET_CONTEXT_OBJECT("font", "underline").get<bool>());
    font.setStrikeOut(CJ_GET_CONTEXT_OBJECT("font", "strikeOut").get<bool>());
    // QPen
    pen.setColor(CJ_GET_CONTEXT_QSTR("pen", "color"));            // 颜色
    pen.setWidth(CJ_GET_CONTEXT_OBJECT("pen", "width"));
    pen.setStyle(static_cast<Qt::PenStyle>(QMetaEnum::fromType<Qt::PenStyle>().keyToValue(CJ_GET_CONTEXT_STR("pen", "style").c_str())));
    pen.setCapStyle(static_cast<Qt::PenCapStyle>(QMetaEnum::fromType<Qt::PenCapStyle>().keyToValue(CJ_GET_CONTEXT_STR("pen", "capStyle").c_str())));
    pen.setJoinStyle(static_cast<Qt::PenJoinStyle>(QMetaEnum::fromType<Qt::PenJoinStyle>().keyToValue(CJ_GET_CONTEXT_STR("pen", "joinStyle").c_str())));
    // QBrush
    brush.setColor(CJ_GET_CONTEXT_QSTR("brush", "color"));
    brush.setStyle(static_cast<Qt::BrushStyle>(QMetaEnum::fromType<Qt::BrushStyle>().keyToValue(CJ_GET_CONTEXT_STR("brush", "style").c_str())));
}

void ConfigJson::setJ(const std::string &key, const ordered_json &newJ)
{
    m_j[key] = newJ;
}




