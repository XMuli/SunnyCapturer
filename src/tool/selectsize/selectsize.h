// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef SELECTSIZE_H
#define SELECTSIZE_H

#include <QLabel>

// 选中矩形区域的尺寸大小，显示在左上角、尺寸位置的显示
class SelectSize : public QLabel
{
    Q_OBJECT
public:
    explicit SelectSize(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    explicit SelectSize(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SelectSize() = default;

signals:
    void sigTextChanged(QString text);

public slots:
    void onTextChanged(QString text);

private:
    void initUI();

protected:
//    void showEvent(QShowEvent *e) override;
    void paintEvent(QPaintEvent* e) override;
};

#endif // SELECTSIZE_H
