#include "xguidetips.h"
#include "ui_xguidetips.h"
#include <QDebug>
#include "communication.h"

XGuideTips::XGuideTips(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XGuideTips)
    , m_wsad(new XGuideButton(GuidTipsType::GTT_WSAD, this))
    , m_azimuthArrow(new XGuideButton(GuidTipsType::GTT_azimuth_arrow, this))
    , m_tab(new XGuideButton(GuidTipsType::GTT_tab, this))
    , m_quoteleft(new XGuideButton(GuidTipsType::GTT_quoteleft, this))
    , m_ctrlShift(new XGuideButton(GuidTipsType::GTT_ctrl_shift, this))
    , m_ctrlE(new XGuideButton(GuidTipsType::GTT_ctrl_e, this))
    , m_mouseWheel(new XGuideButton(GuidTipsType::GTT_mouse_wheel, this))
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
            // "background-color: rgba(255, 0, 0, 0.4);"
            );
        label->adjustSize();
        return label;
    };

    m_wsadLab           = createLabel("");
    m_azimuthArrowLab   = createLabel("");
    m_tabLab            = createLabel("");
    m_quoteleftLab      = createLabel("");
    m_ctrlShiftLab      = createLabel("");
    m_ctrlELab           = createLabel("");
    m_mouseWheelLab     = createLabel("");
    m_debugLab          = createLabel(actionTypeToString(m_type));

    onLanguageChange("");

    auto& layout = ui->gridLayout;
    int row = layout->rowCount();
    int colBtnTips = 0;
    int colLabel = 2;  // 0-控件； 1为10px * 缩放比的固定弹簧； 2 为 label 文案

    layout->addItem(new QSpacerItem(10 * cursorScrnScale(false), 1, QSizePolicy::Fixed, QSizePolicy::Fixed), 0, 1);  // 添加一个固定宽度为 20 px 的弹簧

    layout->addWidget(m_wsad, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_wsadLab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_azimuthArrow, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_azimuthArrowLab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_ctrlShift, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_ctrlShiftLab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_tab, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_tabLab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_quoteleft, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_quoteleftLab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_mouseWheel, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_mouseWheelLab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_ctrlE, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_ctrlELab, row++, colLabel, Qt::AlignLeft);

    layout->addWidget(m_debug, row, colBtnTips, Qt::AlignRight);
    layout->addWidget(m_debugLab, row++, colLabel, Qt::AlignLeft);


    connect(&COMM, &Communication::sigLanguageChange, this, [this]() {
        ui->retranslateUi(this);
        onLanguageChange("");
    });

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

void XGuideTips::onLanguageChange(const QString &qm)
{
    QString wsad_wait      = "";
    QString wsad_detection = "";
    if (m_type == ActionType::AT_picking_detection_windows_rect) {
        wsad_wait      = tr("Move the cursor by 1 pixel");
        wsad_detection = wsad_wait;
    } else { // ActionType::AT_wait and else
        wsad_wait      = tr("Move the snipping area by 1 pixel");
        wsad_detection = wsad_wait;
    }

    const QString& szTab          = tr("Toggle between window detection and element detection");
    const QString& szQuoteleft    = tr("Display detailed information about this window's process");
    const QString& szCtrlShiftLab = tr("Enlargement / Reduction of the snipping area by 1 pixel");
    const QString& szCtrlA        = tr("Select the active screen / fullscreen");
    const QString& szMouseWheel   = tr("Adjust the pen width");

    m_wsadLab->setText(wsad_wait);
    m_azimuthArrowLab->setText(wsad_detection);
    m_tabLab->setText(szTab);
    m_quoteleftLab->setText(szQuoteleft);
    m_ctrlShiftLab->setText(szCtrlShiftLab);
    m_ctrlELab->setText(szCtrlA);
    m_mouseWheelLab->setText(szMouseWheel);
    m_debugLab->setText(actionTypeToString(m_type));

    adjustSize();
}

int XGuideTips::textHeight() const
{
    return m_textHeight;
}

void XGuideTips::setTextHeight(int newTextHeight)
{
    m_textHeight = newTextHeight;

    m_wsad->setTextHeight(m_textHeight);
    m_azimuthArrow->setTextHeight(m_textHeight);
    m_tab->setTextHeight(m_textHeight);
    m_quoteleft->setTextHeight(m_textHeight);
    m_ctrlShift->setTextHeight(m_textHeight);
    m_ctrlE->setTextHeight(m_textHeight);
    m_mouseWheel->setTextHeight(m_textHeight);
    m_debug->setTextHeight(m_textHeight);

    m_wsadLab->setFixedHeight(m_textHeight);
    m_azimuthArrowLab->setFixedHeight(m_textHeight);
    m_tabLab->setFixedHeight(m_textHeight);
    m_quoteleftLab->setFixedHeight(m_textHeight);
    m_ctrlShiftLab->setFixedHeight(m_textHeight);
    m_ctrlELab->setFixedHeight(m_textHeight);
    m_mouseWheelLab->setFixedHeight(m_textHeight);
    m_debugLab->setFixedHeight(m_textHeight);
}

ActionType XGuideTips::actionType() const
{
    return m_type;
}

void XGuideTips::setActionType(const ActionType &newActionType)
{
    m_type = newActionType;
    onLanguageChange(COMM.toLocaleName(CJ_GET_QSTR("general.language")));
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
        m_ctrlShift->hide();
        m_ctrlShiftLab->hide();
        // m_ctrlE->hide();
        // m_ctrlELab->hide();
    } else if (m_type == ActionType::AT_wait) {

    }

    adjustSize();
    hideIfAllChildrenHidden();
}
