// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>
#ifndef MANAGEBAR_H
#define MANAGEBAR_H

#include <QWidget>

class QBoxLayout;
class ManageBar : public QWidget
{
    Q_OBJECT
public:
    explicit ManageBar(Qt::Orientations orien = Qt::Horizontal, QWidget* parent = nullptr);
    virtual ~ManageBar() = default;
    void addWidget(QWidget* w);
    void addSpacer();

private:
    double m_scal;
    Qt::Orientations m_orien;
    QBoxLayout* m_layout;
};

#endif // MANAGEBAR_H
