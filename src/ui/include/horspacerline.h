#ifndef HORSPACERLINE_H
#define HORSPACERLINE_H

#include <QFrame>

class HorSpacerLine : public QFrame
{
    Q_OBJECT
public:
    HorSpacerLine(int length, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif // HORSPACERLINE_H
