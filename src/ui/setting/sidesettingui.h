#ifndef SIDESETTINGUI_H
#define SIDESETTINGUI_H

#include <QButtonGroup>
#include <QPointer>
#include <QWidget>
#include "about.h"
#include "abssettingui.h"

namespace Ui {
class SideSettingUI;
}

class SideSettingUI : public QWidget
{
    Q_OBJECT

public:
    explicit SideSettingUI(QWidget *parent = nullptr);
    virtual ~SideSettingUI();

private:
    void initUI();

private:
    void onSideGroupChanged(int idx);

public slots:
    void onBtnHintClicked(bool checked = false);

protected:
    void closeEvent(QCloseEvent *e) override;

private:
    Ui::SideSettingUI *ui;

    QPointer<QButtonGroup>    m_sideGroup;
    QPointer<AbsSettingUI>    m_general;
    QPointer<AbsSettingUI>    m_interface;
    QPointer<AbsSettingUI>    m_output;
    QPointer<AbsSettingUI>    m_pin;
    QPointer<AbsSettingUI>    m_hotkeys;
    QPointer<AbsSettingUI>    m_tokens;
    QPointer<About>           m_about;
};

#endif // SIDESETTINGUI_H

