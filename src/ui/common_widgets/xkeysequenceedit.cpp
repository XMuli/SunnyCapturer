#include "xkeysequenceedit.h"
#include <QWidget>
#include <QMouseEvent>
#include <QDebug>

/*!
 * \brief The XKeySequenceEdit class 设置仅接受一个快捷键组合；删除清空
 */

XKeySequenceEdit::XKeySequenceEdit(QWidget *parent)
    : QKeySequenceEdit(parent)
{
    initUI();
}

XKeySequenceEdit::XKeySequenceEdit(const QKeySequence &keySequence, QWidget *parent)
    : QKeySequenceEdit(keySequence, parent)
{
    initUI();
}

void XKeySequenceEdit::initUI()
{
    //setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
}

//void XKeySequenceEdit::test()
//{
//	connect(this, &XKeySequenceEdit::keySequenceChanged, this, &XKeySequenceEdit::onKeySequenceChanged);

//	connect(this, &XKeySequenceEdit::editingFinished, this, &XKeySequenceEdit::onEditingFinished);
//}

//void XKeySequenceEdit::onEditingFinished()
//{
//	qInfo() << "editingFinished    ------------>" << keySequence() << keySequence().count();
//}

//void XKeySequenceEdit::onKeySequenceChanged(const QKeySequence & keySequence)
//{
//	qInfo() << "keySequenceChanged ------------>" << keySequence << keySequence.count();
//}

// Ref: https://jishurizhi.com/p-74.html
void XKeySequenceEdit::keyPressEvent(QKeyEvent *event)
{
    QKeySequenceEdit::keyPressEvent(event);

    QKeySequence keySeq = keySequence();
//    qInfo() << "keySeq:" << keySeq << "keySeq.count():" << keySeq.count();

    // Fix: 按下相同快捷键编辑框为空白
    if ( keySeq.count() <= 0)
        return;

    int key = keySeq[0];
//    qDebug()<<"keySeq[0]:"<<keySeq[0];
    if (key == Qt::Key_Backspace || key == Qt::Key_Delete)
        key = 0;

    setKeySequence(key);
    emit sigKeySeqChanged(key); // 要发射对应信号
}

//void XKeySequenceEdit::mousePressEvent(QMouseEvent* event)
//{
//    int a = 0;
//    if (event->button() != Qt::LeftButton)
//        return;
//
//    if (!geometry().contains(mapFromGlobal(QCursor::pos())))
//        clearFocus();
//}
