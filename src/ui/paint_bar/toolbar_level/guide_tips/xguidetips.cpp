#include "xguidetips.h"
#include "ui_xguidetips.h"
#include <QDebug>

XGuideTips::XGuideTips(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XGuideTips)
    , m_wsad(new XGuideButton(GuidTipsType::GTT_WSAD, this))
    , m_azimuthArrow(new XGuideButton(GuidTipsType::GTT_azimuth_arrow, this))
    , m_tab(new XGuideButton(GuidTipsType::GTT_tab, this))
    , m_quoteleft(new XGuideButton(GuidTipsType::GTT_quoteleft, this))
    , m_shift(new XGuideButton(GuidTipsType::GTT_shift, this))
    , m_ctrl(new XGuideButton(GuidTipsType::GTT_ctrl, this))
    , m_debug(new XGuideButton(GuidTipsType::GTT_quoteleft, this))
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
    auto createLabel = [](const QString& text) {
        QLabel *label = new QLabel(text);
        label->setStyleSheet(
            "color: rgba(255, 255, 255, 1);"
            "background-color: rgba(255, 0, 0, 0.4);"
            );
        label->adjustSize();
        return label;
    };

    const QString& wsad_wait      = tr("Move the cursor by 1 pixel");
    const QString& wsad_detection = tr("Move the snipping area by 1 pixel");
    const QString& szTab          = tr("Toggle between window detection and element detection");
    const QString& szQuoteleft    = tr("Display detailed information about this window's process");
    const QString& szShift        = tr("Stretch reduction of the snipping area by 1 pixel");
    const QString& szCtrl         = tr("Stretch enlargement of the snipping area by 1 pixel");

    m_wsadLab           = createLabel(wsad_wait);
    m_azimuthArrowLab   = createLabel(wsad_wait);
    m_tabLab            = createLabel(szTab);
    m_quoteleftLab      = createLabel(szQuoteleft);
    m_shiftLab          = createLabel(szShift);
    m_ctrlLab           = createLabel(szCtrl);
    m_debugLab          = createLabel(actionTypeToString(m_type));

    auto& layout = ui->gridLayout;
    int row = layout->rowCount();
    int colBtnTips = 0;
    int colLabel = 2;  // 0-控件； 1为20px固定弹簧； 2 为 label 文案

    layout->addItem(new QSpacerItem(20, 1, QSizePolicy::Fixed, QSizePolicy::Fixed), 0, 1);  // 添加一个固定宽度为 20 px 的弹簧

    layout->addWidget(m_wsad, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_wsadLab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_azimuthArrow, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_azimuthArrowLab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_tab, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_tabLab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_quoteleft, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_quoteleftLab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_shift, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_shiftLab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_ctrl, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_ctrlLab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_debug, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_debugLab, row++, colLabel, Qt::AlignLeft);
    adjustSize(); // 调整窗口大小以适应新布局

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

ActionType XGuideTips::actionType() const
{
    return m_type;
}

void XGuideTips::setActionType(const ActionType &newActionType)
{
    m_type = newActionType;
    autoShowGuideTips();
}

bool XGuideTips::hideIfAllChildrenHidden()
{
    auto& layout = ui->gridLayout;

    // 遍历布局中的所有子控件
    bool allHidden = true;
    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem* item = layout->itemAt(i);
        if (item && item->widget() && item->widget()->isVisible()) {
            allHidden = false;
            break;
        }
    }

    // 如果所有子控件都被隐藏，则隐藏自身
    if (allHidden)
        this->hide();

    return allHidden;
}

void XGuideTips::autoShowGuideTips()
{
    m_debugLab->setText(actionTypeToString(m_type));
    auto& layout = ui->gridLayout;

    // 将所有子控件设置父对象为空，并隐藏
    for (int i = 0; i < layout->count(); ++i) {
        QWidget* widget = layout->itemAt(i)->widget();
        if (widget) {
            // layout->removeWidget(widget);
            // widget->setParent(nullptr);
            widget->show();
        }
    }

    if (m_type == ActionType::AT_picking_detection_windows_rect) {
        m_shift->hide();
        m_ctrl->hide();
        m_shiftLab->hide();
        m_ctrlLab->hide();
    } else if (m_type == ActionType::AT_wait) {

    }

    adjustSize();
    hideIfAllChildrenHidden();
}
