// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "hotkeyswidget.h"
#include <map>
#include <QBoxLayout>
#include <QList>
#include <QMouseEvent>
#include <QPushButton>
#include "../xglobal.h"
#include "../screen/tray.h"
#include "../widget/xkeysequenceedit.h"
#include "../screen/drawhelper.h"
#include "../screen/datamaid.h"
#include "appellation.h"
#include "../screen/tray.h"

HotkeysWidget::HotkeysWidget(QWidget *parent) : QWidget(parent)
{
    double m_scale = DATAMAID->scale();

    setContentsMargins(0, 0, 0, 0);
    QVBoxLayout* vLay = new QVBoxLayout(this);
    vLay->setContentsMargins(THV_MARGIN_HOR, THV_MARGIN_VER_TOP, THV_MARGIN_HOR, THV_MARGIN_VER_BOTTOM);

    int i = 0;
    int j = 0;
    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setVerticalSpacing(THG_SPACING_VER);
    grid->setHorizontalSpacing(THG_SPACING_HOR);
    grid->setColumnStretch(0, 8);
    grid->setColumnStretch(1, 9);

    QStringList list = {  DATAMAID->paraValue(thScrnCapture).toString()
                         ,DATAMAID->paraValue(thFullScreen).toString()
                         ,DATAMAID->paraValue(thDelayCapture).toString()};
    std::map<XKeySequenceEdit*, const QString> vHkEdit;
    int idx = 0;
    for (auto& it : Tray::instance().getVHotKeys()) {
        auto& hk = std::get<0>(it);
        QString& hotkey = std::get<1>(it);
        QString& describe = std::get<2>(it);
        qDebug() << "----->t0:" << hk << "    hotkey:" << hotkey << "    describe:" << describe;

        XKeySequenceEdit* pEdit = new XKeySequenceEdit(QKeySequence(hotkey));
        vHkEdit.insert(std::make_pair(pEdit, list.at(idx++)));
        pEdit->setObjectName(describe);
        pEdit->setMinimumWidth(110 * m_scale);
        const bool reg = hk->isRegistered();
        if (reg) {
            pEdit->setStyleSheet("background-color: #98fb98;");
        } else {
            pEdit->setStyleSheet("background-color: #ff7f50;");
        }

        grid->addWidget(new QLabel(describe + ":"), i, j, 1, 1, Qt::AlignRight);
        grid->addWidget(pEdit, i++, j + 1, 1, 1, Qt::AlignLeft);
        connect(pEdit, &XKeySequenceEdit::sigKeySeqChanged, &(Tray::instance()), &Tray::onKeySequenceChanged);

//        if (std::get<3>(it) == Tray::ScrnShotType::SST_FullScreen)
//            grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - THV_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);
    }
//    qDebug() << "tabHotkeys:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->setContentsMargins(0, 0, 0, 0);
    hLay->addSpacing(0);
    hLay->addStretch(7);
    auto btn = new QPushButton(tr("Reset"), this);
    if (btn) {
        btn->setObjectName(thReset);
        hLay->addWidget(btn, 1, Qt::AlignRight);

        connect(btn, &QPushButton::released, this, [vHkEdit]() {
            for (auto& it : vHkEdit) {
                it.first->setKeySequence(it.second);
                emit it.first->sigKeySeqChanged(it.first->keySequence());
            }});
    }
    vLay->addLayout(hLay, 1);



}

void HotkeysWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    // 点击 快捷键编辑框 之外，焦点消失
    QList<XKeySequenceEdit*> list = findChildren<XKeySequenceEdit*>();
    for (const auto& it : list) {
        if (!it->geometry().contains(mapFromGlobal(QCursor::pos())))
            it->clearFocus();
    }
}
