// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XMONITORLABEL_H
#define XMONITORLABEL_H

#include <QWidget>
#include <QScreen>
#include <QGuiApplication>
#include <QList>
#include <QPaintEvent>
#include <QStringList>

// 图形化显示器的组合形状
class XMonitorLabel : public QWidget
{
    Q_OBJECT
public:
    explicit XMonitorLabel(QWidget *parent = 0);
    ~XMonitorLabel() = default;

    QStringList scrnsInfo() const;
    const QSize &fixSize();

    void extracted(QRect &maxRt, QPixmap &pixmap);
    QPixmap renderMonitorToPixmap();

protected:
    void paintEvent(QPaintEvent *e) override;


private:
    double scaling(const QScreen *screen = qGuiApp->primaryScreen()) const;
    QList<QScreen *> m_scrns;
    QScreen* m_priScrn;
};

#endif // XMONITORLABEL_H
