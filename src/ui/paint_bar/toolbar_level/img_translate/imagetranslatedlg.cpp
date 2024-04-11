#include "imagetranslatedlg.h"
#include "ui_imagetranslatedlg.h"
#include "communication.h"

ImageTranslateDlg::ImageTranslateDlg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImageTranslateDlg)
{
    ui->setupUi(this);

//    const QString& iamge_translate_channel = CONF_GET_PROPERTY(XTokens_iamge_translate_channel).toString();
    const QString& iamge_translate_channel = CJ_GET_QSTR("tokens.iamge_translate_channel");
    QString currLanguageFrom;
    QString currLanguageTo;
    if (iamge_translate_channel == "baidu") {
        const auto& cbbFrom = languageMapCodeBaiDu();
        const auto& cbbTo = languageMapCodeBaiDu();
//        currLanguageFrom = findKeyByValue(cbbFrom, CONF_GET_PROPERTY(XTokens_iamge_translate_from_baidu).toString());
//        currLanguageTo = findKeyByValue(cbbTo, CONF_GET_PROPERTY(XTokens_iamge_translate_to_baidu).toString());
        currLanguageFrom = findKeyByValue(cbbFrom, CJ_GET_QSTR("tokens.iamge_translate_from_baidu"));
        currLanguageTo = findKeyByValue(cbbTo, CJ_GET_QSTR("tokens.iamge_translate_to_baidu"));

        for (const auto& it : cbbFrom) ui->cbbFrom->addItem(it.first, it.second);
        for (const auto& it : cbbTo) {
            if (it.second != "auto")
                ui->cbbTo->addItem(it.first, it.second);
        }

    } else if (iamge_translate_channel == "youdao") {
        const auto& cbbFrom = languageMapCodeYouDao();
        const auto& cbbTo = languageMapCodeYouDao();
//        currLanguageFrom = findKeyByValue(cbbFrom, CONF_GET_PROPERTY(XTokens_iamge_translate_from_youdao).toString());
//        currLanguageTo = findKeyByValue(cbbTo, CONF_GET_PROPERTY(XTokens_iamge_translate_to_youdao).toString());
        currLanguageFrom = findKeyByValue(cbbFrom, CJ_GET_QSTR("tokens.iamge_translate_from_youdao"));
        currLanguageTo = findKeyByValue(cbbTo, CJ_GET_QSTR("tokens.iamge_translate_to_youdao"));

        for (const auto& it : cbbFrom) ui->cbbFrom->addItem(it.first, it.second);
        for (const auto& it : cbbTo) {
            if (it.second != "auto")
                ui->cbbTo->addItem(it.first, it.second);
        }

    }


    ui->cbbFrom->setCurrentText(currLanguageFrom);
    ui->cbbTo->setCurrentText(currLanguageTo);
}

ImageTranslateDlg::~ImageTranslateDlg()
{
    delete ui;
}

void ImageTranslateDlg::setLeftPixmap(const QPixmap &pix)
{
    if (!pix.isNull())
        ui->labLeft->setPixmap(pix);
}

void ImageTranslateDlg::setRightPixmap(const QSize &size, const QString &path)
{
    if (size.isEmpty())
        ui->labRight->setText(path);
    else
        ui->labRight->setPixmap(QPixmap(path));

}

void ImageTranslateDlg::on_cbbFrom_currentTextChanged(const QString &text)
{
//    const QString& iamge_translate_channel = CONF_GET_PROPERTY(XTokens_iamge_translate_channel).toString();
    const QString& iamge_translate_channel = CJ_GET_QSTR("tokens.iamge_translate_channel");
    const auto& map = (iamge_translate_channel == "baidu") ? languageMapCodeBaiDu() : languageMapCodeYouDao();

    auto it = map.find(text);
    const auto& from = it != map.cend() ? it->second : "auto";;


//    iamge_translate_channel == "baidu" ?  CONF_SET_PROPERTY(XTokens_iamge_translate_from_baidu, from) : CONF_SET_PROPERTY(XTokens_iamge_translate_from_youdao, from);
    iamge_translate_channel == "baidu" ?  CJ_SET("tokens.iamge_translate_from_baidu", from.toStdString()) : CJ_SET("tokens.iamge_translate_from_youdao", from.toStdString());

}
void ImageTranslateDlg::on_cbbTo_currentTextChanged(const QString &text)
{
//    const QString& iamge_translate_channel = CONF_GET_PROPERTY(XTokens_iamge_translate_channel).toString();
    const QString& iamge_translate_channel = CJ_GET_QSTR("tokens.iamge_translate_channel");
    const auto& map = (iamge_translate_channel == "baidu") ? languageMapCodeBaiDu() : languageMapCodeYouDao();

    auto it = map.find(text);
    const auto& to = it != map.cend() ? it->second : "auto";
//    iamge_translate_channel == "baidu" ?  CONF_SET_PROPERTY(XTokens_iamge_translate_to_baidu, to) : CONF_SET_PROPERTY(XTokens_iamge_translate_to_youdao, to);
    iamge_translate_channel == "baidu" ?  CJ_SET("tokens.iamge_translate_to_baidu", to.toStdString()) : CJ_SET("tokens.iamge_translate_to_youdao", to.toStdString());
}
