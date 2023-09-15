#include "sidesettingui.h"
#include "pin.h"
#include "ui_sidesettingui.h"
#include "general.h"
#include "interface.h"
#include "output.h"
#include "pin.h"
#include "hotkeys.h"
#include "about.h"

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
    , m_about(new AbsSettingUI(this))
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

    m_general->insertWidget(0, new General());
    m_interface->insertWidget(0, new Interface());
    m_output->insertWidget(0, new Output());
    m_pin->insertWidget(0, new Pin());
    m_hotkeys->insertWidget(0, new Hotkeys());
    m_tokens->insertWidget(0, new QWidget());
    m_about->insertWidget(0, new About());

    ui->stackedWidget->insertWidget(0, m_general);
    ui->stackedWidget->insertWidget(1, m_interface);
    ui->stackedWidget->insertWidget(2, m_output);
    ui->stackedWidget->insertWidget(3, m_pin);
    ui->stackedWidget->insertWidget(4, m_hotkeys);
    ui->stackedWidget->insertWidget(5, m_tokens);
    ui->stackedWidget->insertWidget(6, m_about);

    // 设置默认选中的页面
    ui->stackedWidget->setCurrentIndex(0);

    connect(m_sideGroup, QOverload<int>::of(&QButtonGroup::idPressed), ui->stackedWidget, &QStackedWidget::setCurrentIndex);
//    connect(m_sideGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &SideSettingUI::onSideGroupChanged);
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

