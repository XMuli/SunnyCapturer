#include "imagetranslatedlg.h"
#include "ui_imagetranslatedlg.h"
#include "communication.h"

ImageTranslateDlg::ImageTranslateDlg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImageTranslateDlg)
{
    ui->setupUi(this);

    const auto& cbbFrom = youDaoLanguageMapCode();
    const auto& cbbTo = youDaoLanguageMapCode();
    const auto& currLanguageFrom = findKeyByValue(cbbFrom, CONF_GET_PROPERTY(XTokens_iamge_translate_from).toString());
    const auto& currLanguageTo = findKeyByValue(cbbTo, CONF_GET_PROPERTY(XTokens_iamge_translate_to).toString());

    for (const auto& it : cbbFrom) ui->cbbFrom->addItem(it.first, it.second);
    for (const auto& it : cbbTo) ui->cbbTo->addItem(it.first, it.second);

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
    const auto& map = youDaoLanguageMapCode();
    auto it = map.find(text);
    const auto& from = it != map.cend() ? it->second : "auto";;
    CONF_SET_PROPERTY(XTokens_iamge_translate_from, from);
}
void ImageTranslateDlg::on_cbbTo_currentTextChanged(const QString &text)
{
    const auto& map = youDaoLanguageMapCode();
    auto it = map.find(text);
    const auto& to = it != map.cend() ? it->second : "auto";
    CONF_SET_PROPERTY(XTokens_iamge_translate_to, to);
}
