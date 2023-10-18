#include "painttoolbar.h"
#include <QDebug>
#include <QMouseEvent>
#include <QVariant>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QShortcut>
#include <QKeySequence>
#include "paintbarhelper.h"
#include "communication.h"
#include "../../../data/configmanager.h"

PaintToolBar::PaintToolBar(const Qt::Orientation &orie, QWidget *parent)
    : QWidget(parent)
    , m_orie(orie)
    , m_layout(nullptr)
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
    m_btns.emplace_back(nullptr, PaintType::PT_pin, "pin", tr("Pin to screen") + " (Ctrl + T)", false, true);

    m_btns.emplace_back(nullptr, PaintType::PT_undo, "undo", tr("Undo") + " (Ctrl + Z)", false, false);
    m_btns.emplace_back(nullptr, PaintType::PT_redo, "redo", tr("Redo") + " (Ctrl + Y)", false, true);

    m_btns.emplace_back(nullptr, PaintType::PT_cancel, "cancel", tr("Cancel Capture") + " (Esc)", false, false);
    m_btns.emplace_back(nullptr, PaintType::PT_save, "save", tr("Save to file") + " (Ctrl + S)", false, false);
    m_btns.emplace_back(nullptr, PaintType::PT_finish, "copy", tr("copy to clipboard") + " (Ctrl + C)", false, false);

    #define CREATOR_QSHORTCUT(_type, _keySequence) \
        if (it.type == _type) { new QShortcut(QKeySequence(_keySequence), this, [&it]() { \
    it.btn->released(); \
    }); }

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
        tb->setStyleSheet(szIconBtnCSS);
//        tb->setStyleSheet(szIconBtnCSS +
//                                         "QToolButton:hover {"
//                                         "    /* 在悬浮状态下的样式 */"
//                                         "    border: 1px solid #00FF00; /* 例如，设置一个绿色边框 */"
//                                         "}");
        tb->setIcon(QIcon(":/resources/icons/paint_tool_bar/paint_btn/" + it.name + ".svg"));
        tb->setContentsMargins(0, 0, 0, 0);
        tb->setIconSize(size);
        tb->setFixedSize(size);
        tb->setToolTip(it.tooltip);
        tb->setCheckable(it.bCheckable);
//        tb->show();  // 提前显示的话，添加到 ScreenShot 中，会显示批量 show 的时候被生成捕捉看到阴影，造成 bug

        m_layout->addWidget(tb, Qt::AlignCenter);
        if (it.bAddSpacer) addSpacerLine(m_layout, m_orie);
        if (it.type == PaintType::PT_undo || it.type == PaintType::PT_redo) tb->setDisabled(true);


        // 若是要实现，则需要调优
//        if (tb->isCheckable()) {
//            const auto type = it.type;
//            if (type == PaintType::PT_rectangle) {
//                if (CONF_GET_PROPERTY(XPaintBarStatus_rect).toBool()) tb->setChecked(true);
//            } else if (type == PaintType::PT_ellipse) {
//                if (CONF_GET_PROPERTY(XPaintBarStatus_ellipse).toBool())
//                    tb->setChecked(true);
//            } else if (type == PaintType::PT_arrow) {
//                if (CONF_GET_PROPERTY(XPaintBarStatus_arrow).toBool()) tb->setChecked(true);
//            } else if (type == PaintType::PT_pencil) {
//                if (CONF_GET_PROPERTY(XPaintBarStatus_penciler).toBool()) tb->setChecked(true);
//            } else if (type == PaintType::PT_marker_pen) {
//                if (CONF_GET_PROPERTY(XPaintBarStatus_marker_pen).toBool()) tb->setChecked(true);
//            } else if (type == PaintType::PT_mosaic) {
//                if (CONF_GET_PROPERTY(XPaintBarStatus_mosaic).toBool()) tb->setChecked(true);
//            } else if (type == PaintType::PT_text) {
//                if (CONF_GET_PROPERTY(XPaintBarStatus_text).toBool()) tb->setChecked(true);
//            } else if (type == PaintType::PT_serial) {
//                if (CONF_GET_PROPERTY(XPaintBarStatus_serial).toBool()) tb->setChecked(true);
//            } else {
//            }
//        }

        connect(tb, &QToolButton::released, this, &PaintToolBar::onPaintBtnReleased);


#if 0
        CREATOR_QSHORTCUT(PaintType::PT_rectangle, Qt::CTRL + Qt::Key_1)
        CREATOR_QSHORTCUT(PaintType::PT_ellipse, Qt::CTRL + Qt::Key_2)
        CREATOR_QSHORTCUT(PaintType::PT_arrow, Qt::CTRL + Qt::Key_3)
        CREATOR_QSHORTCUT(PaintType::PT_pencil, Qt::CTRL + Qt::Key_4)
        CREATOR_QSHORTCUT(PaintType::PT_marker_pen, Qt::CTRL + Qt::Key_5)
        CREATOR_QSHORTCUT(PaintType::PT_mosaic, Qt::CTRL + Qt::Key_6)
        CREATOR_QSHORTCUT(PaintType::PT_text, Qt::CTRL + Qt::Key_7)
        CREATOR_QSHORTCUT(PaintType::PT_serial, Qt::CTRL + Qt::Key_8)
