// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef BLURWIDGET_H
#define BLURWIDGET_H

#include <memory>
#include <QWidget>
#include <QGraphicsBlurEffect>

// 模拟 亚克力/透明磨砂 效果（此处取巧）
class QPixmap;
class BlurWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BlurWidget(QWidget *parent = nullptr);
    virtual ~BlurWidget() = default;
    void setPixmap(const QPixmap& pix, int blurRadius);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_pix;
//    std::unique_ptr<QGraphicsBlurEffect> m_pBlurEffect;
};

#endif // BLURWIDGET_H
