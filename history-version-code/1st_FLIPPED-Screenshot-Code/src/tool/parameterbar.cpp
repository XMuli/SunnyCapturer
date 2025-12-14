// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "parameterbar.h"
#include <QColor>
#include <QSize>
#include <QPainter>
#include <QBoxLayout>
#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QDebug>
#include "../xglobal.h"
#include "../widget/xhorizontalline.h"
#include "../widget/xverticalline.h"
#include "../core/xlog.h"
#include "base/blurwidget.h"
#include "../screen/datamaid.h"

ParameterBar::ParameterBar(Qt::Orientations orien, QWidget* parent)
    : QWidget(parent)
    , m_scal(DATAMAID->scale())
    , m_orien(orien)
    , m_layout(nullptr)
    , m_blur(std::make_unique<BlurWidget>(this))
    , m_rectBar(nullptr)
    , m_ellipseBar(nullptr)
    , m_arrowBar(nullptr)
    , m_mosaicBar(nullptr)
    , m_textBar(nullptr)
    , m_serialnumberShape(nullptr)
    , m_serialnumberType(nullptr)
    , m_lienWidthBar(nullptr)
    , m_colorBar(new ColorParaBar(ColorParaBarMode::CPB_ParaBar, orien))
{
    initUI();
    m_colorBar->setVisible(false);
    connect(m_colorBar, &ColorParaBar::sigColorChange, this, &ParameterBar::sigSelColor);
}

ParameterBar::~ParameterBar()
{
    m_rectBar->deleteLater();
    m_ellipseBar->deleteLater();
    m_arrowBar->deleteLater();
    m_mosaicBar->deleteLater();
    m_textBar->deleteLater();
    m_serialnumberShape->deleteLater();
    m_serialnumberType->deleteLater();
    m_lienWidthBar->deleteLater();
    m_colorBar->deleteLater();
}

void ParameterBar::setBlurBackground(const QPixmap &pix, double blurRadius)
{
    if (!m_blur)
        return;

    m_blur->setPixmap(pix, blurRadius);
    m_blur->lower();
}

void ParameterBar::addWidget(QWidget *w)
{
    if (m_layout && w)
        m_layout->addWidget(w);
}

void ParameterBar::addSpacer()
{
    if (!m_layout)
        return;

    if (m_orien == Qt::Horizontal)
        m_layout->addWidget(new XVerticalLine(B_SPACER_LINE_HEIGHT * m_scal, this));
    else
        m_layout->addWidget(new XHorizontalLine(B_SPACER_LINE_HEIGHT * m_scal, this));
}

// 由于部分图片不规范，所以用此函数修正一下
QString ParameterBar::amendSvgShow(QString path)
{
    static std::map<QString, QString> map = { {"rectangle_fill.svg", "rect"},
                                              {"ellipse_fill.svg", "rect"},
                                              {"line_width_1.svg", "circle"},
                                              {"line_width_2.svg", "circle"},
                                              {"line_width_3.svg", "circle"}};

    auto itor = map.find(path.split('/').last());
    return (itor != map.cend()) ? itor->second : "path";  // shape
}

void ParameterBar::creatorParaBar(QPointer<ManageBar>& manageBar, const QString& path, const QStringList& items, const bool exclusive, const int defaultIdx)
{
    if (!manageBar)
        manageBar = new ManageBar(m_orien, this);

    QMap<QString, QString> map;
    for (int i = 0; i < items.size(); ++i)
        map.insert(QString("tb%1").arg(i), path + items[i] + ".svg");

    QButtonGroup* group = new QButtonGroup(this);
    group->setExclusive(exclusive); // toolBtn 互斥
    auto it = map.begin();
    for (int i = 0; i < items.size(); ++i) {
        QToolButton* tb = new QToolButton();
        tb->setStyleSheet("border-style:none");
        tb->setIconSize(QSize(ICON_WIDTH * m_scal, ICON_WIDTH * m_scal));
        tb->setFixedSize(QSize(ICON_WIDTH * m_scal, ICON_WIDTH * m_scal));
        tb->setObjectName(it.key());
        tb->setIcon(QIcon(it.value()));
        tb->setToolButtonStyle(Qt::ToolButtonIconOnly);
        tb->setAutoRaise(true);
        tb->setCheckable(true);
        tb->setChecked(false);
        if (i == defaultIdx) {
            tb->setChecked(true);
            tb->setIcon(DATAMAID->ChangeSVGColor(it.value(), amendSvgShow(it.value()), DATAMAID->paraValue("borderColor").toString(), QSize(ICON_WIDTH, ICON_WIDTH) * DATAMAID->scale()));
        }

        tb->setProperty("path", it.value());
        group->addButton(tb);
        manageBar->addWidget(tb);
        it++;
    }

    connect(group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonReleased), this, &ParameterBar::onTBReleased);
}

