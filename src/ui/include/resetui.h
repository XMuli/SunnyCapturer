#ifndef RESETUI_H
#define RESETUI_H

#include <QWidget>

namespace Ui {
class ResetUI;
}

class ResetUI : public QWidget
{
    Q_OBJECT

public:
    explicit ResetUI(QWidget *parent = nullptr);
    ~ResetUI();

private:
    Ui::ResetUI *ui;
};

#endif // RESETUI_H
