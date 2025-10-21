// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "rectcalcu.h"
#include <QRect>
#include <QPoint>
#include "screenshot.h"
#include "../core/xlog.h"

// 返回副本
const QRect RectCalcu::getStretchRect() const
{
	QRect rt;
	if (scrnType != ScrnOperate::SO_Stretch)
		return rt;

	rt = rtSel;
	const int width = pos2.x() - pos1.x();
	const int height = pos2.y() - pos1.y();
	const QPoint offset(pos2 - pos1);
	const CursorArea cursArea = getCursorArea(pos1, true);

	if (cursArea == CursorArea::Left) {
		rt.setLeft(rt.left() + width);
	} else if (cursArea == CursorArea::Top) {
		rt.setTop(rt.top() + height);
	} else if (cursArea == CursorArea::Right) {
		rt.setRight(rt.right() + width);
	} else if (cursArea == CursorArea::Bottom) {
		rt.setBottom(rt.bottom() + height);
	} else if (cursArea == CursorArea::TopLeft) {
		rt.setTopLeft(rt.topLeft() + offset);
	} else if (cursArea == CursorArea::TopRight) {
		rt.setTopRight(rt.topRight() + offset);
	} else if (cursArea == CursorArea::BottomLeft) {
		rt.setBottomLeft(rt.bottomLeft() + offset);
	} else if (cursArea == CursorArea::BottomRight) {
		rt.setBottomRight(rt.bottomRight() + offset);
	} else {
		XLOG_ERROR("此时处于未知状态 CursorArea::Unknow 513");
	}

    // 修复拉伸矩形为负数但却可以绘画的场景
    if (!rt.isValid()) {
        if (rt.width() <= 0 || rt.height() <= 0) {
            QPoint p1(rt.topLeft());
            QPoint p2(rt.topLeft() + QPoint(rt.width(), rt.height()));
            rt = getRect(p1, p2);
        }
    }

	return rt;
}

// 计算后得出结果数据，其余归零
const QRect RectCalcu::calcurRsultOnce()
{
    rtSel = getSelRect();
    pos1 = QPoint();
    pos2 = QPoint();

	return rtSel;
}

// 仅被智能窗口选中时候使用，不要随意修改此数值;
void RectCalcu::setRtSel(const QRect rt)
{
    rtSel = rt;
	pos1 = rtSel.topLeft();
	pos2 = rtSel.bottomRight();
}

// 限制选中矩形不超过虚拟屏幕的边界， rect 为当前选中矩形
QRect& RectCalcu::limitBound(QRect& rt, QRect maxRt)
{
	if (!rt.isValid())
		return rt;
	if (rt.left() <= maxRt.left())
		rt.setLeft(maxRt.left());
	if (rt.top() <= maxRt.top())
		rt.setTop(maxRt.top());
	if (rt.right() >= maxRt.right())
		rt.setRight(maxRt.right());
	if (rt.bottom() >= maxRt.bottom())
		rt.setBottom(maxRt.bottom());

	return rt;
}


// 判断当前鼠标所在区域; false 为大概区域（用来粗略计算显示光标区域即可）; true 为详细区域（精确计算所在区域，需要修改矩形大小做准备，显示不同光标）
const CursorArea RectCalcu::getCursorArea(const QPoint pos, bool details) const
{
    QRect rt(rtSel);
	if (!rt.isValid() || !rt.isValid())
		return CursorArea::External;

	QRect rtOuter = getExteRect(rt);
	QRect rtInner = getInteRect(rt);
	int interval = (rtOuter.height() - rtInner.height()) / 2;

  //  XLOG_DEBUG("矩形测试 rtOuter0({}, {}, {} * {})   rtOuter0({}, {}, {} * {})  rtOuter0({}, {}, {} * {})  rtOuter0({}, {}, {} * {})  rtOuter0({}, {}, {} * {})"
  //      , rtOuter.x(), rtOuter.y(), rtOuter.width(), rt.height()
		//, rtOuter.left(), rtOuter.top(), rtOuter.width(), rt.height()
		//, rtOuter.topLeft().x(), rtOuter.topLeft().y(), rtOuter.width(), rt.height()
		//, rtOuter.x(), rtOuter.y(), rtOuter.width(), rt.height()
  //      , rtOuter.left(), rtOuter.left(), rtOuter.width(), rt.height()
  //      , interval);

	//XLOG_DEBUG("区域分割 rtOuter({}, {}, {} * {})  rtInner({}, {}, {} * {})  interval{}"
	//	, rtOuter.left() , rtOuter.left(), rtOuter.width(), rtOuter.height()
	//	, rtInner.left(), rtInner.left(), rtInner.width(), rtInner.height()
	//	, interval);

	QRect rtLeft(rtOuter.left(), rtInner.top(), interval, rtInner.height());
	QRect rtTop(rtInner.left(), rtOuter.top(), rtInner.width(), interval);
	QRect rtRight(rtInner.right(), rtInner.top(), interval, rtInner.height());
	QRect rtBottom(rtInner.left(), rtInner.bottom(), rtInner.width(), interval);
	QRect rtTopLeft(rtOuter.left(), rtOuter.top(), interval, interval);
	QRect rtTopRight(rtInner.right(), rtOuter.top(), interval, interval);
	QRect rtBottomLeft(rtOuter.left(), rtInner.bottom(), interval, interval);
	QRect rtBottomRight(rtInner.right(), rtInner.bottom(), interval, interval);

	// 内部、外部、边框
	if (rtInner.contains(pos, true)) {
		return CursorArea::Internal;
	} else if (!rtOuter.contains(pos, true)) {
		return CursorArea::External;
	//} else if (!rtInner.contains(pos, true) && rtOuter.contains(pos, false)) {
	} else {
        if (!details) {// 模糊区域
            return CursorArea::Border;
        } else {       // 需要更详细区域
            if (rtLeft.contains(pos, true))
                return CursorArea::Left;
            else if (rtRight.contains(pos, true))
                return CursorArea::Right;
            else if (rtTop.contains(pos, true))
                return CursorArea::Top;
            else if (rtBottom.contains(pos, true))
                return CursorArea::Bottom;
            else if (rtTopLeft.contains(pos, true))
                return CursorArea::TopLeft;
            else if (rtBottomRight.contains(pos, true))
                return CursorArea::BottomRight;
            else if (rtTopRight.contains(pos, true))
                return CursorArea::TopRight;
            else if (rtBottomLeft.contains(pos, true))
                return CursorArea::BottomLeft;
            else
                return CursorArea::Unknow;
        }
	}
}

