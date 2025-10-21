// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "colorparabar.h"
#include <QLabel>
#include <QBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout> 
#include <QList>
#include <QVector>
#include <QMap>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QColorDialog>
#include <QMetaProperty>
#include "../../xglobal.h"
#include "../../widget/xlabel.h"
#include "../../screen/datamaid.h"

//test
#include <QMessageBox>
#include <QDebug>

ColorParaBar::ColorParaBar(ColorParaBarMode mode, Qt::Orientations orien, QWidget *parent)
    : QWidget(parent)
    , m_scal(DATAMAID->scale())
    , m_orien(orien)
    , m_layout(nullptr)
    , m_curXLab(nullptr)
    , m_curCol()
{
    m_labMap = { {"lab0_Red", "#DB000F"}
   , {"lab1_Yellow", "#FFCF53"}
   , {"lab2_Green", "#12F63B"}
   , {"lab3_Blue", "#0E70FF"}
   , {"lab4_Pink", "#FB4288"}
   , {"lab5_Black", "#323232"}
   , {"lab6_White", "#FBFBFB"}
   , {"lab7_Pick", ""} };  // 最后一个彩色

    if (mode == ColorParaBarMode::CPB_ParaBar) {
        m_layout = new QGridLayout(this);
        QGridLayout* layout = static_cast<QGridLayout *>(m_layout);
        int rowMax = 0;
        int colMax = 0;

        if (m_orien == Qt::Horizontal) {
            rowMax = 2;
            colMax = m_labMap.size() / rowMax;
        } else {
            colMax = 2;
            rowMax = m_labMap.size() / colMax;
        }

        auto it = m_labMap.begin();
        for (int i = 0; i < rowMax; ++i) {
            for (int j = 0; j < colMax; ++j) {
                XLabel* lab = new XLabel(this);
                if (i == 0 && j == 0) { // 初始化为默认的第一个 XLabel 颜色 
                    m_curXLab = lab;
                    m_curCol.setNamedColor(it.value());
                }

                lab->setObjectName(it.key());
                int width(CPB_PB_LABEL_WIDTH * m_scal);
                lab->setFixedSize(width, width);
                lab->installEventFilter(this);

                if ((it + 1) == m_labMap.end()) // 最后一个渐变色
                    lab->setRainbow(true);
                else
                    lab->setColor(it.value(), 1);

                layout->addWidget(lab, i, j);
                it++;
            }
        }

        //this->installEventFilter(this);
        layout->setContentsMargins(CPB_PB_MARGIN_HOR, CPB_PB_MARGIN_VER, CPB_PB_MARGIN_HOR, CPB_PB_MARGIN_VER);
        layout->setHorizontalSpacing(CPB_PB_SPACING_HOR * m_scal);
        layout->setVerticalSpacing(CPB_PB_SPACING_VER * m_scal);  // 检查比例一下
    } else if (mode == ColorParaBarMode::CPB_HighLight) {
        m_layout = new QHBoxLayout(this);
        QHBoxLayout* layout = static_cast<QHBoxLayout*>(m_layout);
        for (auto it = m_labMap.begin(); it != m_labMap.end(); ++it) {
                XLabel* lab = new XLabel(this);
                if (it == m_labMap.cbegin()) { // 初始化为默认的第一个 XLabel 颜色 
                    m_curXLab = lab;
                    m_curCol.setNamedColor(it.value());
                }

                lab->setObjectName(it.key());
                int width(CPB_HL_LABEL_WIDTH * m_scal);
                lab->setFixedSize(width, width);
                lab->installEventFilter(this);

                if (it + 1 == m_labMap.cend())  // 最后一个渐变色
                    lab->setRainbow(true);
                else
                    lab->setColor(it.value(), 1);

                layout->addWidget(lab);
        }

        //this->installEventFilter(this);
        layout->setContentsMargins(CPB_HL_MARGIN_HOR, CPB_HL_MARGIN_VER, CPB_HL_MARGIN_HOR, CPB_HL_MARGIN_VER);
        layout->setSpacing(CPB_HL_SPACING_HOR * m_scal);
    }
    
    setLayout(m_layout);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
}

void ColorParaBar::setOrientations(Qt::Orientations orien)
{
    m_orien = orien;
}

QColor ColorParaBar::setCurColor(const QColor col)
{
    if (col.isValid() || m_curCol.name() != col.name()) {
        m_curCol.setNamedColor(col.name());
        onUpdateSel(col);
    }

    return m_curCol;
}

const QColor ColorParaBar::getCurColor()
{
    return m_curCol;
}

void ColorParaBar::onUpdateSel(const QColor& col)
{
    m_curXLab = findChild<XLabel*>(m_labMap.lastKey());
    m_curCol.setNamedColor(col.name());

    for (auto it = m_labMap.cbegin(); it != m_labMap.cend(); ++it){
        qDebug() << "col.name():" << col.name() << "   it.value():" << it.value();
        if (it.value().compare(col.name(), Qt::CaseInsensitive) == 0) {
            m_curXLab = findChild<XLabel*>(it.key());
            break;
        }
    }
}

// #see: 用法 https://blog.csdn.net/xiezhongyuan07/article/details/79992099
// return 为 false，表示其余事件交还给目标对象处理； true 表示该事件不再进一步处理
bool ColorParaBar::eventFilter(QObject *watched, QEvent *event)
{
    XLabel* lab = qobject_cast<XLabel *>(watched);
    if (!lab) return false;

    for (auto& it : findChildren<XLabel*>())
        it->setChecked(it == m_curXLab);

    if (event->type() == QEvent::MouseButtonRelease) {
        if (lab->objectName().compare(m_labMap.lastKey()) == 0) {
            m_curCol = QColorDialog::getColor(lab->palette().color(QPalette::Window), this, tr("select color"));
        } else {
            const auto& it = m_labMap.find(lab->objectName());
            m_curCol.setNamedColor(it.value());
        }

        emit sigColorChange(m_curCol);
        m_curXLab = lab;
        update();
        return true;

    } else if (event->type() == QEvent::MouseButtonPress) {
        return true;
    } else {
        return false;
    }
}

void ColorParaBar::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
//    if (!m_curXLab)
//        return;

//    QPainter pa(this);
//    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
//    QPen pen(m_curCol);
//    pen.setWidth(CPB_WIDTH_SELECTED);
//    pa.setPen(pen);
//    pa.setBrush(Qt::NoBrush);

//    int margin = CPB_MARGIN_SELECTED;
//    auto topLeft = m_curXLab->mapToGlobal(QPoint(0, 0)); // 子控件的窗口的（左上角的）绝对坐标; QPoint(0, 0) 为子控件的左上角坐标，子窗口的总是(0, 0)
//    topLeft = mapFromGlobal(topLeft);              // 切换为相对父窗口的绝对坐标
//    const QRect rt = QRect(topLeft, m_curXLab->size()).adjusted(-margin, -margin, margin, margin);
//    pa.drawEllipse(rt.center(), rt.width() / 2, rt.height() / 2);
}
