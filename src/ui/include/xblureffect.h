#ifndef XBLUREFFECT_H
#define XBLUREFFECT_H

#include <QWidget>

// 绘制一个 模糊效果的窗口，需要传递一个 QPixmap 作为底层
class XBlurEffect : public QWidget
{
    Q_OBJECT
public:
    explicit XBlurEffect(QWidget *parent = nullptr);
    void setPixmap(const QPixmap &pix, int radius);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QPixmap m_pixmap;         // 绘画本窗口的底层
};

#endif // XBLUREFFECT_H
