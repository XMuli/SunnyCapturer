// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "prefmanage.h"
#include "qmetaobject.h"
#include "ui_prefmanage.h"

#include <map>
//#include <QUrl>
#include <QIcon>
#include <QString>

#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QtGlobal>
#include <QFontDialog>
#include <QFont>
#include <QApplication>
#include <QTranslator>
#include <QFileDialog>
//#include <QDesktopServices>
#include "../screen/tray.h"
#include "../screen/datamaid.h"
#include "../tool/base/colorparabar.h"
#include "../widget/xkeysequenceedit.h"

PrefManage::PrefManage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrefManage)
{
    ui->setupUi(this);
    initUI();

//    connect(ui->tabWidget,  &QTabWidget::currentChanged, [this](int index){
//        this->show();
//        this->update();

////        ui->tabGeneral->adjustSize();
////        ui->tabInterface->adjustSize();
////        ui->tabOutput->adjustSize();
////        ui->tabHotkeys->adjustSize();
////        ui->tabPin->adjustSize();
////        ui->tabAbout->adjustSize();
//        this->adjustSize();
//    });
}

PrefManage::~PrefManage()
{
    delete ui;
}

void PrefManage::initUIGeneral()
{
    m_lanuages = { {"en_US", "English"}
                  ,{"zh_CN", "简体中文"}
                  ,{"zh_TW", "繁體中文"}};

    const auto it = m_lanuages.find(DATAMAID->paraValue("lanuage").value<QString>());
    const QString strLanuage = (it != m_lanuages.end()) ? it->second : m_lanuages.at("en_US");

    for (const auto& it : m_lanuages)
        ui->cbLanuage->addItem(it.second, it.first);

    ui->cbLanuage->setCurrentText(strLanuage);
    ui->btnFont->setText(DATAMAID->paraValue("font").value<QString>());
    ui->cbAutoRun->setChecked(DATAMAID->paraValue("autoRun").value<bool>());
    ui->cbLogLevel->setCurrentText(DATAMAID->paraValue("logLevel").value<QString>());
}

void PrefManage::initUIInterface()
{
    QString style = DATAMAID->paraValue("borderStyle").value<QString>();  // Fix: 提前，cbBorderStyle->addItem 会导致其值改变

    m_borderStyle = {  {tr("Flipped"), "Flipped"}
                      ,{tr("BalckAndWhite"), "BalckAndWhite"}
                      ,{tr("LightBlue"), "LightBlue"}};
    for (const auto& it : m_borderStyle)
        ui->cbBorderStyle->addItem(it.first, it.second);

    auto cpbHighLight = new ColorParaBar(ColorParaBarMode::CPB_HighLight);
    auto cpbCrosshair = new ColorParaBar(ColorParaBarMode::CPB_HighLight);
    cpbHighLight->setObjectName(tiBorderColor);
    cpbCrosshair->setObjectName(tiCrosshairColor);
    ui->interfaceLayout->addWidget(cpbHighLight, 1, 1);
    ui->interfaceLayout->addWidget(cpbCrosshair, 4, 1);

    
    ui->cbBorderStyle->setCurrentText(style);
    cpbHighLight->setCurColor(DATAMAID->paraValue("borderColor").value<QString>());
    ui->spBorderWidth->setValue(DATAMAID->paraValue("borderWidth").value<int>());
    cpbCrosshair->setCurColor(DATAMAID->paraValue("crosshairColor").value<QString>());
    ui->spCrosshairWidth->setValue(DATAMAID->paraValue("crosshairWidth").value<int>());
    ui->cbAutoDetectWindows->setChecked(DATAMAID->paraValue("smartWindow").value<bool>());
    ui->cbCrosshair->setChecked(DATAMAID->paraValue("crosshair").value<bool>());
    ui->cbAutoCopy2clipboard->setChecked(DATAMAID->paraValue(tiAutoCopy2Clipboard).value<bool>());

    connect(cpbHighLight, &ColorParaBar::sigColorChange, this, &PrefManage::onBorderColor);
    connect(cpbCrosshair, &ColorParaBar::sigColorChange, this, &PrefManage::onCrosshairColor);
}

