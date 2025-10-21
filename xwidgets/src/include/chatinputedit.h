#ifndef CHATINPUTEDIT_H
#define CHATINPUTEDIT_H

#include "xwidgets_global.h"
#include <QPlainTextEdit>

enum class CustomKey {
    CK_EnterSend,   // 忽略回车，发送消息
    CK_CRLF           // carriage return/line feed 回车换行
};

class XWIDGETS_EXPORT ChatInputEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit ChatInputEdit(QWidget* parent = nullptr);

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

signals:
    void sigCustomKeyBtnReleased(const CustomKey& customKey);
};


#endif // CHATINPUTEDIT_H
