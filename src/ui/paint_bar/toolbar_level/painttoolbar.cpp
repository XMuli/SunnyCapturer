#include "painttoolbar.h"
#include <QDebug>
#include <QMouseEvent>
#include <QVariant>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include "paintbarhelper.h"
#include "communication.h"
#include "../../../data/configmanager.h"

PaintToolBar::PaintToolBar(const Qt::Orientation &orie, QWidget *parent)
    : QWidget(parent)
    , m_layout(nullptr)
    , m_orie(orie)
{
    initUI();
}

void PaintToolBar::initUI()
{
    if (m_orie == Qt::Horizontal) m_layout = new QHBoxLayout(this);
    else if (m_orie == Qt::Vertical) m_layout =  new QVBoxLayout(this);

    setContentsMargins(0, 0, 0, 0);
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    initBtns();
}

void PaintToolBar::initBtns()
{
    m_btns.reserve(13);
    m_btns.emplace_back(nullptr, PaintType::PT_rectangle, "rectangle", tr("Rectangle"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_ellipse, "ellipse", tr("Ellipse"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_arrow, "arrow", tr("Arrow"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_pencil, "pencil", tr("Pencil"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_marker_pen, "marker_pen", tr("Marker pen"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_mosaic, "mosaic", tr("Mosaic/Blur"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_text, "text", tr("Text"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_serial, "serial", tr("Serial"), true, false);
    m_btns.emplace_back(nullptr, PaintType::PT_pin, "pin", tr("Pin"), false, true);

    m_btns.emplace_back(nullptr, PaintType::PT_undo, "undo", tr("Undo (Ctrl + Z)"), false, false);
    m_btns.emplace_back(nullptr, PaintType::PT_redo, "redo", tr("Redo (Ctrl + Y)"), false, true);

    m_btns.emplace_back(nullptr, PaintType::PT_save, "save", tr("Save"), false, false);
    m_btns.emplace_back(nullptr, PaintType::PT_cancel, "cancel", tr("Cancel"), false, false);
    m_btns.emplace_back(nullptr, PaintType::PT_finish, "finish", tr("Finish"), false, false);

    for (int i = 0; i < m_btns.size(); ++i) {
        auto& it = m_btns.at(i);
        auto& tb = it.btn;
        const double& scal = dpiScale();
        const QSize size(ICON_SIZE * scal, ICON_SIZE * scal);
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
//        tb->show();  // 提前显示的话，添加到 ScreenShot 中，会显示批量 show 的时候被生成捕捉看到阴影，造成 bug

        m_layout->addWidget(tb, Qt::AlignCenter);
        if (it.bAddSpacer) addSpacerLine(m_layout, m_orie);
        connect(tb, &QToolButton::released, this, &PaintToolBar::onPaintBtnReleased);
    }

    // 结尾添加弹簧进行压缩
    if (m_orie == Qt::Horizontal)  {
        m_layout->addItem(new QSpacerItem(0, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
    } else if (m_orie == Qt::Vertical) {
        m_layout->addItem(new QSpacerItem(1, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }
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
            const QIcon newIcon(changedSVGColor(path, highlightColor(), btn->iconSize()));

            if (bSpik && tBtn != nullptr && tBtn == btn) {
                btn->setIcon(btn->isChecked() ? newIcon : origIcon);
            } else {
                btn->setChecked(false);
                btn->setIcon(QIcon(origIcon));
            }
        }
    }
}

// 一级工具栏，有某个绘画按钮被按下
bool PaintToolBar::hadDrawBtnsChecked() const
{
    for (int i = 0; i < m_btns.size(); ++i) {
        const auto& it = m_btns.at(i).btn;

        if (it->isCheckable() && it->isChecked())
            return true;
    }

    return false;
}

void PaintToolBar::onPaintBtnReleased()
{
    QToolButton* btn = qobject_cast<QToolButton*>(sender());
    if (!btn) return;
    const PaintType& type = btn->property(PROPERTY_PAINT_TYPR).value<PaintType>();
    emit sigPaintToolBtnsRelease(type, btn->isCheckable());

    if (btn->isCheckable()) paintBtnsExclusive(btn, true);
    adjustSize();
    qDebug() << "------------->onBtnReleased:" << btn << btn->objectName() << btn->isCheckable() << btn->isChecked();
}