void PrefManage::initUIOutput()
{
    ui->sbImageQuailty->setValue(DATAMAID->paraValue(toImageQuailty).value<int>());
    ui->leFileName->setText(DATAMAID->paraValue(toFileName).toString());
    ui->leConfig->setText(DATAMAID->paraValue(toConfigPath).toString());
    ui->leQuickSave->setText(DATAMAID->paraValue(toQuickSavePath).toString());
    ui->leAutoSave->setText(DATAMAID->paraValue(toAutoSavePath).toString());

    const bool quickSaveEnable = DATAMAID->paraValue(toQuickSave).toBool();
    ui->cbQuickSave->setChecked(quickSaveEnable);
    ui->btnQuickSave->setEnabled(quickSaveEnable);
    ui->leQuickSave->setEnabled(quickSaveEnable);
    const bool autoSaveEnable = DATAMAID->paraValue(toAutoSave).toBool();
    ui->cbAutoSave->setChecked(autoSaveEnable);
    ui->btnAutoSave->setEnabled(autoSaveEnable);
    ui->leAutoSave->setEnabled(autoSaveEnable);

    const QString tooltip = DATAMAID->formatToFileName(ui->leFileName->text());
    ui->leFileName->setToolTip(tooltip);

//    QString tip = "Shift + F4";
//    cbQuickSave->setToolTip(tip);

    connect(ui->btnConfig, &QPushButton::released, this, &PrefManage::onSelectPath);
    connect(ui->btnQuickSave, &QPushButton::released, this, &PrefManage::onSelectPath);
    connect(ui->btnAutoSave, &QPushButton::released, this, &PrefManage::onSelectPath);
}

void PrefManage::initUIPin()
{
    ui->sbOpacity->setValue(DATAMAID->paraValue(tpOpacity).value<int>());
    ui->sbMaxSize->setValue(DATAMAID->paraValue(tpMaxSize).value<int>());
}


void PrefManage::initUIHotkeys()
{
    int row = 0;
    int col = 2;

    for (auto& it : Tray::instance().getVHotKeys()) {
        auto& hk = std::get<0>(it);
        QString& hotkey = std::get<1>(it);
        CaptureHelper::CaptureType& sst = std::get<3>(it);

        XKeySequenceEdit* pEdit = new XKeySequenceEdit(QKeySequence(hotkey));
        ui->hotkeyLayout->addWidget(pEdit, row++, col);

        QMetaEnum enumSst = QMetaEnum::fromType<CaptureHelper::CaptureType>();
        pEdit->setObjectName(enumSst.valueToKey(sst));
        pEdit->setMinimumWidth(110 * DATAMAID->scale());
        const bool reg = hk->isRegistered();
        if (reg) {
            pEdit->setStyleSheet("background-color: #98fb98;");
        } else {
            pEdit->setStyleSheet("background-color: #ff7f50;");
        }

        qDebug() << "====>" << pEdit << "  " << pEdit->objectName() << pEdit->parent() << " " << pEdit->parentWidget() << Qt::endl
            << ui->hotkeyLayout;
        connect(pEdit, &XKeySequenceEdit::sigKeySeqChanged, &(Tray::instance()), &Tray::onKeySequenceChanged);
    }
}

void PrefManage::initUIAbout()
{
    QString bit;
    if (_BIT_ARCH == 4) bit = "x86";
    else if (_BIT_ARCH == 8) bit = "x64";
    ui->labVersion->setText(QString("%1-Beta %2 %3 (%4)")
                            .arg(_PROJECT_VERSION)
                            .arg(_COMPILER_ID)
                            .arg(bit)
                            .arg(MY_BUILD_TIME));

}

void PrefManage::initUI()
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    initUIGeneral();
    initUIInterface();
    initUIOutput();
    initUIPin();
    initUIHotkeys();
    initUIAbout();
}

bool PrefManage::cbState2Bool(int state)
{
    return state == Qt::Checked ? true : false;
}

void PrefManage::onBorderColor(const QColor &color)
{
    DATAMAID->setParaValue(tiBorderColor, color.name());
}

void PrefManage::onCrosshairColor(const QColor &color)
{
    DATAMAID->setParaValue(tiCrosshairColor, color.name());
}

void PrefManage::on_cbBorderStyle_currentTextChanged(const QString &style)
{
    Q_UNUSED(style);
    auto btn = ui->cbBorderStyle;
    const QString dataStyle = btn->itemData(btn->currentIndex()).toString();
    DATAMAID->setParaValue(tiBorderStyle, dataStyle);
}


void PrefManage::on_spBorderWidth_valueChanged(int val)
{
    DATAMAID->setParaValue(tiBorderWidth, val);
}


void PrefManage::on_spCrosshairWidth_valueChanged(int val)
{
    DATAMAID->setParaValue(tiCrosshairWidth, val);
}


