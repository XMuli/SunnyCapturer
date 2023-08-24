#include "sidesettingui.h"
#include "ui_sidesettingui.h"
#include <QDebug>
#include "general.h"
#include "interface.h"
#include "output.h"

SideSettingUI::SideSettingUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SideSettingUI)
    , m_sideGroup(new QButtonGroup(this))
    , m_general(new AbsSettingUI(this))
    , m_interface(new AbsSettingUI(this))
    , m_output(new AbsSettingUI(this))
{
    ui->setupUi(this);
    m_general->insertWidget(0, new General());
    m_interface->insertWidget(0, new Interface());
    m_output->insertWidget(0, new Output());
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

    ui->stackedWidget->addWidget(m_general);
    ui->stackedWidget->addWidget(m_interface);
    ui->stackedWidget->addWidget(m_output);

    // 设置默认选中的页面
    ui->stackedWidget->setCurrentIndex(0);

    connect(m_sideGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), ui->stackedWidget, &QStackedWidget::setCurrentIndex);
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

