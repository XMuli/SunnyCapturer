#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QWidget>
#include "../../paint_bar/paintbarhelper.h"

#define COMM Communication::instance()

class Communication : public QObject
{
    Q_OBJECT

public:
    static Communication& instance(); // signle
    void init();

signals:
    void sigWidgetResized();                                                      // paintBtnsBar 控件布局变换完成
    void sigPaintBtnRelease(const PaintType& type, const bool& isCheckable);      // paintBtnsBar 按钮被按下
    void sigPaintCtrlIdReleased(const int& id);                                      // PaintCtrlBar 按钮被按下
    void sigPaintCtrlIdReleasedFromPointCtrl(const int& id);


private:
    explicit Communication(QObject *parent = nullptr);
    virtual ~Communication() = default;
    Communication(const Communication&) = delete;
    Communication& operator=(const Communication&) = delete;
    Communication(Communication&&) = delete;
    Communication& operator=(Communication&&) = delete;
};

#endif // COMMUNICATION_H
