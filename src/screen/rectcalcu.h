// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef PICSHOT_RECTCALCU_H
#define PICSHOT_RECTCALCU_H

#include "../xglobal.h"
#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>

QT_BEGIN_NAMESPACE
class QRect;
class QPoint;
QT_END_NAMESPACE

namespace XC {
	enum class ScrnOperate {
		SO_Wait,                                // 基础的等待状态（未有，和已有矩形局域）
		SO_Select,                              // 基础的选中状态
		SO_Move,                                // 基础的移动状态
		SO_Draw,                                // 基础的绘画状态    
		SO_Stretch,                             // 基础的拉伸状态
	};

	enum class CursorArea {
	   Left = 0x00000001,
	   Top = 0x00000002,
	   Right = 0x00000004,
	   Bottom = 0x00000008,
	   Horizontal = Left | Right,
	   Vertical = Top | Bottom,
	   Edga = Horizontal | Vertical,
	
	   TopLeft = 0x00000010,
	   TopRight = 0x00000020,
	   BottomLeft = 0x00000040,
	   BottomRight = 0x00000080,
	   TLAndBR = TopLeft | BottomRight,
	   TRAndBL = TopRight | BottomLeft,
	   Corner = TLAndBR | TRAndBL,
	
	   Border = Edga | Corner,               // 边框
	   External = 0x00000100,                // 外部
	   Internal = 0x00000200,                // 内部
	   Unknow
    };
}

// ** 方便使用枚举 **
using namespace XC;


class ScreenShot;
// 矩形计算
class RectCalcu
{
public:
	RectCalcu(ScreenShot* pSrnShot);
	~RectCalcu();

    static QRect getRect(QPoint pos1, QPoint pos2);
	const QRect getSelRect() const;

	void clear();
	void setClear(bool clear);
	bool isClear();
    const CursorArea getCursorArea(const QPoint pos, bool details = false) const;

	QRect& limitBound(QRect& rt, QRect maxRt);

	const QRect calcurRsultOnce();
    void setRtSel(const QRect rt);

private:
	const QRect getStretchRect() const; // 仅拉伸时使用

public:
	QRect getExteRect(QRect& rect, int interval = HAIF_INTERVAL) const; // 获取外部矩形
	QRect getInteRect(QRect& rect, int interval = HAIF_INTERVAL) const; // 获取内部矩形
	
public:
	QPoint pos1;
	QPoint pos2;
	ScrnOperate scrnType;          // 程序状态（对应此时鼠标的操作类型）

private:
	QRect  rtSel;               // 由 pos1, pos2 所得
	bool m_bClear;              // 当前清理状态
	CursorArea cursArea;        // 光标对应区域
	ScreenShot* m_scrnShot;     // 用来判定 当前选中 移动 形状
};

#endif //PICSHOT_RECTCALCU_H
