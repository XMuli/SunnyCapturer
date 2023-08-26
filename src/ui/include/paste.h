#ifndef PASTE_H
#define PASTE_H

#include "xwidget_global.h"
#include <QWidget>

namespace Ui {
class Paste;
}

class Paste : public QWidget
{
    Q_OBJECT

public:
    explicit Paste(QWidget *parent = nullptr);
    ~Paste();

private:
    Ui::Paste *ui;
};

#endif // PASTE_H
