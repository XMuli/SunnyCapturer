// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "xtextedit.h"
#include <QDebug>

XTextEdit::XTextEdit(QWidget* parent)
    : QTextEdit(parent)
    , m_focusAble(true)
{
    setStyleSheet(QStringLiteral("XTextEdit { background: transparent; }"));
    connect(this, &XTextEdit::textChanged, this, &XTextEdit::adjustSize);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::NoContextMenu);
}

void XTextEdit::showEvent(QShowEvent* e)
{
    QFont font;
    QFontMetrics fm(font);
    setFixedWidth(fm.lineSpacing() * 6);
    setFixedHeight(fm.lineSpacing() * 2.5);
    m_baseSize = size();
    m_minSize = m_baseSize;
    QTextEdit::showEvent(e);
    adjustSize();
}

void XTextEdit::resizeEvent(QResizeEvent* e)
{
    m_minSize.setHeight(qMin(m_baseSize.height(), height()));
    m_minSize.setWidth(qMin(m_baseSize.width(), width()));
    QTextEdit::resizeEvent(e);
}

void XTextEdit::focusInEvent(QFocusEvent *e)
{
    if (m_focusAble) {
        QTextEdit::focusInEvent(e);
    } else {
        e->ignore();
    }
}

void XTextEdit::focusOutEvent(QFocusEvent *e)
{
    if (m_focusAble) {
        QTextEdit::focusOutEvent(e);
    } else {
        e->ignore();
    }
}

void XTextEdit::mousePressEvent(QMouseEvent *e)
{
    if (m_focusAble) {
        QTextEdit::mousePressEvent(e);
    } else {
        e->ignore();
    }
}

void XTextEdit::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_focusAble) {
        QTextEdit::mouseReleaseEvent(e);
    } else {
        e->ignore();
    }
}

void XTextEdit::mouseMoveEvent(QMouseEvent *e)
{
    if (m_focusAble) {
        QTextEdit::mouseMoveEvent(e);
    } else {
        e->ignore();
    }
}

void XTextEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (m_focusAble) {
        QTextEdit::mouseDoubleClickEvent(e);
    } else {
        e->ignore();
    }
}

void XTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (m_focusAble) {
        QTextEdit::keyPressEvent(e);
    } else {
        e->ignore();
    }
}

void XTextEdit::keyReleaseEvent(QKeyEvent *e)
{
    if (m_focusAble) {
        QTextEdit::keyReleaseEvent(e);
    } else {
        e->ignore();
    }
}

void XTextEdit::setFont(const QFont& f)
{
    QTextEdit::setFont(f);
    adjustSize();
}

void XTextEdit::applyAllCharFormat(const QTextCharFormat& format)
{
    // 将新的格式应用于整个文本框中的所有文字
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::Document);
    m_lastFormat = currentCharFormat();
    cursor.mergeCharFormat(m_lastFormat);
}

void XTextEdit::adjustSize()
{
    QString&& text = this->toPlainText();

    if (text.isEmpty()) {
        mergeCurrentCharFormat(m_lastFormat);
    }

    QFontMetrics fm(font());
    QRect bounds = fm.boundingRect(QRect(), 0, text);
    int pixelsWide = bounds.width() + fm.lineSpacing();
    int pixelsHigh = bounds.height() * 1.15 + fm.lineSpacing();
    if (pixelsWide < m_minSize.width()) {
        pixelsWide = m_minSize.width();
    }
    if (pixelsHigh < m_minSize.height()) {
        pixelsHigh = m_minSize.height();
    }

    this->setFixedSize(pixelsWide, pixelsHigh);
}

bool XTextEdit::focusAble() const
{
    return m_focusAble;
}

void XTextEdit::setFocusAble(bool focusAble)
{
    m_focusAble = focusAble;

    if (m_focusAble)
        setStyleSheet(QStringLiteral("XTextEdit { background: transparent;}"));
    else
        setStyleSheet(QStringLiteral("XTextEdit { background: transparent; border: none; }"));
}
