// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XFRAMEWIDGET_H
#define XFRAMEWIDGET_H

#include <QWidget>

class XFrameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XFrameWidget(QWidget *parent = nullptr);
    virtual ~XFrameWidget() = default;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // XFRAMEWIDGET_H
