#include "absbtnsctrl.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "horspacerline.h"
#include "verspacerline.h"

AbsBtnsCtrl::AbsBtnsCtrl(const Qt::Orientations &orien, QWidget *parent)
    : QWidget(parent)
    , m_orien(orien)
    , m_layout(nullptr)
{
    if (m_orien == Qt::Horizontal) m_layout = new QHBoxLayout(this);
    else if (m_orien == Qt::Vertical) m_layout =  new QVBoxLayout(this);

    setContentsMargins(0, 0, 0, 0);
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setLayout(m_layout);
}

void AbsBtnsCtrl::addWidget(QWidget *w, const bool& bAddSpacer)
{
    if (w) {
        m_layout->addWidget(w, Qt::AlignCenter);

        if (bAddSpacer) {
            addSpacerLine(); // 实际没有使用到
//            addSpacerItem(100);   // 都解开可以看效果
//            addSpacerLine(10); // 实际没有使用到
        }
    }
}

// 实际没有使用到
void AbsBtnsCtrl::addSpacerLine(const int& length)
{
    if (!m_layout) return;
    if (m_orien == Qt::Horizontal) m_layout->addWidget(new VerSpacerLine(length, this), Qt::AlignCenter);
    else if (m_orien == Qt::Vertical) m_layout->addWidget(new HorSpacerLine(length + 5, this), Qt::AlignCenter);
}

// 实际没有使用到
void AbsBtnsCtrl::addSpacerItem(const int &length)
{
    // 子控件里面有弹簧拉伸，所以最父亲层的弹簧就也就不能尽情的压缩子控件， 虽然不影响效果，以后再回头【直接用】 design 来看看这个点
    QSpacerItem *spacer = nullptr;
    if (m_orien == Qt::Horizontal)  {
        spacer = new QSpacerItem(length, 1, QSizePolicy::Expanding, QSizePolicy::Minimum); // Expanding 替换看效果
    } else if (m_orien == Qt::Vertical) {
        spacer = new QSpacerItem(1, length, QSizePolicy::Minimum, QSizePolicy::Expanding);
    }

    m_layout->addSpacerItem(spacer);
}

void AbsBtnsCtrl::setFixSpacing(const int &val)
{
    if (m_layout) m_layout->setSpacing(val);
}
