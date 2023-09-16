#ifndef HOTKEYS_H
#define HOTKEYS_H

#include "xwidget_global.h"
#include <QWidget>

namespace Ui {
class Hotkeys;
}

class Hotkeys : public QWidget
{
    Q_OBJECT

public:
    explicit Hotkeys(QWidget *parent = nullptr);
    ~Hotkeys();

private slots:
    void onKeySeqChanged(const QKeySequence &keySequence);

public slots:
    void onBtnResetClicked(bool checked = false);



private:
    void initUI();

private:
    Ui::Hotkeys *ui;
};

#endif // HOTKEYS_H
