#ifndef SIDESETTINGUI_H
#define SIDESETTINGUI_H


#include <QWidget>

namespace Ui {
class SideSettingUI;
}

class SideSettingUI : public QWidget
{
    Q_OBJECT

public:
    explicit SideSettingUI(QWidget *parent = nullptr);
    ~SideSettingUI();

private:
    Ui::SideSettingUI *ui;
};

#endif // SIDESETTINGUI_H
