#include "xtextedit.h"

XTextWidget::XTextWidget(QWidget* parent)
    : QTextEdit(parent)
{
    setStyleSheet(QStringLiteral("XTextWidget { background: transparent; }"));
    connect(this, &XTextWidget::textChanged, this, &XTextWidget::adjustSize);
    connect(this, &XTextWidget::textChanged, this, &XTextWidget::emitTextUpdated);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::NoContextMenu);
}

void XTextWidget::showEvent(QShowEvent* e)
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

void XTextWidget::resizeEvent(QResizeEvent* e)
{
    m_minSize.setHeight(qMin(m_baseSize.height(), height()));
    m_minSize.setWidth(qMin(m_baseSize.width(), width()));
    QTextEdit::resizeEvent(e);
}

void XTextWidget::setFont(const QFont& f)
{
    QTextEdit::setFont(f);
    adjustSize();
}

void XTextWidget::setAlignment(Qt::AlignmentFlag alignment)
{
    QTextEdit::setAlignment(alignment);
    adjustSize();
}
void XTextWidget::setTextColor(const QColor& c)
{
    QString s(
        QStringLiteral("XTextWidget { background: transparent; color: %1; }"));
    setStyleSheet(s.arg(c.name()));
}

void XTextWidget::adjustSize()
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

void XTextWidget::emitTextUpdated()
{
    emit textUpdated(this->toPlainText());
}
