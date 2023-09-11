#ifndef XTEXTEDIT_H
#define XTEXTEDIT_H

#include <QTextEdit>

class XTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit XTextEdit(QWidget* parent = nullptr);

    void adjustSize();
    void setFont(const QFont& f);

protected:
    void showEvent(QShowEvent* e);
    void resizeEvent(QResizeEvent* e);

signals:
    void textUpdated(const QString& s);

public slots:
    void setTextColor(const QColor& c);
    void setAlignment(Qt::AlignmentFlag alignment);

private slots:
    void emitTextUpdated();

private:
    QSize m_baseSize;
    QSize m_minSize;
};

#endif // XTEXTEDIT_H
