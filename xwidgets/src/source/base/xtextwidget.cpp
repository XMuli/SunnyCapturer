#include "xtextwidget.h"
#include <QFontMetrics>
#include <QTextCharFormat>
#include <QPen>
#include <QDebug>

XTextWidget::XTextWidget(QWidget *parent)
    : QTextEdit(parent)
    , m_baseSize()
    , m_minSize()
{
    //qDebug() << "XTextWidget::XTextWidget()" << Qt::endl << "--a1--rect:" << rect() << "  size:" << size();
    
    //setStyleSheet(QStringLiteral("XTextWidget { background: transparent; }"));
    connect(this, &XTextWidget::textChanged, this, &XTextWidget::adjustSize);
    connect(this, &XTextWidget::textChanged, this, &XTextWidget::emitTextUpdated);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::NoContextMenu);
}

XTextWidget::~XTextWidget()
{
    //qDebug() << "XTextWidget::~XTextWidget()" << Qt::endl << "rect:" << rect() << "  size:" << size();
}

// https://blog.csdn.net/kenfan1647/article/details/115171891
void XTextWidget::adjustSize()
{
    //qDebug() << "XTextWidget::adjustSize()" << Qt::endl << "--a1--rect:" << rect();

    QString&& text = this->toPlainText();
    QFontMetrics fm(font());
    QRect bound = fm.boundingRect(QRect(), Qt::AlignLeft, text);  // 需要再研究下
    int pixelsWidth = qMax<double>(bound.width() + fm.lineSpacing(), m_minSize.width()) ;
    int pixelsHigh = qMax<double>(bound.height() * 1.15 + fm.lineSpacing(), m_minSize.height());
    this->setFixedSize(pixelsWidth, pixelsHigh);
    //qDebug() << "--a3--rect:" << rect();
    //qDebug() << "bounds:" << bounds << "  fm.lineSpacing:" << fm.lineSpacing()
    //    << "  pixelsWide:" << pixelsWide << "  pixelsHigh:" << pixelsHigh << "  m_minSize:" << m_minSize;
}

void XTextWidget::setFont(const QFont & font)
{
    //qDebug() << "XTextWidget::setFont()" << Qt::endl << "rect:" << rect();
    QTextEdit::setFont(font);
    adjustSize();
}

void XTextWidget::showEvent(QShowEvent *e)
{
    QFont font;
    QFontMetrics fm(font);  // 赋值一个初始的宽高

    //qDebug() << "XTextWidget::showEvent()" << Qt::endl << "--a1--rect:" << rect() << "  fm.lineSpacing:" << fm.lineSpacing() << "  size:" << size();
    setFixedWidth(fm.lineSpacing() * 6);
    setFixedHeight(fm.lineSpacing() * 2.5);
    m_baseSize = size();
    m_minSize = m_baseSize;

    QTextEdit::showEvent(e);
    adjustSize();

    //qDebug() << "--a2--rect:" << rect() << "  fm.lineSpacing:" << fm.lineSpacing() << "  size:" << size();
}

// 当 widget 大小被重新设置的时候就会被调用
void XTextWidget::resizeEvent(QResizeEvent *e)
{
    //qDebug() << "XTextWidget::showEvent()" << Qt::endl << "--a1--rect:" << rect()  << "  size:" << size();
    m_minSize.setHeight(qMin(m_baseSize.height(), height()));
    m_minSize.setWidth(qMin(m_baseSize.width(), width()));
    QTextEdit::resizeEvent(e);

    //qDebug() << "--a2--rect:" << rect() << "  size:" << size();
}

void XTextWidget::setTextColor(const QColor & c)
{
    QString s(QStringLiteral("XTextWidget { background: transparent; color: %1; }"));
    setStyleSheet(s.arg(c.name()));
}

void XTextWidget::setAlignment(Qt::AlignmentFlag alignment)
{
    QTextEdit::setAlignment(alignment);
    adjustSize();
}

void XTextWidget::emitTextUpdated()
{
    //qDebug() << "XTextWidget::showEvent()" << Qt::endl << "--a1--rect:" << rect() << "  size:" << size();
    emit textUpdated(this->toPlainText());

    //qDebug() << "--a2--rect:" << rect() << "  size:" << size();
}
