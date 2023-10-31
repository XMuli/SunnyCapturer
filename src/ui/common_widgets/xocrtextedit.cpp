#include "xocrtextedit.h"


XOcrTextEdit::XOcrTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    initUI();
}

XOcrTextEdit::XOcrTextEdit(const QString &text, QWidget *parent)
    : QTextEdit(text, parent)
{
    initUI();
}

void XOcrTextEdit::initUI()
{
//    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::NoContextMenu);
}
