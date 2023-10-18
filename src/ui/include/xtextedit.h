#ifndef XTEXTEDIT_H
#define XTEXTEDIT_H

#include <QFocusEvent>
#include <QTextEdit>

class XTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit XTextEdit(QWidget* parent = nullptr);

    void adjustSize();
    void setFont(const QFont& f);

    bool focusAble() const;
    void setFocusAble(bool focusAble);

protected:
    void showEvent(QShowEvent* e) override;
    void resizeEvent(QResizeEvent* e) override;
    void focusInEvent(QFocusEvent *e) override;
    void focusOutEvent(QFocusEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

signals:
    void textUpdated(const QString& s);

public slots:
    void setTextColor(const QColor& c);
    void setAlignment(Qt::AlignmentFlag alignment);

private slots:
    void emitTextUpdated();

private:
    bool  m_focusAble;  // true-支持获取焦点， false-不接受焦点获取
    QSize m_baseSize;
    QSize m_minSize;
};

#endif // XTEXTEDIT_H
