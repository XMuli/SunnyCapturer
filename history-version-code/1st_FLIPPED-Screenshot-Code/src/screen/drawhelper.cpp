// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "drawhelper.h"
#include <QScreen>
#include <QFile>
#include <QByteArray>
#include <QPainter>
#include <QPixmap>
#include <QIcon>
#include <QSvgRenderer>
#include <QDomElement>
#include <QDomElement>
#include <QIcon>
#include <QGraphicsEffect>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainterPath>
#include <QLine>
#include <QDir>
#include <QSettings>
#include <QDateTime>
#include <QStandardPaths>
#include "../xglobal.h"
#include "../preference/appellation.h"

QString XDrawStep::serialText = "0_0_0_0";

void XDrawStep::showDebug() const
{
    const auto& t = pen.widthF();
    qDebug() << "showDebug(): this:" << this;
    qDebug() << "p1:" << p1 << "   p2:" << p2 << "   rt:" << rt << "   shape:" << int(shape) << "   shapePara:" << int(shapePara);
    qDebug() << &pen << "  " << pen << "   " << pen.color().name() << "   " << pen.widthF() << "  t:" << &t << "  " << t;
    qDebug() << &brush << "  " << brush << "   " << brush.color().name();
    qDebug() << &text << "  " << text << "   font:" << font << "   textParas:" << textParas << "   serialText:" << serialText << Qt::endl;
}

void XDrawStep::partClear()
{
    p1 = QPoint();
    p2 = QPoint();
    rt = QRect();
    custPath.clear();
}

void XDrawStep::destroyClear()
{
    p1 = QPoint();
    p2 = QPoint();
    rt = QRect();
    shape = DrawShape::NoDraw;
    shapePara = ShapePara::SP_0;
    custPath.clear();
    text.clear();

    font.setBold(false);
    font.setItalic(false);
    textParas = TextPara(0);
}

