// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "managebar.h"
#include <QBoxLayout>
#include "../../xglobal.h"
#include "../../widget/xhorizontalline.h"
#include "../../widget/xverticalline.h"
#include "../../screen/drawhelper.h"
#include "../../screen/datamaid.h"

ManageBar::ManageBar(Qt::Orientations orien /*= Qt::Horizontal*/, QWidget* parent /*= nullptr*/)
    : QWidget(parent)
    , m_scal(DATAMAID->scale())
    , m_orien(orien)
    , m_layout(nullptr)
{
    if (m_orien == Qt::Horizontal)
        m_layout = new QHBoxLayout(this);
    else
        m_layout = new QVBoxLayout(this);

    setContentsMargins(MB_MARGIN_HOR, MB_MARGIN_VER, MB_MARGIN_HOR, MB_MARGIN_VER);
    setLayout(m_layout);
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
}

void ManageBar::addWidget(QWidget* w)
{
    if (w)
        m_layout->addWidget(w);
}

void ManageBar::addSpacer()
{
    if (m_orien == Qt::Horizontal)
        m_layout->addWidget(new XVerticalLine(B_SPACER_LINE_HEIGHT * m_scal, this));
    else
        m_layout->addWidget(new XHorizontalLine(B_SPACER_LINE_HEIGHT * m_scal, this));
}
