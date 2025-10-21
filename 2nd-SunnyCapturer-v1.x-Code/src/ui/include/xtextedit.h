// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XTEXTEDIT_H
#define XTEXTEDIT_H

#include <QFocusEvent>
#include <QTextEdit>

// struct TextData
// {
//     TextData() {}

//     QColor          m_colorBackground;     // 前景色
//     QColor          m_colorForeground;     // 背景色
//     QTextCharFormat m_format;              // 当前选中的状态
// };

class XTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit XTextEdit(QWidget* parent = nullptr);

    void adjustSize();
    void setFont(const QFont& f);
    void applyAllCharFormat(const QTextCharFormat& format);

    bool focusAble() const;
    void setFocusAble(bool focusAble);

protected:
    void showEvent(QShowEvent* e) override;
    void resizeEvent(QResizeEvent* e) override;
    void focusInEvent(QFocusEvent *e) override;
    void focusOutEvent(QFocusEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

signals:
    void textUpdated(const QString& s);

private:
    QSize           m_baseSize;
    QSize           m_minSize;
    bool            m_focusAble;    // true-支持获取焦点， false-不接受焦点获取
    QTextCharFormat m_lastFormat;   // fix: 删除了所有时刻，再次写保留之前的格式
};

#endif // XTEXTEDIT_H
