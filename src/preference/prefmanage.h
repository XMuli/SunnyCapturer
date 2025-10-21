// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef PREFMANAGE_H
#define PREFMANAGE_H

#include <map>
#include <QWidget>

namespace Ui {
class PrefManage;
}

class PrefManage : public QWidget
{
    Q_OBJECT

public:
    explicit PrefManage(QWidget *parent = nullptr);
    ~PrefManage();

private:
    void initUIGeneral();
    void initUIInterface();
    void initUIOutput();
    void initUIPin();
    void initUIHotkeys();
    void initUIAbout();
    void initUI();

    inline bool cbState2Bool(int state);

private slots:
    // tabGeneral
    void on_cbLanuage_currentTextChanged(const QString &language);
    void on_btnFont_released();
    void on_cbAutoRun_stateChanged(int sta);
    void on_cbLogLevel_currentTextChanged(const QString &level);

    // tabInterface
    void onBorderColor(const QColor& color);
    void onCrosshairColor(const QColor& color);
    void on_cbBorderStyle_currentTextChanged(const QString &style);
    void on_spBorderWidth_valueChanged(int val);
    void on_spCrosshairWidth_valueChanged(int val);
    void on_cbAutoDetectWindows_stateChanged(int val);
    void on_cbCrosshair_stateChanged(int val);
    void on_cbAutoCopy2clipboard_stateChanged(int val);

    // tabOutput
    void on_cbQuickSave_clicked(bool checked);
    void on_cbAutoSave_clicked(bool checked);
    void on_sbImageQuailty_valueChanged(int val);
    void on_leFileName_textChanged(const QString &name);
    void on_leConfig_textChanged(const QString &path);
    void on_leQuickSave_textChanged(const QString &path);
    void on_leAutoSave_textChanged(const QString &path);
    QString onSelectPath();

    // tabPin
    void on_sbOpacity_valueChanged(int val);
    void on_spMaxSize_valueChanged(int val);

    // tabAbout

    // Reset
    void on_generalReset_released();
    void on_interfaceReset_released();
    void on_outputReset_released();
    void on_pinReset_released();
    void on_hotkeysReset_released();

private:
    Ui::PrefManage *ui;
    std::map<const QString, const QString> m_lanuages;
    std::map<const QString, const QString> m_borderStyle;
};

#endif // PREFMANAGE_H
