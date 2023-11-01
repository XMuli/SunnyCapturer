#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H
#include <QWidget>
#include <QTimer>
#include <QColor>

class SwitchButton : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchButton(QWidget *parent = 0);
    ~SwitchButton(){}

signals:
    void statusChanged(bool checked);

private slots:
    void updateValue();

private:
    void drawBackGround(QPainter *painter);
    void drawSlider(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

private:
    int m_space;                //滑块距离边界距离
    int m_radius;               //圆角角度

    bool m_checked;             //是否选中
    bool m_showText;            //是否显示文字
    bool m_showCircle;          //是否显示圆圈
    bool m_animation;           //是否使用动画

    QColor m_bgColorOn;         //打开时候的背景色
    QColor m_bgColorOff;        //关闭时候的背景色
    QColor m_sliderColorOn;     //打开时候滑块颜色
    QColor m_sliderColorOff;    //关闭时候滑块颜色
    QColor m_textColor;         //文字颜色

    QString m_textOn;           //打开时候的文字
    QString m_textOff;          //关闭时候的文字

    QTimer  *m_timer;            //动画定时器
    int     m_step;             //动画步长
    int     m_startX;           //滑块开始X轴坐标
    int     m_endX;             //滑块结束X轴坐标

public:
    int space()                 const;
    int radius()                const;
    bool checked()              const;
    bool showText()             const;
    bool showCircel()           const;
    bool animation()            const;

    QColor bgColorOn()          const;
    QColor bgColorOff()         const;
    QColor sliderColorOn()      const;
    QColor sliderColorOff()     const;
    QColor textColor()          const;

    QString textOn()            const;
    QString textOff()           const;

    int step()                  const;
    int startX()                const;
    int endX()                  const;


public Q_SLOTS:
    void setSpace(int space);
    void setRadius(int radius);
    void setChecked(bool checked);
    void setShowText(bool show);
    void setShowCircle(bool show);
    void setAnimation(bool ok);

    void setBgColorOn(const QColor &color);
    void setBgColorOff(const QColor &color);
    void setSliderColorOn(const QColor &color);
    void setSliderColorOff(const QColor &color);
    void setTextColor(const QColor &color);

    void setTextOn(const QString &text);
    void setTextOff(const QString &text);
};

#endif // SWITCHBUTTON_H
