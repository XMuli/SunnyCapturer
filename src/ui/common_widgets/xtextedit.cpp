#include "xtextedit.h"

XTextEdit::XTextEdit(QWidget* parent)
    : QTextEdit(parent)
    , m_focusAble(true)
{
    setStyleSheet(QStringLiteral("XTextEdit { background: transparent; }"));
    connect(this, &XTextEdit::textChanged, this, &XTextEdit::adjustSize);
    connect(this, &XTextEdit::textChanged, this, &XTextEdit::emitTextUpdated);
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

void XTextEdit::setAlignment(Qt::AlignmentFlag alignment)
{
    QTextEdit::setAlignment(alignment);
    adjustSize();
}
void XTextEdit::setTextColor(const QColor& c)
{
    QString s(
        QStringLiteral("XTextEdit { background: transparent; color: %1; }"));
    setStyleSheet(s.arg(c.name()));
}

void XTextEdit::adjustSize()
{
    QString&& text = this->toPlainText();

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

void XTextEdit::emitTextUpdated()
{
    emit textUpdated(this->toPlainText());
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
