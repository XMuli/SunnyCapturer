#ifndef OUTPUT_H
#define OUTPUT_H

#include "xwidget_global.h"
#include <QWidget>

namespace Ui {
class Output;
}

class Output : public QWidget
{
    Q_OBJECT

public:
    explicit Output(QWidget *parent = nullptr);
    ~Output();

private:
    void initUI();

private slots:
    void on_sbImageQuailty_valueChanged(int arg1);
    void on_leFileName_textChanged(const QString &arg1);
    void on_leConfigPath_textChanged(const QString &arg1);
    void on_leQuickSavePath_textChanged(const QString &arg1);
    void on_leAutoSavePath_textChanged(const QString &arg1);
    void onSleletedDir();

public slots:
    void onBtnResetClicked(bool checked = false);
    void onLanguageChange(const QString qmName);

private:
    Ui::Output *ui;
};

#endif // OUTPUT_H
