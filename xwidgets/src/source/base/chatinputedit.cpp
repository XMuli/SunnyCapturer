#include "chatinputedit.h"
#include <QDebug>

ChatInputEdit::ChatInputEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
}

void ChatInputEdit::keyReleaseEvent(QKeyEvent *event)
{
//    qDebug() << "Release key:" << QKeySequence(event->modifiers()).toString() << QKeySequence(event->key()).toString();

    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        event->ignore(); // 标记为未处理，继续传递事件给父类的事件处理函数

        if (event->modifiers() == Qt::ShiftModifier || event->modifiers() == Qt::ControlModifier) { // 按下的是 Shift/Ctrl + Enter
            emit sigCustomKeyBtnReleased(CustomKey::CK_CRLF);

            if (event->modifiers() == Qt::ShiftModifier) {
//                qDebug() << "ChatInputPlainTextEdit: emit CustomKey::CK_CRLF()! ShiftModifier";
                // shift + enter 可以正常换行
            } else if (event->modifiers() == Qt::ControlModifier) {
//                qDebug() << "ChatInputPlainTextEdit: emit CustomKey::CK_CRLF() ControlModifier";
                insertPlainText("\n");  // 很奇怪，appendPlainText 则不可以正常换行
            }
        } else { // 仅 Enter
            emit sigCustomKeyBtnReleased(CustomKey::CK_EnterSend);
//            qDebug() << "ChatInputPlainTextEdit: emit CustomKey::CK_EnterSend()!";
            return;
        }
    }

    QPlainTextEdit::keyPressEvent(event);
}
