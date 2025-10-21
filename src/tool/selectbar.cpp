// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>
#include "selectbar.h"
#include <vector>
#include <tuple>
#include <QString>
#include <QToolButton>
#include <QStringList>
#include <QBoxLayout>
#include <QIcon>
#include <QButtonGroup>
#include <QPainter>
#include <QDebug>
#include "../xglobal.h"
#include "../widget/xhorizontalline.h"
#include "../widget/xverticalline.h"
#include "../screen/datamaid.h"

SelectBar::SelectBar(Qt::Orientations orien, QWidget *parent)
    : QWidget(parent)
    , m_scal(DATAMAID->scale())
    , m_orien(orien)
    , m_layout(nullptr)
    , m_blur(std::make_unique<BlurWidget>(this))
{
    initUI();
    // pTb, btnName, tr(), bCheckable, bAddSpacer
    m_vBtns = {
        std::make_tuple(nullptr, "rectangle", tr("Rectangle"), true, false),
        std::make_tuple(nullptr, "ellipse", tr("Ellipse"), true, false),
        std::make_tuple(nullptr, "arrow", tr("Arrow"), true, false),
        std::make_tuple(nullptr, "custompath", tr("CustomPath"), true, false),
        std::make_tuple(nullptr, "mosaic", tr("Mosaic"), true, false),
        std::make_tuple(nullptr, "text", tr("Text"), true, false),
        std::make_tuple(nullptr, "serialnumber", tr("SerialNumber"), true, false),

        std::make_tuple(nullptr, "pin", tr("Pin"), false, true),
        std::make_tuple(nullptr, "revocation", tr("Revocation"), false, false),
        std::make_tuple(nullptr, "renewal", tr("Renewal"), false, true),
        std::make_tuple(nullptr, "save", tr("Save"), false, false),
        std::make_tuple(nullptr, "cancel", tr("Cancel"), false, false),
        std::make_tuple(nullptr, "finish", tr("Finish"), false, false)
    };

    for (auto& it : m_vBtns){
        auto& tb = std::get<0>(it);
        const QString& btnName = std::get<1>(it);
        const QString& btnTip = std::get<2>(it);
        const bool& bCheckable = std::get<3>(it);
        const bool& bAddSpacer = std::get<4>(it);

        tb = new QToolButton();
        tb->setStyleSheet("border-style:none");
        tb->setObjectName(btnName);
        tb->setToolButtonStyle(Qt::ToolButtonIconOnly);
        tb->setAutoRaise(true);
        tb->setIcon(QIcon(":/resources/tool/" + btnName + ".svg"));
        tb->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT) * m_scal);
        tb->setContentsMargins(0, 0, 0, 0);
        tb->setFixedSize(QSize(ICON_WIDTH, ICON_HEIGHT) * m_scal);
        tb->setToolTip(btnTip);
        tb->setChecked(false);
        tb->setCheckable(bCheckable);
        addWidget(tb);

        if (bAddSpacer) addSpacer();
        connect(tb, &QToolButton::released, this, &SelectBar::onToolBtn);
    }

    //btnExclusiveManage();
}

void SelectBar::setBlurBackground(const QPixmap &pix, int blurRadius)
{
    if (!m_blur)
        return;

    m_blur->setPixmap(pix, blurRadius);
    m_blur->lower();
}

void SelectBar::initUI()
{
    if (!m_layout) {
        if (m_orien == Qt::Horizontal)
            m_layout = new QHBoxLayout(this);
        else
            m_layout = new QVBoxLayout(this);
    }

    setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);
    m_layout->setContentsMargins(SB_MARGIN_HOR, SB_MARGIN_VER, SB_MARGIN_HOR, SB_MARGIN_VER);
    m_layout->setSpacing(SB_ITEM_SPACE);
}

void SelectBar::addWidget(QWidget *w)
{
    if (w && m_layout)
        m_layout->addWidget(w);
}

void SelectBar::addSpacer()
{
    if (!m_layout)
        return;

    if (m_orien == Qt::Horizontal)
        m_layout->addWidget(new XVerticalLine(B_SPACER_LINE_HEIGHT * m_scal, this));
    else
        m_layout->addWidget(new XHorizontalLine(B_SPACER_LINE_HEIGHT * m_scal, this));
}


