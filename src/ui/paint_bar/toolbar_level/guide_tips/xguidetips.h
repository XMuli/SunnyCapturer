#ifndef XGUIDETIPS_H
#define XGUIDETIPS_H

#include <QWidget>
#include <QLabel>
#include <QPointer>
#include "xguidebutton.h"
#include "../../screenshot/capturehelper.h"

// 引导式 - 操作透明窗口
namespace Ui {
class XGuideTips;
}

class XGuideTips : public QWidget
{
    Q_OBJECT

public:
    explicit XGuideTips(QWidget *parent = nullptr);
    ~XGuideTips();
    void initUI();

    ActionType actionType() const;
    void setActionType(const ActionType &newActionType);

    int textHeight() const;
    void setTextHeight(int newTextHeight);

protected:
    bool hideIfAllChildrenHidden();
    void autoShowGuideTips();
    void paintEvent(QPaintEvent *e) override;

private slots:
    void onLanguageChange(const QString& qm);

private:
    Ui::XGuideTips *ui;
    ActionType      m_type;         // 当前的操作状态
    int             m_textHeight;

    QPointer<XGuideButton> m_wsad;
    QPointer<XGuideButton> m_azimuthArrow;
    QPointer<XGuideButton> m_tab;
    QPointer<XGuideButton> m_quoteleft;
    QPointer<XGuideButton> m_shift;
    QPointer<XGuideButton> m_ctrl;
    QPointer<XGuideButton> m_mouseWheel;
    QPointer<XGuideButton> m_debug;

    QPointer<QLabel>       m_wsadLab;
    QPointer<QLabel>       m_azimuthArrowLab;
    QPointer<QLabel>       m_tabLab;
    QPointer<QLabel>       m_quoteleftLab;
    QPointer<QLabel>       m_shiftLab;
    QPointer<QLabel>       m_ctrlLab;
    QPointer<QLabel>       m_mouseWheelLab;
    QPointer<QLabel>       m_debugLab;
};

#endif // XGUIDETIPS_H
