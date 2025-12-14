// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef HOTKEYSWIDGET_H
#define HOTKEYSWIDGET_H

#include <map>
#include <QWidget>
#include <QString>

// Description: 偏好设置界面的 hotkeys UI；为了规避掉 编辑框 鼠标点击外面却仍有焦点
class HotkeysWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HotkeysWidget(QWidget *parent = nullptr);
    virtual ~HotkeysWidget() = default;

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // HOTKEYSWIDGET_H