void PrefManage::on_cbAutoDetectWindows_stateChanged(int val)
{
    DATAMAID->setParaValue(tiSmartWindow, cbState2Bool(val));
}


void PrefManage::on_cbCrosshair_stateChanged(int val)
{
    DATAMAID->setParaValue(tiCrosshair, cbState2Bool(val));
}


void PrefManage::on_cbAutoCopy2clipboard_stateChanged(int val)
{
    DATAMAID->setParaValue(tiAutoCopy2Clipboard, cbState2Bool(val));
}


QString PrefManage::onSelectPath()
{
    QString selPath = "";
    auto btn = qobject_cast<QPushButton*>(sender());
    if (!btn) selPath;

    QString defPath = "";
    const QString title(tr("Select a path"));
    if (btn == ui->btnQuickSave) {
        defPath = ui->leQuickSave->text();
        selPath = QFileDialog::getExistingDirectory(this, title, defPath, QFileDialog::ShowDirsOnly);

        if (!selPath.isEmpty() && defPath != selPath)
            ui->leQuickSave->setText(selPath);
    } else if (btn == ui->btnAutoSave) {
        defPath = ui->leAutoSave->text();
        selPath = QFileDialog::getExistingDirectory(this, title, defPath, QFileDialog::ShowDirsOnly);

        if (!selPath.isEmpty() && defPath != selPath)
            ui->leAutoSave->setText(selPath);
    } else if (btn == ui->btnConfig) {
        defPath = ui->leConfig->text();
        selPath = QFileDialog::getExistingDirectory(this, title, defPath, QFileDialog::ShowDirsOnly);

        if (!selPath.isEmpty() && defPath != selPath)
            ui->leConfig->setText(selPath);
    }

    return selPath;
}

void PrefManage::on_sbOpacity_valueChanged(int val)
{
    DATAMAID->setParaValue(tpOpacity, val);
}

void PrefManage::on_spMaxSize_valueChanged(int val)
{
    DATAMAID->setParaValue(tpMaxSize, val);
}

void PrefManage::on_generalReset_released()
{
    const DataIni origIni;
    ui->cbLanuage->setCurrentText(origIni.defLanuage());
    ui->btnFont->setText(origIni.defFont());
    ui->cbAutoRun->setChecked(origIni.defAutoRun());
    ui->cbLogLevel->setCurrentText(origIni.defLogLevel());
}


void PrefManage::on_interfaceReset_released()
{
    const DataIni origIni;
    ui->cbBorderStyle->setCurrentText(origIni.defBorderStyle());
    ui->spBorderWidth->setValue(origIni.defBorderWidth());
    ui->spCrosshairWidth->setValue(origIni.defCrosshairWidth());
    ui->cbAutoDetectWindows->setChecked(origIni.defSmartWindow());
    ui->cbCrosshair->setChecked(origIni.defCrosshair());
    ui->cbAutoCopy2clipboard->setChecked(origIni.defAutoCopy2Clipboard());
}


void PrefManage::on_outputReset_released()
{
    const DataIni origIni;
    ui->sbImageQuailty->setValue(origIni.defImageQuailty());

    ui->leFileName->setText(origIni.defFileName());
    ui->leConfig->setText(origIni.defConfigPath());

    auto t = origIni.defQuickSavePath();
    ui->leQuickSave->setText(origIni.defQuickSavePath());
    ui->leAutoSave->setText(origIni.defAutoSavePath());

    ui->cbQuickSave->setChecked(origIni.defQuickSave());
    ui->cbAutoSave->setChecked(origIni.defAutoSave());
}


void PrefManage::on_pinReset_released()
{
    const DataIni origIni;
    ui->sbOpacity->setValue(origIni.defOpacity());
    ui->sbMaxSize->setValue(origIni.defMaxSize());
}