void ParameterBar::initRectBar()
{
    const QString path(":/resources/tool_para/rectangles/");
    const QStringList list = { "rectangle",  "rectangle_fill" };
    creatorParaBar(m_rectBar, path, list);
}

void ParameterBar::initEllipseBar()
{
    const QString path(":/resources/tool_para/ellipses/");
    const QStringList list = { "ellipse",  "ellipse_fill" };
    creatorParaBar(m_ellipseBar, path, list);
}

void ParameterBar::initMosaicBar()
{
    const QString path(":/resources/tool_para/mosaics/");
    const QStringList list = { "smooth", "pixelated" };
    creatorParaBar(m_mosaicBar, path, list);
}

void ParameterBar::initTextBar()
{
    const QString path(":/resources/tool_para/text/");
    const QStringList list = { "bold", "italic", "outline" };
    creatorParaBar(m_textBar, path, list, false, -1);
}

void ParameterBar::initArrowBar()
{
    const QString path(":/resources/tool_para/arrows/");
    const QStringList list = { "arrow_1", "arrow_2", "arrow_3", "arrow_4" };
    creatorParaBar(m_arrowBar, path, list);
}

void ParameterBar::initLineWidthBar()
{
    const QString path(":/resources/tool_para/line_width/");
    const QStringList list = { "line_width_1", "line_width_2", "line_width_3" };
    creatorParaBar(m_lienWidthBar, path, list, true, 1);
}

void ParameterBar::initSerialnumberBar()
{
    const QString path(":/resources/tool_para/serialnumber/");
    const QStringList listShape = { "rectangle_fill", "ellipse_fill" };
    creatorParaBar(m_serialnumberShape, path, listShape);

    const QStringList listType = { "numbers", "letters" };
    creatorParaBar(m_serialnumberType, path, listType);
}

void ParameterBar::removeAllBar()
{
    if (!m_layout)
        return;

    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        const auto& it = child->widget();
        QFrame* tLine = nullptr;
        m_orien == Qt::Horizontal ? tLine = qobject_cast<XVerticalLine*>(it) : tLine = qobject_cast<XHorizontalLine*>(it);
        m_layout->removeWidget(it);
        it->setParent(nullptr);
        it->setVisible(false);
        if (tLine)
            it->deleteLater();
    }
}

void ParameterBar::onEnableDraw(bool enable)
{
    setVisible(enable);
}

void ParameterBar::onSelShape(DrawShape shape, bool checked)
{
    removeAllBar();

    #define ADDWIDGET_AND_SHOE(w, bAddSpacer) \
        addWidget(w); \
        w->setVisible(true); \
        if (bAddSpacer) \
            addSpacer();

    if (shape == DrawShape::NoDraw) {
        XLOG_INFO("[shape] is DrawShape::NoDraw.");
    } else if (shape == DrawShape::Rectangles) {
        ADDWIDGET_AND_SHOE(m_rectBar, true);
        ADDWIDGET_AND_SHOE(m_lienWidthBar, true);
        ADDWIDGET_AND_SHOE(m_colorBar, false);
    } else if (shape == DrawShape::Ellipses) {
        ADDWIDGET_AND_SHOE(m_ellipseBar, true);
        ADDWIDGET_AND_SHOE(m_lienWidthBar, true);
        ADDWIDGET_AND_SHOE(m_colorBar, false);
    } else if (shape == DrawShape::LineWidth) {
        ADDWIDGET_AND_SHOE(m_lienWidthBar, true);
        ADDWIDGET_AND_SHOE(m_colorBar, false);
    } else if (shape == DrawShape::Arrows) {
        ADDWIDGET_AND_SHOE(m_arrowBar, true);
        ADDWIDGET_AND_SHOE(m_lienWidthBar, true);
        ADDWIDGET_AND_SHOE(m_colorBar, false);
    } else if (shape == DrawShape::CustomPath) {
        ADDWIDGET_AND_SHOE(m_lienWidthBar, true);
        ADDWIDGET_AND_SHOE(m_colorBar, false);
    } else if (shape == DrawShape::Mosaics) {
        ADDWIDGET_AND_SHOE(m_mosaicBar, true);
        ADDWIDGET_AND_SHOE(m_lienWidthBar, false);
    } else if (shape == DrawShape::Text) {
        ADDWIDGET_AND_SHOE(m_textBar, true);
        ADDWIDGET_AND_SHOE(m_lienWidthBar, true);
        ADDWIDGET_AND_SHOE(m_colorBar, false);
    } else if (shape == DrawShape::SerialNumberShape) {
        ADDWIDGET_AND_SHOE(m_serialnumberShape, true);
        ADDWIDGET_AND_SHOE(m_serialnumberType, true);
        ADDWIDGET_AND_SHOE(m_colorBar, false);
    } else {
        XLOG_ERROR("[shape] is not any enumeration.");
    }

    setVisible(checked);
    adjustSize();
    update();
}

