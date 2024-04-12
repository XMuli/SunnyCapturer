// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "hotkeys.h"
#include "ui_hotkeys.h"
#include <QKeySequence>
#include "communication.h"

Hotkeys::Hotkeys(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hotkeys)
{
    ui->setupUi(this);
    initUI();
}

Hotkeys::~Hotkeys()
{
    delete ui;
}

void Hotkeys::onKeySeqChanged(const QKeySequence &keySequence)
{
    XKeySequenceEdit* keyEdit = qobject_cast<XKeySequenceEdit *>(sender());
    if (keySequence.isEmpty() || !keyEdit) return;

    HotKeyType type;
    QLabel *lab = nullptr;
    QString  name;
    if (keyEdit == ui->kseCapture) {
        type = HotKeyType::HKT_capture;
        name = "hotkeys.capture";
        lab = ui->labCaptureStatus;
    } else if (keyEdit == ui->kseDelayCapture) {
        type = HotKeyType::HKT_delay_capture;
        name = "hotkeys.delay_capture";
        lab = ui->labDelayCaptureStatus;
    } else if (keyEdit == ui->kseCustomCapture) {
        type = HotKeyType::HKT_custiom_capture;
        name = "hotkeys.custom_capture";
        lab = ui->labCustomCaptureStatus;
    } else {
        qDebug() << "keyEdit does not any know XKeySequenceEdit object!";
    }

    const bool& resetOK = COMM.resetShortcut(keySequence, type);
    if (resetOK) CJ.setKeyValue(name, keySequence.toString().toStdString());
    keyEdit->setStyleSheet(QString("background-color: %1;").arg(resetOK ? "" : "#FFCDD2"));
    setHotkeyIconStatus(lab, type);
}

void Hotkeys::onBtnResetClicked(bool checked)
{
    const ordered_json& j = CJ.defaultConfigJson();
    CJ.setJ("hotkeys", j["hotkeys"]);

    ui->kseCapture->setKeySequence(QKeySequence(CJ_GET_QSTR("hotkeys.capture")));
    ui->kseDelayCapture->setKeySequence(QKeySequence(CJ_GET_QSTR("hotkeys.delay_capture")));
    ui->kseCustomCapture->setKeySequence(QKeySequence(CJ_GET_QSTR("hotkeys.custom_capture")));
}

void Hotkeys::initUI()
{
    ui->kseCapture->setKeySequence(QKeySequence(CJ_GET_QSTR("hotkeys.capture")));
    ui->kseDelayCapture->setKeySequence(QKeySequence(CJ_GET_QSTR("hotkeys.delay_capture")));
    ui->kseCustomCapture->setKeySequence(QKeySequence(CJ_GET_QSTR("hotkeys.custom_capture")));
    setHotkeyIconStatus(ui->labCaptureStatus, HotKeyType::HKT_capture);
    setHotkeyIconStatus(ui->labDelayCaptureStatus, HotKeyType::HKT_delay_capture);
    setHotkeyIconStatus(ui->labCustomCaptureStatus, HotKeyType::HKT_custiom_capture);

    connect(ui->kseCapture, &XKeySequenceEdit::sigKeySeqChanged, this, &Hotkeys::onKeySeqChanged);
    connect(ui->kseDelayCapture, &XKeySequenceEdit::sigKeySeqChanged, this, &Hotkeys::onKeySeqChanged);
    connect(ui->kseCustomCapture, &XKeySequenceEdit::sigKeySeqChanged, this, &Hotkeys::onKeySeqChanged);
    connect(&COMM, &Communication::sigLanguageChange, this, [this]() { ui->retranslateUi(this);});
}

void Hotkeys::setHotkeyIconStatus(QLabel *lab, const HotKeyType &type)
{
    if (!lab) return;
    const QString& t = COMM.shortcutStatus(type) ? "success" : "error";
    const QString& path = QString(":/resources/icons/setting/%1.svg").arg(t);
    lab->setPixmap(QPixmap(path));
}
