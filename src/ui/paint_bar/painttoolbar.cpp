#include "painttoolbar.h"
#include <QDebug>
#include <QVariant>
#include "paintbarhelper.h"

PaintToolBar::PaintToolBar(const Qt::Orientation &orie, QWidget *parent)
    : QWidget(parent)
    , m_layout(new QGridLayout(this))
    , m_orie(orie)
    , m_paintCtrlBar(new PaintCtrlBar(orie, this))
{
    initUI();
}

void PaintToolBar::initUI()
{
    setContentsMargins(0, 0, 0, 0);
    m_layout->setContentsMargins(0, 0, 0 ,0);
    setLayout(m_layout);

    initBtns();

    connect(this, &PaintToolBar::sigBtnRelease, m_paintCtrlBar, &PaintCtrlBar::onPaintBtnRelease);
}

void PaintToolBar::initBtns()
{
//    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags());  // | Qt::WindowStaysOnTopHint
    setWindowFlags(Qt::WindowMinMaxButtonsHint | windowFlags());  // | Qt::WindowStaysOnTopHint  【】
//    【Qt bug，】 水平布局下，属于 Qt 的bug，去掉 Qt::FramelessWindowHint 属性，就完美了; 且可以使用如下 来查看验证； 反之带上，则需要手动拖曳一下，才会发现界面被刷新是正常的。
//    qDebug() << "-----@3---->rowCount:" << m_layout->rowCount()  << "columnCount:" << m_layout->columnCount() << "" << m_layout->count();

    m_btns.reserve(13);
    m_btns.emplace_back(nullptr, PaintType::PT_rectangle, "rectangle", tr("Rectangle"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_ellipse, "ellipse", tr("Ellipse"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_arrow, "arrow", tr("Arrow"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_pencil, "pencil", tr("Pencil"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_mosaic, "mosaic", tr("Mosaic/Blur"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_marker_pen, "marker_pen", tr("Marker pen"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_text, "text", tr("Text"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_serial, "serial", tr("Serial"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_pin, "pin", tr("Pin"), false, true);

    m_btns.emplace_back(nullptr, PaintType::PT_undo, "undo", tr("Undo (Ctrl + ZA)"), false, false);
    m_btns.emplace_back(nullptr, PaintType::PT_redo, "redo", tr("Redo (Ctrl + Y)"), false, true);

    m_btns.emplace_back(nullptr, PaintType::PT_save, "save", tr("Save"), false, false);
    m_btns.emplace_back(nullptr, PaintType::PT_cancel, "cancel", tr("Cancel"), false, false);
    m_btns.emplace_back(nullptr, PaintType::PT_finish, "finish", tr("Finish"), false, false);

    for (int i = 0; i < m_btns.size(); ++i) {
        auto& it = m_btns.at(i);
        auto& tb = it.btn;
        const QSize size(ICON_SIZE, ICON_SIZE);

        tb = new QToolButton();
        tb->setObjectName(it.name);
        tb->setProperty(PROPERTY_PAINT_TYPR, QVariant::fromValue(it.type));
        tb->setChecked(false);
        tb->setAutoRaise(true);
        tb->setToolButtonStyle(Qt::ToolButtonIconOnly);
        tb->setStyleSheet("border-style:none");
        tb->setIcon(QIcon(":/resources/screenshot_ui/paint_tool_bar/paint_btn/" + it.name + ".svg"));
        tb->setContentsMargins(0, 0, 0, 0);
        tb->setIconSize(size);
        tb->setFixedSize(size);
        tb->setToolTip(it.tooltip);
        tb->setCheckable(it.bCheckable);
        tb->show();

        const int& count = countItemsformLayout(m_layout, m_orie);
        if (m_orie == Qt::Horizontal)  {
            m_layout->addWidget(tb, 0, count, Qt::AlignCenter);
        } else if (m_orie == Qt::Vertical)  {
            m_layout->addWidget(tb, count, 0, Qt::AlignCenter);
        }
        if (it.bAddSpacer) addSpacerLine(m_layout, m_orie);
        connect(tb, &QToolButton::released, this, &PaintToolBar::onPaintBtnReleased);
    }

    // 结尾添加弹簧进行压缩
    int count = countItemsformLayout(m_layout, m_orie);
    if (m_orie == Qt::Horizontal)  {
        m_layout->addItem(new QSpacerItem(0, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, count++);
        m_layout->addWidget(m_paintCtrlBar, 1, 0, 1, count);
    } else if (m_orie == Qt::Vertical) {
        m_layout->addItem(new QSpacerItem(1, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), count++, 0);
        m_layout->addWidget(m_paintCtrlBar, 0, 1, count, 1);
    }

    printfAllItems("初始化函数之前");

    m_paintCtrlBar->setParent(nullptr);
    m_paintCtrlBar->hide();
    setLayoutSpacing();
    printfAllItems("初始化函数之后");
}

// bSpik: true-btn需要跳过统一逻辑，单独执行自己的逻辑修改 icon, false-全部都设置为回复默认; ret: true-有按钮处理选中按下  false-全部都没有按下
// 通过自定义 paintBtns-Exclusive 规则，改变自生 icon 的颜色
void PaintToolBar::paintBtnsExclusive(const QToolButton* tBtn, const bool& bSpik)
{
    for (int i = 0; i < m_btns.size(); ++i) {
        auto& btn = m_btns.at(i).btn;

        // 仅传入进来的 tBtn 状态相反变化，而其余的都需要置为未选中状态
        if (btn->isCheckable()) {
            const QString path = ":/resources/screenshot_ui/paint_tool_bar/paint_btn/" + btn->objectName() + ".svg";
            const QIcon origIcon(path);
            const QIcon newIcon(changedSVGColor(path, QColor(Qt::green).name(), btn->iconSize()));

            if (bSpik && tBtn != nullptr && tBtn == btn) {
                btn->setIcon(btn->isChecked() ? newIcon : origIcon);
            } else {
                btn->setChecked(false);
                btn->setIcon(QIcon(origIcon));
            }
        }
    }
}

void PaintToolBar::setLayoutSpacing(int horSpace, int verSpace)
{
    bool bClearnSpace = m_layout->rowCount() == 1 || m_layout->columnCount() == 1;
    if (m_orie == Qt::Horizontal)  {
        if (bClearnSpace) verSpace = 0;
    } else if (m_orie == Qt::Vertical) {
        if (bClearnSpace) horSpace = 0;
    }

    m_layout->setHorizontalSpacing(horSpace);
    m_layout->setVerticalSpacing(verSpace);
}

// 添加 m_paintCtrlBar 到整体的布局里面
void PaintToolBar::setPaintCtrlBarToLayout()
{
    const int& count = countItemsformLayout(m_layout, m_orie);
    // 【 需要调整 bOnlyOneRowOrCol 不存在时候的 逻辑】
    if (hadPaintBtnChecked()) {
        if (m_orie == Qt::Horizontal)  {
            m_layout->addWidget(m_paintCtrlBar, 1, 0, 1, count);
        } else if (m_orie == Qt::Vertical) {
            m_layout->addWidget(m_paintCtrlBar, 0, 1, count, 1);
        }

        m_paintCtrlBar->show();
    } else {
        m_layout->removeWidget(m_paintCtrlBar);
        m_paintCtrlBar->setParent(nullptr);
        m_paintCtrlBar->hide();
    }
}

// 一级工具栏，有某个绘画按钮被按下
bool PaintToolBar::hadPaintBtnChecked()
{
    for (int i = 0; i < m_btns.size(); ++i) {
        const auto& it = m_btns.at(i).btn;

        if (it->isCheckable() && it->isChecked())
            return true;
    }

    return false;
}

void PaintToolBar::printfAllItems(const QString &prompted)
{
    QLayoutItem *item;

    // Iterate through the layout's items
    for (int row = 0; row < m_layout->rowCount(); ++row) {
        for (int col = 0; col < m_layout->columnCount(); ++col) {
            QLayoutItem *item = m_layout->itemAtPosition(row, col);
            qDebug() << "row:" << row << "col:" << col << "item:" << item;
            if (item) {
                QWidget *widget = item->widget();
                qDebug() << "widget:" << widget;

                if (widget) {
                    int rowSpan = m_layout->rowStretch(row);
                    int colSpan = m_layout->columnStretch(col);
                    qDebug() << "Row:" << row << "Column:" << col << "Row Span:" << rowSpan << "Column Span:" << colSpan;
                }
            }
        }
    }

    qDebug() << QString("---%1---m_layout 的数量是:%2").arg(prompted).arg(m_layout->count()) << Qt::endl << Qt::endl << Qt::endl;
}




void PaintToolBar::onPaintBtnReleased()
{
    QToolButton* btn = qobject_cast<QToolButton*>(sender());

    if (btn->isCheckable()) {
        paintBtnsExclusive(btn, true);
        const PaintType& type = btn->property(PROPERTY_PAINT_TYPR).value<PaintType>();
        emit sigBtnRelease(type);
        qDebug() << "------------->onBtnReleased:" << btn << btn->objectName() << btn->isCheckable() << btn->isChecked();

        printfAllItems("执行 setPaintCtrlBarToLayout 之前");
        setPaintCtrlBarToLayout();
        printfAllItems("执行 setPaintCtrlBarToLayout 后");
    } else { // Pin,Undo,Redo.. 的响应逻辑另写

    }


    qDebug() << "-----@3---->rowCount:" << m_layout->rowCount()  << "columnCount:" << m_layout->columnCount() << "" << m_layout->count();


    adjustSize();
}
