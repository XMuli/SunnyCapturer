#ifndef VERSPACERLINE_H
#define VERSPACERLINE_H

#include <QFrame>

class VerSpacerLine : public QFrame
{
    Q_OBJECT
public:
    VerSpacerLine(int length, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

};

#endif // VERSPACERLINE_H
