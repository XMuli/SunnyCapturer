// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef WININFO_WIN_H
#define WININFO_WIN_H
#include "iwininfo.h"

#include <windows.h>
#include <vector>

class WinInfo_Win : public IWinInfo
{
public:
    WinInfo_Win();
    virtual ~WinInfo_Win();

public:
    virtual void setWinIdFilter(WinID target) override;
    virtual WinData* getWinInfoFromPoint(QPoint pt, bool bPrevCache = false) override;

private:
    void getAllWinInfoCache();
    HWND getAllWinInfoRealTime(POINT pt);

public:
	static BOOL CALLBACK EnumRealTimeWindowsProc(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK EnumChildRealTimeWindowsProc(HWND hWnd, LPARAM lParam);

    static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
    static BOOL CALLBACK EnumChildWindowsProc(HWND hWnd, LPARAM lParam);

    static BOOL WindowsContainsPoint(HWND hWnd, POINT pt);
	static HWND getWinInfoFromCache(POINT pt);
    static BOOL WindowsFilter(HWND hWnd);
    static std::wstring getWindowPath(DWORD processId);
    static std::wstring windowPath2Name(std::wstring path);

    // error: LNK2005: "protected: static struct HWND__ * WinInfoWin::m_hWndTarget" (?m_hWndTarget@WinInfoWin@@1PAUHWND__@@A) already defined in mocs_compilation.cpp.obj
    // 静态成员变量只能在 cpp 中初始化，坑了一会 https://stackoverflow.com/questions/40991522
public:
    static HWND m_hWndTarget;
    static int m_level;
    static int m_curIndex;
    static int m_totalIndex;
};

#endif // WININFO_WIN_H
