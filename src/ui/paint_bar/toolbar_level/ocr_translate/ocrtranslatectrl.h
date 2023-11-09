#ifndef OCRTRANSLATECTRL_H
#define OCRTRANSLATECTRL_H

#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include <QLabel>
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

public:
    SwitchButton* m_swBtnShowModel;
    XToolButton*  m_tbCopy;

    QComboBox*    m_cbbFrom;
    QLabel*       m_lab;
    QComboBox*    m_cbbTo;
};

#endif // OCRTRANSLATECTRL_H