#endif
        CREATOR_QSHORTCUT(PaintType::PT_pin, Qt::CTRL + Qt::Key_T)
        CREATOR_QSHORTCUT(PaintType::PT_undo, Qt::CTRL + Qt::Key_Z)
        CREATOR_QSHORTCUT(PaintType::PT_redo, Qt::CTRL + Qt::Key_Y)
        CREATOR_QSHORTCUT(PaintType::PT_save, Qt::CTRL + Qt::Key_S)
        CREATOR_QSHORTCUT(PaintType::PT_finish, Qt::CTRL + Qt::Key_C)
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
            const QString path = ":/resources/icons/paint_tool_bar/paint_btn/" + btn->objectName() + ".svg";
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

PaintBtn *PaintToolBar::findPaintBtn(const PaintType &type)
{
    PaintBtn *btn = nullptr;

    for (PaintBtn& it : m_btns) {
        if (it.type == type)
            btn = &it;
    }

    return btn;
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

#include <QTimer>
#include <QTime>
void PaintToolBar::onPaintBtnReleased()
{
    QToolButton* btn = qobject_cast<QToolButton*>(sender());
    if (!btn) return;
    const PaintType& type = btn->property(PROPERTY_PAINT_TYPR).value<PaintType>();
    emit sigPaintToolBtnsRelease(type, btn->isCheckable());

    if (btn->isCheckable()) {
        paintBtnsExclusive(btn, true);

          // 这一段比较耗时，不推荐
//        QTimer::singleShot(100, this, [&btn, &type](){
//            QTime timer;
//            timer.start();  // 启动计时器

//            const bool& tChecked = false;
//            CONF_SET_PROPERTY(XPaintBarStatus_rect, tChecked);
//            CONF_SET_PROPERTY(XPaintBarStatus_ellipse, tChecked);
//            CONF_SET_PROPERTY(XPaintBarStatus_arrow, tChecked);
//            CONF_SET_PROPERTY(XPaintBarStatus_penciler, tChecked);
//            CONF_SET_PROPERTY(XPaintBarStatus_marker_pen, tChecked);
//            CONF_SET_PROPERTY(XPaintBarStatus_mosaic, tChecked);
//            CONF_SET_PROPERTY(XPaintBarStatus_text, tChecked);
//            CONF_SET_PROPERTY(XPaintBarStatus_serial, tChecked);


//            int elapsedFirst = timer.elapsed();  // 获取经过的毫秒数
//            qDebug() << "Elapsed time for the first block: " << elapsedFirst << "ms";

//            timer.restart();  // 重新启动计时器

//            const bool& isChecked = btn->isChecked();
//            if (type == PaintType::PT_rectangle) {
//                CONF_SET_PROPERTY(XPaintBarStatus_rect, isChecked);
//            } else if (type == PaintType::PT_ellipse) {
//                CONF_SET_PROPERTY(XPaintBarStatus_ellipse, isChecked);

//                auto t = CONF_GET_PROPERTY(XPaintBarStatus_ellipse).toBool();
//                qDebug() << t;
//            } else if (type == PaintType::PT_arrow) {
//                CONF_SET_PROPERTY(XPaintBarStatus_arrow, isChecked);
//            } else if (type == PaintType::PT_pencil) {
//                CONF_SET_PROPERTY(XPaintBarStatus_penciler, isChecked);
//            } else if (type == PaintType::PT_marker_pen) {
//                CONF_SET_PROPERTY(XPaintBarStatus_marker_pen, isChecked);
//            } else if (type == PaintType::PT_mosaic) {
//                CONF_SET_PROPERTY(XPaintBarStatus_mosaic, isChecked);
//            } else if (type == PaintType::PT_text) {
//                CONF_SET_PROPERTY(XPaintBarStatus_text, isChecked);
//            } else if (type == PaintType::PT_serial) {
//                CONF_SET_PROPERTY(XPaintBarStatus_serial, isChecked);
//            } else {
//            }


//            int elapsedSecond = timer.elapsed();  // 获取经过的毫秒数
//            qDebug() << "Elapsed time for the second block: " << elapsedSecond << "ms";
//        });

    }

    adjustSize();
    qDebug() << "------------->onBtnReleased:" << btn << btn->objectName() << btn->isCheckable() << btn->isChecked();
}

void PaintToolBar::onAutoDisableUndoAndRedo(const bool &undoDisable, const bool &redoDisable)
{
    PaintBtn* btn = findPaintBtn(PaintType::PT_undo);
    if (btn && btn->btn) btn->btn->setDisabled(undoDisable);

    btn = findPaintBtn(PaintType::PT_redo);
    if (btn && btn->btn) btn->btn->setDisabled(redoDisable);
}
