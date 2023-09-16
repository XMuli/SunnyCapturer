#ifndef GENERAL_H
#define GENERAL_H

#include "xwidget_global.h"
#include <QWidget>

namespace Ui {
class General;
}

class General : public QWidget
{
    Q_OBJECT

public:
    explicit General(QWidget *parent = nullptr);
    ~General();

private slots:
    void on_cbbLanguage_currentTextChanged(const QString &arg1);
    void on_cbbLogLevel_currentTextChanged(const QString &arg1);
    void on_btnFont_released();
    void on_cbAutostart_clicked(bool checked);

private:
    void initUI();

private:
    Ui::General *ui;
};

#endif // GENERAL_H