void SelectBar::onToolBtn()
{
    QToolButton* tb = nullptr;
    QObject* obj = sender();
    tb = qobject_cast<QToolButton*>(obj);
    if (!obj) 
        return;

    QList<QToolButton *> listBtn = findChildren<QToolButton *>();
    for (QToolButton* it : listBtn) {
        QString path = ":/resources/tool/" + it->objectName() + ".svg";
        it->setIconSize(QSize(ICON_WIDTH, ICON_WIDTH) * DATAMAID->scale());
        const bool bDrawTb = tb->isCheckable() == true;   // 绘画按钮
        const bool bRevoOrReneTb = tb->objectName() == "revocation" || tb->objectName() == "renewal";
        const bool bOtherTb = (!tb->isCheckable() && tb->objectName() != "revocation" && tb->objectName() != "renewal");

        if (it == tb) {
            if (bDrawTb) {
                QIcon icon(path);
                if (it->isChecked())
                    icon = DATAMAID->ChangeSVGColor(path, "path", DATAMAID->paraValue("borderColor").toString(), QSize(ICON_WIDTH, ICON_WIDTH) * DATAMAID->scale());
                it->setIcon(icon);
            } /*else if (bRevoOrReneTb) {
            } else if (bOtherTb) {
            }*/
        } else {
            if (it->objectName() == "text" && it->isChecked())  // fix: textedit edits half of the selected other drawing controls
                emit sigInterruptEdit(QCursor::pos());

            if (bDrawTb) {
                it->setChecked(false);
                it->setIcon(QIcon(path));  // 频繁切换 icon 应该不会有泄露，后面确认下
            } /*else if (bRevoOrReneTb) {
            } else if (bOtherTb) {
            }*/
        }
    }

//    for (QToolButton* it : listBtn)
//        qDebug() << "it:" << it << "   it->isCheckable():" << it->isCheckable() << "  it->isChecked():" << it->isChecked();
//    qDebug() << "---------------------------------------------\n";
    
    emit sigEnableDraw(isEnableDraw());  // true: btn is pressed, in drawing state
    bool isChecked = tb->isChecked();
    
    if (tb->objectName() == "rectangle") {
        emit sigSelShape(XC::DrawShape::Rectangles, isChecked);
    } else if (tb->objectName() == "ellipse") {
        emit sigSelShape(XC::DrawShape::Ellipses, isChecked);
    } else if (tb->objectName() == "line") {
        emit sigSelShape(XC::DrawShape::LineWidth, isChecked);
    } else if (tb->objectName() == "arrow") {
        emit sigSelShape(XC::DrawShape::Arrows, isChecked);
    } else if (tb->objectName() == "custompath") {
        emit sigSelShape(XC::DrawShape::CustomPath, isChecked);
    } else if (tb->objectName() == "mosaic") {
        emit sigSelShape(XC::DrawShape::Mosaics, isChecked);
    } else if (tb->objectName() == "text") {
        emit sigSelShape(XC::DrawShape::Text, isChecked);
    } else if (tb->objectName() == "serialnumber") {
        emit sigSelShape(XC::DrawShape::SerialNumberShape, isChecked);
    } else if (tb->objectName() == "pin") {
        emit sigPin();
    } else if (tb->objectName() == "revocation") {
        emit sigRevocation();
    } else if (tb->objectName() == "renewal") {
        emit sigRenewal();
    } else if (tb->objectName() == "save") {
        emit sigSave();
    } else if (tb->objectName() == "cancel") {
        emit sigCancel();
    } else if (tb->objectName() == "finish") {
        emit sigFinish();
    } else{
    }
}

inline bool SelectBar::isEnableDraw()
{
    bool ret = false;
    for (auto& it : m_vBtns) {
        auto& tb = std::get<0>(it);
        if (tb->isCheckable() && tb->isChecked())
            ret = true;;
    }

    return ret;
}

void SelectBar::enterEvent(QEvent* event)
{
    setCursor(Qt::ArrowCursor);
}

void SelectBar::resizeEvent(QResizeEvent *event)
{
    if (!m_blur)
        return;

    m_blur->setGeometry(0, 0, width(), height());
    return QWidget::resizeEvent(event);
}
