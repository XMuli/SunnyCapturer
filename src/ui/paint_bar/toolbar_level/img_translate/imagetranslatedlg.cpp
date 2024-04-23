#include "imagetranslatedlg.h"
#include "ui_imagetranslatedlg.h"
#include "communication.h"

ImageTranslateDlg::ImageTranslateDlg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImageTranslateDlg)
{
    ui->setupUi(this);

    const int& iamge_translate_channel = CJ_GET("tokens.iamge_translate.channel");
    QString currLanguageFrom;
    QString currLanguageTo;
    if (iamge_translate_channel == 0) {  // TBD
        const auto& cbbFrom = languageMapCodeBaiDu();
        const auto& cbbTo = languageMapCodeBaiDu();
        currLanguageFrom = findKeyByValue(cbbFrom, CJ_GET_QSTR("tokens.iamge_translate.baidu.from"));
        currLanguageTo = findKeyByValue(cbbTo, CJ_GET_QSTR("tokens.iamge_translate.baidu.to"));

        for (const auto& it : cbbFrom) ui->cbbFrom->addItem(it.first, it.second);
        for (const auto& it : cbbTo) {
            if (it.second != "auto")
                ui->cbbTo->addItem(it.first, it.second);
        }

    } else if (iamge_translate_channel == 1) {
        const auto& cbbFrom = languageMapCodeYouDao();
        const auto& cbbTo = languageMapCodeYouDao();
        currLanguageFrom = findKeyByValue(cbbFrom, CJ_GET_QSTR("tokens.iamge_translate.youdao.from"));
        currLanguageTo = findKeyByValue(cbbTo, CJ_GET_QSTR("tokens.iamge_translate.youdao.to"));

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
    const int& iamge_translate_channel = CJ_GET("tokens.iamge_translate.channel");
    const auto& map = (iamge_translate_channel == 0) ? languageMapCodeBaiDu() : languageMapCodeYouDao();

    auto it = map.find(text);
    const auto& from = it != map.cend() ? it->second : "auto";
    iamge_translate_channel == 0 ?  CJ_SET("tokens.iamge_translate.baidu.from", from.toStdString()) : CJ_SET("tokens.iamge_translate.youdao.from", from.toStdString());

}
void ImageTranslateDlg::on_cbbTo_currentTextChanged(const QString &text)
{
    const int& iamge_translate_channel = CJ_GET("tokens.iamge_translate.channel");
    const auto& map = (iamge_translate_channel == 0) ? languageMapCodeBaiDu() : languageMapCodeYouDao();

    auto it = map.find(text);
    const auto& to = it != map.cend() ? it->second : "auto";
    iamge_translate_channel == 0 ?  CJ_SET("tokens.iamge_translate.baidu.to", to.toStdString()) : CJ_SET("tokens.iamge_translate.youdao.to", to.toStdString());
}
