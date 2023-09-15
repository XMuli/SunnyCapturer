#ifndef PASTE_H
#define PASTE_H

#include "xwidget_global.h"
#include <QWidget>

namespace Ui {
class Pin;
}

class Pin : public QWidget
{
    Q_OBJECT

public:
    explicit Pin(QWidget *parent = nullptr);
    ~Pin();

private:
    Ui::Pin *ui;
};

#endif // PASTE_H
