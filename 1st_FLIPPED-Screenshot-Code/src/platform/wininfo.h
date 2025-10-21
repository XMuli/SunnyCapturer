// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef WININFO_H
#define WININFO_H

#include "../core/isingleton.h"
#include "iwininfo.h"

class WinInfo : public ISingleton<WinInfo>
{
    FRIEND_CREAT_SINGLETION(WinInfo);

public:
    const WinData*  targWinInfo(WinID hWnd, bool bPrevCache);
    const QRect targWinRect(WinID hWnd, bool bPrevCache = false);

private:
    void winInfoAllCache();
    void winInfoRealTime();

private:
    WinInfo();
    virtual ~WinInfo() = default;

private:
    bool m_bGetAllCache;
    IWinInfo* m_pWinList;
};

#endif // WININFO_H
