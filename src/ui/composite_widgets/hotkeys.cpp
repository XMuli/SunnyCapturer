#include "hotkeys.h"
#include "ui_hotkeys.h"

#include <QKeySequence>
#include "../../data/configmanager.h"

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
    const XKeySequenceEdit* keyEdit = qobject_cast<XKeySequenceEdit *>(sender());
    if (keySequence.isEmpty() || !keyEdit) return;

    if (keyEdit == ui->kseCapture) {
        CONF_MANAGE.setProperty("XHotkeys_capture", keySequence.toString());
    } else if (keyEdit == ui->kseDelayCapture) {
        CONF_MANAGE.setProperty("XHotkeys_delay_capture", keySequence.toString());
    } else if (keyEdit == ui->kseCustomCapture) {
        CONF_MANAGE.setProperty("XHotkeys_custiom_capture", keySequence.toString());
    } else {
        qDebug() << "keyEdit does not any know XKeySequenceEdit object!";
    }

}

void Hotkeys::onBtnResetClicked(bool checked)
{
    ui->kseCapture->setKeySequence(QKeySequence("F6"));
    ui->kseDelayCapture->setKeySequence(QKeySequence("Ctrl+F6"));
    ui->kseCustomCapture->setKeySequence(QKeySequence("Shift+F6"));

    CONF_MANAGE.setProperty("XHotkeys_capture", ui->kseCapture->keySequence().toString());
    CONF_MANAGE.setProperty("XHotkeys_delay_capture", ui->kseDelayCapture->keySequence().toString());
    CONF_MANAGE.setProperty("XHotkeys_custiom_capture", ui->kseCustomCapture->keySequence().toString());
}

void Hotkeys::initUI()
{
    ui->kseCapture->setKeySequence(QKeySequence(CONF_MANAGE.property("XHotkeys_capture").toString()));
    ui->kseDelayCapture->setKeySequence(QKeySequence(CONF_MANAGE.property("XHotkeys_delay_capture").toString()));
    ui->kseCustomCapture->setKeySequence(QKeySequence(CONF_MANAGE.property("XHotkeys_custiom_capture").toString()));

    connect(ui->kseCapture, &XKeySequenceEdit::sigKeySeqChanged, this, &Hotkeys::onKeySeqChanged);
    connect(ui->kseDelayCapture, &XKeySequenceEdit::sigKeySeqChanged, this, &Hotkeys::onKeySeqChanged);
    connect(ui->kseCustomCapture, &XKeySequenceEdit::sigKeySeqChanged, this, &Hotkeys::onKeySeqChanged);
}
