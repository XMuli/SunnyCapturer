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
    } else if (m_orie == Qt::Vertical) {
        m_layout->addItem(new QSpacerItem(1, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), count++, 0);
    }

    // 这两行顺序很重要
    setLayoutSpacing();
    setPaintCtrlBarToLayout(false);
}

// bSpik: true-btn需要跳过统一逻辑，单独执行自己的逻辑修改 icon, false-全部都设置为回复默认; ret: true-有按钮处理选中按下  false-全部都没有按下
bool PaintToolBar::paintBtnsExclusive(const QToolButton* tBtn, const bool& bSpik)
{
    bool ret = false;
    for (int i = 0; i < m_btns.size(); ++i) {
        auto& it = m_btns.at(i);
        auto& btn = it.btn;

        // 仅传入进来的 tBtn 状态相反变化，而其余的都需要置为未选中状态
        if (btn->isCheckable()) {
            const QString path = ":/resources/screenshot_ui/paint_tool_bar/paint_btn/" + btn->objectName() + ".svg";
            const QIcon origIcon(path);
            const QIcon newIcon(changedSVGColor(path, QColor(Qt::green).name(), btn->iconSize()));

            if (bSpik && tBtn && tBtn == btn) {
                btn->setIcon(btn->isChecked() ? newIcon : origIcon);
            } else {
                btn->setChecked(false);
                btn->setIcon(QIcon(origIcon));
            }

            if (btn->isChecked()) ret = true;

        } else {
            continue;
        }
    }

    return ret;
}

void PaintToolBar::toPaintBtn(const QToolButton *btn)
{

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
void PaintToolBar::setPaintCtrlBarToLayout(const bool &hadBtnChecked)
{
    int count = countItemsformLayout(m_layout, m_orie);
//    bool onlyHadRowOrCol = bOnlyOneRowOrCol();  // [需要调整逻辑， 只要被添加过，那么就是]   这里还是有问题 ！！！


    qDebug() << "m_layout->rowCount():" << m_layout->rowCount() << "m_layout->columnCount():" << m_layout->columnCount();
    // 【 需要调整 bOnlyOneRowOrCol 不存在时候的 逻辑】
    if (m_orie == Qt::Horizontal)  {
        if (onlyHadRowOrCol) {
            if (hadBtnChecked) {
                m_layout->addWidget(m_paintCtrlBar, 1, 0, 1, count);
                m_paintCtrlBar->show();
            } else {
                m_layout->removeWidget(m_paintCtrlBar);
                m_paintCtrlBar->setParent(nullptr);
                m_paintCtrlBar->hide();
            }
        }
    } else if (m_orie == Qt::Vertical) {
        if (onlyHadRowOrCol) {
            if (hadBtnChecked) {
                m_layout->addWidget(m_paintCtrlBar, 0, 1, count, 1);
                m_paintCtrlBar->show();
            } else {
                m_layout->removeWidget(m_paintCtrlBar);
                m_paintCtrlBar->setParent(nullptr);
                m_paintCtrlBar->hide();
            }
        }
    }
}

bool PaintToolBar::bOnlyOneRowOrCol()
{
    // 检查第二行和第二列是否有元素
    bool hasItemInSecondRow = false;
    bool hasItemInSecondColumn = false;

    if (m_orie == Qt::Horizontal && m_layout->rowCount() >= 2)  {
        for (int col = 0; col < m_layout->columnCount(); ++col) {
            QLayoutItem *item = m_layout->itemAtPosition(1, col);
            if (item) {
                hasItemInSecondRow = true;
                break;
            }
        }

        return hasItemInSecondRow;
    } else if (m_orie == Qt::Vertical && m_layout->columnCount() >= 2) {
        for (int row = 0; row < m_layout->rowCount(); ++row) {
            QLayoutItem *item = m_layout->itemAtPosition(row, 1);
            if (item) {
                hasItemInSecondColumn = true;
                break;
            }
        }

        return hasItemInSecondColumn;
    } else {
        return true;
    }
}




void PaintToolBar::onPaintBtnReleased()
{
    QToolButton* btn = qobject_cast<QToolButton*>(sender());
    static bool prevHadBtnChecked = false;
    const bool& hadBtnChecked = paintBtnsExclusive(btn, true);

    const PaintType& type = btn->property(PROPERTY_PAINT_TYPR).value<PaintType>();
    emit sigBtnRelease(type);

////    虽然理论是更加标准的做法，但是好像没有必要，属于额外的？？

    if (prevHadBtnChecked != hadBtnChecked) {
        setPaintCtrlBarToLayout(hadBtnChecked);
    }

    qDebug() << "------------->onBtnReleased:" << btn << btn->objectName() << btn->isCheckable() << btn->isChecked()
             << "prevHadBtnChecked:" << prevHadBtnChecked << "hadBtnChecked:" << hadBtnChecked;
    prevHadBtnChecked = hadBtnChecked;
    adjustSize();

}
