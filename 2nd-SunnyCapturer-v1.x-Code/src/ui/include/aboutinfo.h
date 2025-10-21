// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef ABOUTINFO_H
#define ABOUTINFO_H

#include <QKeyEvent>
#include <QWidget>
#include <QFont>

namespace Ui {
class AboutInfo;
}

class AboutInfo : public QWidget
{
    Q_OBJECT

public:
    explicit AboutInfo(QWidget *parent = nullptr);
    ~AboutInfo();

private slots:
    void on_pushButton_released();
    void onLanguageChange(const QString &language);

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void init();
    const QString getSystemInfo();
    const QString detailedInfo() const;
    void insertLayout(const QStringList &lists);

private:
    Ui::AboutInfo *ui;
    QFont m_labFont;
};

#endif // ABOUTINFO_H