void PrefManage::on_hotkeysReset_released()
{
    const DataIni origIni;
    QMetaEnum enumSst = QMetaEnum::fromType<CaptureHelper::CaptureType>();
    QString hkCaptureName = enumSst.valueToKey(CaptureHelper::SST_ScrnCapture);
    QString hkDelayCaptureName = enumSst.valueToKey(CaptureHelper::SST_DelayCapture);
    QString hkFullScreCaptureName = enumSst.valueToKey(CaptureHelper::SST_FullScrnCapture);

    auto hkCapture = ui->tabHotkeys->findChild<XKeySequenceEdit*>(hkCaptureName);
    if (hkCapture) hkCapture->setKeySequence(origIni.defScrnCapture());  // 很奇怪，没有触发槽函数，所以下一行代码修改
    DATAMAID->setParaValue(thScrnCapture, origIni.defScrnCapture());  
    auto hkDelayCapture = ui->tabHotkeys->findChild<XKeySequenceEdit*>(hkDelayCaptureName);
    if (hkDelayCapture) hkDelayCapture->setKeySequence(origIni.defDelayCapture());
    DATAMAID->setParaValue(thDelayCapture, origIni.defDelayCapture());
    auto hkFullScreCapture = ui->tabHotkeys->findChild<XKeySequenceEdit*>(hkFullScreCaptureName);
    if (hkFullScreCapture) hkFullScreCapture->setKeySequence(origIni.defFullScrnCapture());
    DATAMAID->setParaValue(thFullScreen, origIni.defFullScrnCapture());
}


void PrefManage::on_sbImageQuailty_valueChanged(int val)
{
    DATAMAID->setParaValue(toImageQuailty, val);
}


void PrefManage::on_leFileName_textChanged(const QString &name)
{
    DATAMAID->setParaValue(toFileName, name);
}


void PrefManage::on_leConfig_textChanged(const QString &path)
{
    DATAMAID->setParaValue(toConfigPath, path);
}


void PrefManage::on_leQuickSave_textChanged(const QString &path)
{
    DATAMAID->setParaValue(toQuickSavePath, path);
}


void PrefManage::on_leAutoSave_textChanged(const QString &path)
{
    DATAMAID->setParaValue(toAutoSavePath, path);
}

void PrefManage::on_cbQuickSave_clicked(bool checked)
{
    DATAMAID->setParaValue(toQuickSave, checked);

    ui->btnQuickSave->setEnabled(checked);
    ui->leQuickSave->setEnabled(checked);
}


void PrefManage::on_cbAutoSave_clicked(bool checked)
{
    DATAMAID->setParaValue(toAutoSave, checked);

    qDebug() << "enbale:" << checked;

    ui->btnAutoSave->setEnabled(checked);
    ui->leAutoSave->setEnabled(checked);
}

void PrefManage::on_cbLanuage_currentTextChanged(const QString &language)
{
    qDebug() << qApp->applicationDirPath() + "/config/config.ini" << " language:" << language;
    auto btn = ui->cbLanuage;
    const QString strLanuage = btn->itemData(btn->currentIndex()).toString();
    DATAMAID->setParaValue("lanuage", strLanuage);

    auto t = DATAMAID->paraValue("font").value<QString>();
    

    QTranslator trans;
    QString qmPath(qApp->applicationDirPath());
#if defined(Q_OS_MAC)
    qmPath += "/../../../../bin/translations/" + strLanuage + ".qm";
#else
    qmPath += "/translations/" + strLanuage + ".qm";
#endif
    trans.load(qmPath);
    qApp->installTranslator(&trans);

    ui->retranslateUi(this);
    ui->btnFont->setText(DATAMAID->paraValue("font").value<QString>()); // fix: 切换后文本空白； 没有 tr 放于 retranslateUi 后
    
    QString bit;
    if (_BIT_ARCH == 4) bit = "x86";
    else if (_BIT_ARCH == 8) bit = "x64";
    ui->labVersion->setText(QString("%1-Beta %2 (%3)")
        .arg(_PROJECT_VERSION)
        .arg(bit)
        .arg(MY_BUILD_TIME));
}


void PrefManage::on_btnFont_released()
{
    bool ret = false;
    const auto fontFamily = DATAMAID->paraValue("font").toString();
    QFont font = QFontDialog::getFont(&ret, QFont(fontFamily.split(',').at(0)), this, tr("Select Font"));
    if (!ret) return;

    qApp->setFont(font);
    QString text = QString("%1, %2").arg(font.family()).arg(font.pointSize());
    ui->btnFont->setText(text);

    DATAMAID->setParaValue(tgFont, text);
    qDebug("当前选择的字体是[%s]-是否加粗[%d]-是否倾斜[%d]-字号[%d]", font.family().toUtf8().data(), font.bold(), font.italic(), font.pointSize());
}


void PrefManage::on_cbAutoRun_stateChanged(int sta)
{
    DATAMAID->setParaValue(tgAutoRun, cbState2Bool(sta));
    DATAMAID->setAutoRun();
}


void PrefManage::on_cbLogLevel_currentTextChanged(const QString &level)
{
    DATAMAID->setParaValue(tgLogLevel, level);
}




