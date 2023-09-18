#ifndef TIPS_H
#define TIPS_H

#include <QColor>
#include <QWidget>
#include <QRect>

enum class TipsType{
    TT_empty,
    TT_picked_rect_tips,
    TT_point_changed_tips,
    TT_countdown_tips,  // 延迟截图时刻的-倒计时
    TT_keyboard_operation_tips
};

// information tips: 矩尺寸，画笔 point-with， operation tips
class Tips : public QWidget
{
    Q_OBJECT
public:
    explicit Tips(const QString& text, const TipsType& type = TipsType::TT_empty, QWidget *parent = nullptr);

    void setColor(const QColor &newColor = QColor(0, 0, 0, 0.4 * 255));
    void setText(const QString &newText);
    void setType(TipsType newType);

    QRect textRect(const QString &text);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor    m_color;
    QString   m_text;
    TipsType  m_type;
};

#endif // TIPS_H
