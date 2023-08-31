#include "verspacerline.h"

VerSpacerLine::VerSpacerLine(int length, QWidget *parent, Qt::WindowFlags f)
{
    setFrameShape(VLine);
    setFrameShadow(Sunken);
    setLineWidth(0);
    setMidLineWidth(2);
    setFixedHeight(length);
}