QRect RectCalcu::getRect(QPoint pos1, QPoint pos2)
{
	int xMin = qMin<int>(pos1.x(), pos2.x());
	int xMax = qMax<int>(pos1.x(), pos2.x());
	int yMin = qMin<int>(pos1.y(), pos2.y());
	int yMax = qMax<int>(pos1.y(), pos2.y());

    if (xMin == xMax && yMin == yMax) // 若是重复点，则会返回 QRect(0, 0, 1, 1);
        return QRect(xMin, xMax, 0, 0);
    else
        return QRect(QPoint(xMin, yMin), QPoint(xMax, yMax));

}

RectCalcu::RectCalcu(ScreenShot* pSrnShot)
	: pos1(0, 0)
	, pos2(0, 0)
	, scrnType(ScrnOperate::SO_Wait)
	, rtSel(0, 0, -1, -1)
	, m_bClear(false)
	, cursArea(CursorArea::Unknow)
    , m_scrnShot(pSrnShot)
{
}

RectCalcu::~RectCalcu()
{
}

// 此时并不修改其 resel 数值
const QRect RectCalcu::getSelRect() const
{
	if (scrnType == ScrnOperate::SO_Select) {
		return getRect(pos1, pos2);
	} else if (scrnType == ScrnOperate::SO_Move) {
		// TODO 2022.06.14: 单例改写了为 new 形式，故此处有一个 移动图形的 bug，且略感觉此获取方式有点不优雅
		if (m_scrnShot && m_scrnShot->isSelBorder()) // 选中绘画的矩形
			return rtSel.translated(pos2 - pos1);

		return rtSel;
	} else if (scrnType == ScrnOperate::SO_Stretch) {
		return getStretchRect(); // 返回副本

	} else {
		return rtSel;
	}
}

QRect RectCalcu::getExteRect(QRect& rect, int interval) const
{
	QPoint topLeft = rect.topLeft();
	QPoint bottomRight = rect.bottomRight();// +QPoint(1, 1);
	return QRect(QPoint(topLeft.x() - interval, topLeft.y() - interval), QPoint(bottomRight.x() + interval, bottomRight.y() + interval));
}

QRect RectCalcu::getInteRect(QRect& rect, int interval) const
{
	// TODO 2021-10-01: 考虑 rect 的width 和 height 本身 <= 2 * HAIF_INTERVAL？
	QPoint topLeft = rect.topLeft();
	QPoint bottomRight = rect.bottomRight();// +QPoint(1, 1);
	return QRect(QPoint(topLeft.x() + interval, topLeft.y() + interval), QPoint(bottomRight.x() - interval, bottomRight.y() - interval));
}


void RectCalcu::clear()
{
	pos1 = QPoint();
	pos2 = QPoint();
	rtSel = QRect();
	scrnType = ScrnOperate::SO_Wait;
	m_bClear = true;
}

void RectCalcu::setClear(bool clear)
{
	m_bClear = clear;
}

bool RectCalcu::isClear()
{
	return m_bClear;
}