void ParameterBar::enterEvent(QEvent* event)
{
    setCursor(Qt::ArrowCursor);
}

void ParameterBar::resizeEvent(QResizeEvent *event)
{
    if (m_blur)
        m_blur->setGeometry(0, 0, width(), height());

    return QWidget::resizeEvent(event);
}

void ParameterBar::onTBReleased(QAbstractButton* btn)
{
    const auto& parent = btn->parentWidget();
    if (!btn || !parent) 
        return;

    const auto selectedTb = qobject_cast<QToolButton*>(btn);
    const QSize iconSize = QSize(ICON_WIDTH, ICON_WIDTH) * DATAMAID->scale();
    for (auto& it : parent->findChildren<QToolButton *>()) {
        if (it == selectedTb) {
            QString name = it->objectName();
            DrawShape shap = DrawShape::NoDraw;

            if (parent == m_rectBar) {
                shap = DrawShape::Rectangles;
            } else if (parent == m_ellipseBar) {
                shap = DrawShape::Ellipses;
            } else if (parent == m_arrowBar) {
                shap = DrawShape::Arrows;
            } else if (parent == m_mosaicBar) {
                shap = DrawShape::Mosaics;
            } else if (parent == m_textBar) {
                shap = DrawShape::Text;
            } else if (parent == m_lienWidthBar) {
                shap = DrawShape::LineWidth;
            } else if (parent == m_serialnumberShape || parent == m_serialnumberType) {
                if (parent == m_serialnumberShape) shap = DrawShape::SerialNumberShape;
                if (parent == m_serialnumberType) shap = DrawShape::SerialNumberType;
            } else {
            }

            emit sigParaBtnId(shap, selectedTb);
        }

        QString path = it->property("path").value<QString>();
        const QIcon& icon = it->isChecked() ? DATAMAID->ChangeSVGColor(path, amendSvgShow(path), DATAMAID->paraValue("borderColor").toString(), iconSize) : QIcon(path);
        it->setIconSize(iconSize);
        it->setIcon(icon);
    }
}

void ParameterBar::initUI()
{
    if (m_orien == Qt::Horizontal)
        m_layout = new QHBoxLayout(this);
    else
        m_layout = new QVBoxLayout(this);

    m_layout->setMargin(0);
    m_layout->setSpacing(PB_ITEM_SPACE);
    setContentsMargins(0, 0, 0, 0);

    initRectBar();
    initEllipseBar();
    initArrowBar();
    initMosaicBar();
    initTextBar();
    initSerialnumberBar();
    initLineWidthBar();
    m_layout->addWidget(m_rectBar);
    m_layout->addWidget(m_ellipseBar);
    m_layout->addWidget(m_arrowBar);
    m_layout->addWidget(m_mosaicBar);
    m_layout->addWidget(m_textBar);
    m_layout->addWidget(m_serialnumberShape);
    m_layout->addWidget(m_serialnumberType);
    m_layout->addWidget(m_lienWidthBar);
    m_layout->addWidget(m_colorBar);
}
