#ifndef XOCRTEXTEDIT_H
#define XOCRTEXTEDIT_H
#include <QTextEdit>

// 定制显示 OCR Text 的组件
class XOcrTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit XOcrTextEdit(QWidget* parent = nullptr);
    explicit XOcrTextEdit(const QString &text, QWidget *parent = nullptr);

private:
    void initUI();
};

#endif // XOCRTEXTEDIT_H
