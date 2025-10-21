// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "wininfo.h"

#include <QWidget>
#include <QCursor>

#ifdef _WIN32
    //define something for Windows (32-bit and 64-bit, this part is common)
    #include "wininfo_win.h"
#elif __APPLE__
#elif __linux__
    #include "wininfo_x11.h"
#else
    # error "Unknown compiler"
#endif

// pt 光标当前位置；bPrevCache: true 获取所有窗口模式: false 实时获取窗口模式
const WinData* WinInfo::targWinInfo(WinID hWnd, bool bPrevCache)
{
    if (!m_pWinList)
        return nullptr;

    WinData* pData = nullptr;
    QPoint pt = QCursor::pos();

#ifdef _WIN32
    POINT pos;
    ::GetCursorPos(&pos);
    pt = QPoint(pos.x, pos.y);
#elif __APPLE__
#elif __linux__
#else
#endif

    if (m_pWinList) {
        m_pWinList->setWinIdFilter(hWnd);
        pData = m_pWinList->getWinInfoFromPoint(pt, bPrevCache);
    }

    return pData;
}

const QRect WinInfo::targWinRect(WinID hWnd, bool bPrevCache)
{
    const WinData* pDate = targWinInfo(hWnd, bPrevCache);

    if (pDate)
        return pDate->rect;
    else
        return QRect();
}

void WinInfo::winInfoAllCache()
{
}

void WinInfo::winInfoRealTime()
{
}

WinInfo::WinInfo()
    : m_bGetAllCache(false)
    , m_pWinList(nullptr)
{
#ifdef _WIN32
    m_pWinList = new WinInfo_Win();
#elif __APPLE__
#elif __linux__
    m_pWinList = new WinInfo_x11();
#else
#endif
}
