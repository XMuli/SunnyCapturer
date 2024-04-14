// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "sidesettingui.h"
#include "pin.h"
#include "ui_sidesettingui.h"
#include "general.h"
#include "interface.h"
#include "output.h"
#include "pin.h"
#include "hotkeys.h"
#include "about.h"
#include "tokens.h"
#include "communication.h"


SideSettingUI::SideSettingUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SideSettingUI)
    , m_sideGroup(new QButtonGroup(this))
    , m_general(new AbsSettingUI(this))
    , m_interface(new AbsSettingUI(this))
    , m_output(new AbsSettingUI(this))
    , m_pin(new AbsSettingUI(this))
    , m_hotkeys(new AbsSettingUI(this))
    , m_tokens(new AbsSettingUI(this))
    , m_about(new About(this))
{
    ui->setupUi(this);

    initUI();
}

SideSettingUI::~SideSettingUI()
{
    delete ui;
}

void SideSettingUI::initUI()
{
    m_sideGroup->setExclusive(true);
    for (auto& it : ui->sideWidget->findChildren<QToolButton*>()) {
        static int i = 0;
        if (!it) continue;
        it->setCheckable(true);
        it->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        it->setAutoRaise(true);
        m_sideGroup->addButton(it, i++);
    }


    const auto& general = new General();
    const auto& interface = new Interface();
    const auto& output = new Output();
    const auto& pin = new Pin();
    const auto& hotkeys = new Hotkeys();
    const auto& tokens = new Tokens();
//    const auto& about = new About();

    m_general->insertWidget(0, general);
    m_interface->insertWidget(0, interface);
    m_output->insertWidget(0, output);
    m_pin->insertWidget(0, pin);
    m_hotkeys->insertWidget(0, hotkeys);
    m_tokens->insertWidget(0, tokens);
//    m_about->insertWidget(0, about);

    ui->stackedWidget->insertWidget(0, m_general);
    ui->stackedWidget->insertWidget(1, m_interface);
    ui->stackedWidget->insertWidget(2, m_output);
    ui->stackedWidget->insertWidget(3, m_pin);
    ui->stackedWidget->insertWidget(4, m_hotkeys);
    ui->stackedWidget->insertWidget(5, m_tokens);
    ui->stackedWidget->insertWidget(6, m_about);

    // 设置默认选中的页面
    ui->stackedWidget->setCurrentIndex(0);
    connect(m_sideGroup, QOverload<int>::of(&QButtonGroup::idPressed), this, [this](int n) {
        ui->stackedWidget->setCurrentIndex(n);
        auto btns = ui->sideWidget->findChildren<QToolButton*>();
        if (n <= btns.count() && btns.at(n)) btns.at(n)->setChecked(true);
    });
//    connect(m_sideGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &SideSettingUI::onSideGroupChanged);

    connect(m_general, &AbsSettingUI::sigBtnHintClicked, this, &SideSettingUI::onBtnHintClicked);
    connect(m_interface, &AbsSettingUI::sigBtnHintClicked, this, &SideSettingUI::onBtnHintClicked);
    connect(m_output, &AbsSettingUI::sigBtnHintClicked, this, &SideSettingUI::onBtnHintClicked);
    connect(m_pin, &AbsSettingUI::sigBtnHintClicked, this, &SideSettingUI::onBtnHintClicked);
    connect(m_hotkeys, &AbsSettingUI::sigBtnHintClicked, this, &SideSettingUI::onBtnHintClicked);
    connect(m_tokens, &AbsSettingUI::sigBtnHintClicked, this, &SideSettingUI::onBtnHintClicked);

    connect(m_general, &AbsSettingUI::sigBtnResetClicked, general, &General::onBtnResetClicked);
    connect(m_interface, &AbsSettingUI::sigBtnResetClicked, interface, &Interface::onBtnResetClicked);
    connect(m_output, &AbsSettingUI::sigBtnResetClicked, output, &Output::onBtnResetClicked);
    connect(m_pin, &AbsSettingUI::sigBtnResetClicked, pin, &Pin::onBtnResetClicked);
    connect(m_hotkeys, &AbsSettingUI::sigBtnResetClicked, hotkeys, &Hotkeys::onBtnResetClicked);
    connect(m_tokens, &AbsSettingUI::sigBtnResetClicked, tokens, &Tokens::onBtnResetClicked);
    connect(&COMM, &Communication::sigLanguageChange, this, [this]() { ui->retranslateUi(this);});

}

void SideSettingUI::onSideGroupChanged(int idx)
{

//    auto& stackedWidget = ui->stackedWidget;

//    qDebug() << "----1-->stackedWidget->count():" << stackedWidget->count();

//    const int& count = stackedWidget->count();
//    for (int i = 0; i < count; ++i) {
//        QWidget *widget = stackedWidget->widget(i);
//        stackedWidget->removeWidget(widget); // 从QStackedWidget中移除
////        widget->deleteLater(); // 销毁QWidget
//        widget->hide();
//    }

//    qDebug() << "----1.1-->stackedWidget->count():" << stackedWidget->count();

//    QPointer<AbsSettingUI> newWidget = new AbsSettingUI(this);
//    if (idx == 0){
//        newWidget->insertWidget(0, new General());
//    } else if (idx == 1) {
//        newWidget->insertWidget(0, new Interface());
//    }

//    stackedWidget->insertWidget(idx, newWidget);

    //    qDebug() << "----2-->stackedWidget->count():" << stackedWidget->count();
}

void SideSettingUI::onBtnHintClicked(bool checked)
{
    const AbsSettingUI* absUI = qobject_cast<AbsSettingUI *>(sender());
    if (absUI == m_general) {
    } else if (absUI == m_interface) {
    } else if (absUI == m_output) {
    } else if (absUI == m_pin) {
    } else if (absUI == m_hotkeys) {
    } else if (absUI == m_tokens) {
    } else {
    }
}

void SideSettingUI::closeEvent(QCloseEvent *e)
{
    CJ.onSyncToFile();
    QWidget::closeEvent(e);
}

//void SideSettingUI::onBtnResetClicked(bool checked)
//{
//    const AbsSettingUI* absUI = qobject_cast<AbsSettingUI *>(sender());
//    if (absUI == m_general) {
//    } else if (absUI == m_interface) {
//    } else if (absUI == m_output) {
//    } else if (absUI == m_pin) {
//    } else if (absUI == m_hotkeys) {
//    } else if (absUI == m_tokens) {
//    } else {
//    }
//}

