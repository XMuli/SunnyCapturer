#include "xguidetips.h"
#include "ui_xguidetips.h"
#include <QLabel>
#include <QDebug>
#include "xguidebutton.h"

XGuideTips::XGuideTips(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XGuideTips)
{
    ui->setupUi(this);

    initUI();
}

XGuideTips::~XGuideTips()
{
    delete ui;
}

void XGuideTips::initUI()
{
    auto& layout = ui->gridLayout;
    int row = layout->rowCount();
    int colBtnTips = 0;
    int colLabel = 2;  // 0-控件； 1为20px固定弹簧； 2 为 label 文案

    auto createLabel = [](const QString& text) {
        QLabel *label = new QLabel(text);
        // label->setStyleSheet("color: rgba(255, 255, 255, 0.4);");
        label->setStyleSheet(
            "color: rgba(255, 255, 255, 1);"
            "background-color: rgba(255, 0, 0, 0.4);"
            );

        label->adjustSize();
        return label;
    };

    layout->addItem(new QSpacerItem(20, 1, QSizePolicy::Fixed, QSizePolicy::Fixed), 0, 1);  // 添加一个固定宽度为 20 px 的弹簧

    layout->addWidget(new XGuideButton(GuidTipsType::GTT_WSAD), row, colBtnTips, Qt::AlignRight);
    layout->addWidget(createLabel(tr("Move the cursor by 1 pixel")), row++, colLabel, Qt::AlignLeft);

    layout->addWidget(new XGuideButton(GuidTipsType::GTT_azimuth_arrow), row, colBtnTips, Qt::AlignRight);
    layout->addWidget(createLabel(tr("Move the snipping area by 1 pixel")), row++, colLabel, Qt::AlignLeft);

    layout->addWidget(new XGuideButton(GuidTipsType::GTT_tab), row, colBtnTips, Qt::AlignRight);
    layout->addWidget(createLabel(tr("Toggle between window detection and element detection")), row++, colLabel, Qt::AlignLeft);
    adjustSize();
}

void XGuideTips::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    const QColor wihte(255, 255, 255, 0.4 * 255);
    const QColor black(0, 0, 0, 0.8 * 255);        // 灰色透明

    QPainter pa(this);
    pa.setRenderHint(QPainter::Antialiasing);
    pa.setPen(QPen(wihte, 2));
    pa.setBrush(black);
    pa.drawRect(rect());
}
