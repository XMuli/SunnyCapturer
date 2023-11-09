#ifndef XOCRTRANSLATECTRL_H
#define XOCRTRANSLATECTRL_H

#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include "switchbutton.h"
#include "xtoolbutton.h"
#include "../common/absbtnsctrl.h"

class XOcrTextCtrl : public AbsBtnsCtrl
{
    Q_OBJECT

public:
    explicit XOcrTextCtrl(const Qt::Orientations& orien, QWidget *parent = nullptr);
    virtual ~XOcrTextCtrl() = default;

private:
    void initUI();


private:
    SwitchButton* m_swBtnShowModel;
    XToolButton*  m_tbModelSwitch;
    QComboBox*    m_cbbFrom;
    QComboBox*    m_cbbTo;

public:
    XToolButton*  m_tbdit;
    XToolButton*  m_tbCopy;
    XToolButton*  m_tbUpdate;
};

#endif // XOCRTRANSLATECTRL_H
