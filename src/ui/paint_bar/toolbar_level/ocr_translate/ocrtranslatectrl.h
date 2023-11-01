#ifndef OCRTRANSLATECTRL_H
#define OCRTRANSLATECTRL_H

#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include "switchbutton.h"
#include "xtoolbutton.h"
#include "../common/absbtnsctrl.h"

class OcrTranslateCtrl : public AbsBtnsCtrl
{
    Q_OBJECT
public:
    explicit OcrTranslateCtrl(const Qt::Orientations& orien, QWidget *parent = nullptr);
    virtual ~OcrTranslateCtrl() = default;

private:
    void initUI();


private:
    SwitchButton* m_swBtnShowModel;
    XToolButton*  m_tbModelSwitch;
    QComboBox*    m_cbbFrom;
    QComboBox*    m_cbbTo;

    XToolButton*  m_tbdit;
    XToolButton*  m_tbCopy;
    XToolButton*  m_tbUpdate;

};

#endif // OCRTRANSLATECTRL_H
