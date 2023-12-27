#include "hotkeys.h"
#include "ui_hotkeys.h"
#include <QKeySequence>
#include "communication.h"
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

    XKeySequenceEdit* keyEdit = qobject_cast<XKeySequenceEdit *>(sender());
    if (keySequence.isEmpty() || !keyEdit) return;

    HotKeyType type;
    QLabel *lab = nullptr;
    QString  propertyName;
    if (keyEdit == ui->kseCapture) {
        type = HotKeyType::HKT_capture;
        propertyName = "XHotkeys_capture";
        lab = ui->labCaptureStatus;
    } else if (keyEdit == ui->kseDelayCapture) {
        type = HotKeyType::HKT_delay_capture;
        propertyName = "XHotkeys_delay_capture";
        lab = ui->labDelayCaptureStatus;
    } else if (keyEdit == ui->kseCustomCapture) {
        type = HotKeyType::HKT_custiom_capture;
        propertyName = "XHotkeys_custom_capture";
        lab = ui->labCustomCaptureStatus;
    } else {
        qDebug() << "keyEdit does not any know XKeySequenceEdit object!";
    }

    const bool& resetOK = COMM.resetShortcut(keySequence, type);
    if (resetOK) CONF_MANAGE.setProperty(propertyName.toStdString().data(), keySequence.toString());
    keyEdit->setStyleSheet(QString("background-color: %1;").arg(resetOK ? "" : "#FFCDD2"));
    setHotkeyIconStatus(lab, type);
}

void Hotkeys::onBtnResetClicked(bool checked)
{
    ui->kseCapture->setKeySequence(QKeySequence("F6"));
    ui->kseDelayCapture->setKeySequence(QKeySequence("Ctrl+F6"));
    ui->kseCustomCapture->setKeySequence(QKeySequence("Shift+F6"));

    CONF_MANAGE.setProperty("XHotkeys_capture", ui->kseCapture->keySequence().toString());
    CONF_MANAGE.setProperty("XHotkeys_delay_capture", ui->kseDelayCapture->keySequence().toString());
    CONF_MANAGE.setProperty("XHotkeys_custom_capture", ui->kseCustomCapture->keySequence().toString());
}

void Hotkeys::initUI()
{
    ui->kseCapture->setKeySequence(QKeySequence(CONF_MANAGE.property("XHotkeys_capture").toString()));
    ui->kseDelayCapture->setKeySequence(QKeySequence(CONF_MANAGE.property("XHotkeys_delay_capture").toString()));
    ui->kseCustomCapture->setKeySequence(QKeySequence(CONF_MANAGE.property("XHotkeys_custom_capture").toString()));
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
