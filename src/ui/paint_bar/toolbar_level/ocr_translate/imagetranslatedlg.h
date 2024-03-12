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

private:
    Ui::ImageTranslateDlg *ui;
};

#endif // IMAGETRANSLATEDLG_H
