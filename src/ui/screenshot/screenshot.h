#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QWidget>

class ScreenShot : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenShot(QWidget *parent = nullptr);

signals:

};

#endif // SCREENSHOT_H
