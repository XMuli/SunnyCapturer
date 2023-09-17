#ifndef INTERFACE_H
#define INTERFACE_H

#include "xwidget_global.h"
#include <QWidget>

namespace Ui {
class Interface;
}

enum class StyleType
{
    ST_sunny,
    ST_macos,
    ST_dde
};
Q_DECLARE_METATYPE(StyleType)                     // 可以被 QVariant 类型存储

class Interface : public QWidget
{
    Q_OBJECT

public:
    explicit Interface(QWidget *parent = nullptr);
    ~Interface();

private slots:
    void on_cbbStyle_currentTextChanged(const QString &arg1);
    void onHighlightPickedColor(const QColor& color);
    void onCrosshairPickedColor(const QColor& color);
    void on_sbBorderWidth_valueChanged(int arg1);
    void on_sbCrosshairWidth_valueChanged(int arg1);
    void on_cbAcrylicEffect_clicked(bool checked);
    void on_cbAutoDetectWindows_clicked(bool checked);
    void on_cbAutoCopyToClipboard_clicked(bool checked);
    void on_cbCrosshairShow_clicked(bool checked);

public slots:
    void onBtnResetClicked(bool checked = false);

private:
    void initUI();

private:
    Ui::Interface *ui;
};

#endif // INTERFACE_H
