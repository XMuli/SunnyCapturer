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

private:
    Ui::General *ui;
};

#endif // GENERAL_H
