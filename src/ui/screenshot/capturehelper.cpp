#include "capturehelper.h"

CaptureHelper::CaptureHelper(QObject *parent)
    : QObject{parent}
{

}

QRect CaptureHelper::rectFromTowPos(const QPoint &p1, const QPoint &p2)
{
    int xMin = qMin<int>(p1.x(), p2.x());
    int xMax = qMax<int>(p1.x(), p2.x());
    int yMin = qMin<int>(p1.y(), p2.y());
    int yMax = qMax<int>(p1.y(), p2.y());

    // 若是重复点，则会返回 QRect(0, 0, 1, 1);
    if (xMin == xMax && yMin == yMax)
        return QRect(xMin, xMax, 0, 0);
    else
        return QRect(QPoint(xMin, yMin), QPoint(xMax, yMax));
}
