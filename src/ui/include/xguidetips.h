#ifndef XGUIDETIPS_H
#define XGUIDETIPS_H

#include <QWidget>


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

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    Ui::XGuideTips *ui;
};

#endif // XGUIDETIPS_H
