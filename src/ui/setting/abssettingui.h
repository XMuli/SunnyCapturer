#ifndef ABSSETTINGUI_H
#define ABSSETTINGUI_H

#include <QWidget>

namespace Ui {
class AbsSettingUI;
}

class AbsSettingUI : public QWidget
{
    Q_OBJECT

public:
    explicit AbsSettingUI(QWidget *parent = nullptr);
    ~AbsSettingUI();

    void insertWidget(int index, QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());

private:
    void initUI();

signals:
    void sigBtnHintClicked(bool checked = false);
    void sigBtnResetClicked(bool checked = false);

private:
    Ui::AbsSettingUI *ui;
};

#endif // ABSSETTINGUI_H
