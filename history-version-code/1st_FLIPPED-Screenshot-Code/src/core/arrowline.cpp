// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "arrowline.h"

ArrowLine::ArrowLine(double lineWidth, const QPointF &p1, const QPointF &p2)
    : m_lineWidth(lineWidth)
    , m_p1(p1)
    , m_p2(p2)
{
}

const QPainterPath ArrowLine::arrowLine() const
{
    QPointF p1;
    QPointF p2;
    calcVertexes(p1, p2);

    QPolygonF polygon;
    polygon << m_p2 << p1 << p2;

    QPainterPath path;
    path.addPolygon(polygon);
    path.closeSubpath();

    const QLineF l1(p1, p2);
    const QLineF l2(l1.center(), m_p2);
//    const double vertLine( + pow(l2.center().x() - m_p2.x(), 2)));
//    const QPointF tEnd(m_p2 - qAbs<double>(l2.center().y() - m_p2.y()) / 3);  //三角形垂线的 1/3 处
    path.moveTo(m_p1);
    path.lineTo(l2.center());
    return path;
}

const QPainterPath ArrowLine::arrowLineOpen() const
{
    QPointF p1;
    QPointF p2;
    calcVertexes(p1, p2);

    QPainterPath path;
    path.moveTo(m_p1);
    path.lineTo(m_p2);
    path.lineTo(p1);
    path.moveTo(m_p2);
    path.lineTo(p2);
    return path;
}
const QPainterPath ArrowLine::circleLine() const
{
    QPainterPath path;
    path.moveTo(m_p1);
    path.lineTo(m_p2);
    const double mix = 2;
    const double max = 100;
    const double r = qBound<double>(mix, m_lineWidth * 1.2, max);
    path.addEllipse(m_p2, r, r);
    return path;
}

const QPainterPath ArrowLine::line() const
{
    QPainterPath path;
    path.moveTo(m_p1);
    path.lineTo(m_p2);
    return path;
}

void ArrowLine::calcVertexes(QPointF &p1, QPointF &p2) const
{
    double lenght = 12;       // 箭头边线长度
    double degrees = 0.4;     // 箭头夹角
//    const double baseLine = sqrt(pow(m_p2.y() - m_p1.y(), 2) + pow(m_p2.x() - m_p1.x(), 2));
    lenght = qBound<double>(30, lenght + m_lineWidth * 2, 200);

    double angle = atan2(m_p2.y() - m_p1.y(), m_p2.x() - m_p1.x()) + M_PI;

    p1.setX(m_p2.x() + lenght * cos(angle - degrees));
    p1.setY(m_p2.y() + lenght * sin(angle - degrees));
    p2.setX(m_p2.x() + lenght * cos(angle + degrees));
    p2.setY(m_p2.y() + lenght * sin(angle + degrees));
}
