#ifndef IMAGETRANSLATEDLG_H
#define IMAGETRANSLATEDLG_H

#include <QWidget>

namespace Ui {
class ImageTranslateDlg;
}

class ImageTranslateDlg : public QWidget
{
    Q_OBJECT

public:
    explicit ImageTranslateDlg(QWidget *parent = nullptr);
    ~ImageTranslateDlg();

    void setLeftPixmap(const QPixmap& pix);
    void setRightPixmap(const QSize &size, const QString &path);

private slots:
    void on_cbbFrom_currentTextChanged(const QString &text);
    void on_cbbTo_currentTextChanged(const QString &text);

private:
    Ui::ImageTranslateDlg *ui;
};

#endif // IMAGETRANSLATEDLG_H
