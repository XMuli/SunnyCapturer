#include "painttoolbar.h"
#include <QDebug>
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
        tb->setProperty(PROPERTY_PAINT_TYPR, static_cast<int>(it.type));
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
        connect(tb, &QToolButton::released, this, &PaintToolBar::onBtnReleased);
    }

    QSpacerItem *spacer = nullptr;
    int horSpace = 0;
    int verSpace = 0;
    int count = countItemsformLayout(m_layout, m_orie);

    if (m_orie == Qt::Horizontal)  {
        verSpace = 10;
        spacer = new QSpacerItem(0, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
        m_layout->addItem(spacer, 0, count++);
        m_layout->addWidget(m_paintCtrlBar, 1, 0, 1, count);
    } else if (m_orie == Qt::Vertical) {
        horSpace = 10;
        spacer = new QSpacerItem(1, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        m_layout->addItem(spacer, count++, 0);
        m_layout->addWidget(m_paintCtrlBar, 0, 1, count, 1);
    }
    m_layout->setHorizontalSpacing(horSpace);
    m_layout->setVerticalSpacing(verSpace);
}

// bSpik: true-btn需要跳过的, false-全部都设置为回复默认
void PaintToolBar::paintBtnsExclusive(const QToolButton* tBtn, const bool& bSpik)
{
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

        }
    }
}

void PaintToolBar::onBtnReleased()
{
    QToolButton* btn = qobject_cast<QToolButton*>(sender());
    paintBtnsExclusive(btn, true);

    qDebug() << "------------->onBtnReleased:" << btn << btn->objectName() << btn->isCheckable() << btn->isChecked();
}
