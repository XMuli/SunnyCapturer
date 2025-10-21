// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XROUNDWIDGET_H
#define XROUNDWIDGET_H

#include <QBoxLayout>
#include <QWidget>

class XRoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XRoundWidget(QWidget *parent = nullptr);
    virtual ~XRoundWidget();

    void initUI();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // XROUNDWIDGET_H
