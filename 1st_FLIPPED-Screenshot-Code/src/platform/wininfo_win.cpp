// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "wininfo_win.h"
#include <psapi.h>
#include <string>
#include <iostream>

#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#else
#define tcout std::cout
#endif

HWND WinInfo_Win::m_hWndTarget = NULL;
int WinInfo_Win::m_level = 0;
int WinInfo_Win::m_curIndex = 0;
int WinInfo_Win::m_totalIndex = 0;

WinInfo_Win::WinInfo_Win()
{
}

WinInfo_Win::~WinInfo_Win()
{
}

void WinInfo_Win::getAllWinInfoCache()
{
    m_vWinData.clear();
    m_hWndTarget = nullptr;
    ::EnumWindows(WinInfo_Win::EnumWindowsProc, m_level); // 0 为 z 序最顶层的

//    int i = 0;
//    setlocale(LC_ALL, "");
//    for (auto it = IWinInfo::m_vWinData.cbegin(); it != IWinInfo::m_vWinData.cend(); ++it) {
//        //std::string str = CT2A(it->procPath.GetString());
//        tcout << i++ << _T("   ") << it->hWnd
//            << _T("   ") << it->index
//            << _T("   ") << it->path
//            << _T("   ") << it->name
//            << _T("   ") << it->title
//            << _T("   (") << it->rect.left << _T(", ") << it->rect.top << _T(", ")
//            << it->rect.right - it->rect.left << _T(" * ") << it->rect.bottom - it->rect.top << _T(")")
//            << std::endl;
//    }
}

HWND WinInfo_Win::getAllWinInfoRealTime(POINT pt)
{ 
    m_vWinData.clear();
    m_hWndTarget = nullptr;
    m_level = 0;
    m_curIndex = 0;
    m_totalIndex = 0;
    ::EnumWindows(WinInfo_Win::EnumRealTimeWindowsProc, MAKELPARAM(pt.x, pt.y));

//    int i = 0;
//    setlocale(LC_ALL, "");
//    for (auto it = IWinInfo::m_vWinData.cbegin(); it != IWinInfo::m_vWinData.cend(); ++it) {
//        //std::string str = CT2A(it->procPath.GetString());
//        tcout << i++ << _T("   ") << it->hWnd
//            << _T("   ") << it->index
//            << _T("   ") << it->path
//            << _T("   ") << it->name
//            << _T("   ") << it->title
//            << _T("   (") << it->rect.left << _T(", ") << it->rect.top << _T(", ")
//            << it->rect.right - it->rect.left << _T(" * ") << it->rect.bottom - it->rect.top << _T(")")
//            << std::endl;
//    }

    // 其中第一个为 Z 序最上层;
    if (m_vWinData.size())
        m_hWndTarget = static_cast<HWND>(m_vWinData.at(0).id._hWnd);  // m_vWinData.size() -1
        //m_hWndTarget = static_cast<HWND>(m_vWinData.at(m_vWinData.size() - 1).id._hWnd);  // m_vWinData.size() -1

//    for (const auto& it : m_vWinData) {
//        HWND hwnd1 = ::GetWindow((HWND)it.hWnd, GW_HWNDFIRST);
//        HWND hwnd2 = ::GetTopWindow((HWND)it.hWnd);
//		HWND hwnd3 = ::GetTopWindow(nullptr);
//        HWND hwnd4 = ::GetNextWindow((HWND)it.hWnd, GW_HWNDFIRST);
//        HWND hwnd5 = ::GetNextWindow((HWND)it.hWnd, GW_HWNDLAST);
//        std::wcout << L"it.hWnd:" << (HWND)it.hWnd
//			<< L"  " << hwnd1 << L"  " << hwnd2
//			<< L"  " << hwnd3 << L"  " << hwnd4
//			<< L"  " << hwnd5 << std::endl;
//	}
    return m_hWndTarget;
}

// 设置窗口过滤
void WinInfo_Win::setWinIdFilter(WinID target /*= nullptr*/)
{
    m_vWinIdFilter.push_back(target);
}

WinData* WinInfo_Win::getWinInfoFromPoint(QPoint pt, bool bPrevCache /*= false*/)
{
    POINT pos;
    pos.x = pt.x();
    pos.y = pt.y();

    HWND hWndTarget = nullptr;
    if (bPrevCache) {
        getAllWinInfoCache();
        hWndTarget = getWinInfoFromCache(pos);
    } else {
        hWndTarget = getAllWinInfoRealTime(pos);
    }

    for (WinData& it : m_vWinData) {
        if (hWndTarget && hWndTarget == (HWND)(it.id._hWnd))
            return &it;
    }

    return nullptr;
}

BOOL WinInfo_Win::EnumRealTimeWindowsProc(HWND hWnd, LPARAM lParam)
{
	POINT pos;
    pos.x = ((int)(short)LOWORD(lParam));
    pos.y = ((int)(short)HIWORD(lParam));

	if (WindowsContainsPoint(hWnd, pos) && WindowsFilter(hWnd)) {
        RECT rt;
		DWORD processId;
        wchar_t windowTitle[MAX_PATH] = L"";
        ::GetWindowRect(hWnd, &rt);
		::GetWindowThreadProcessId(hWnd, &processId);
		::GetWindowText(hWnd, windowTitle, MAX_PATH);
        std::wstring procPath = getWindowPath(processId);
        std::wstring procName = windowPath2Name(procPath);

        m_hWndTarget = hWnd;
        WinID winId;
        winId._hWnd = (void *)hWnd;
        m_vWinData.push_back(WinData(winId
                                     , false
                                     , QRect(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top)
                                     , QString::fromStdWString(procPath)
                                     , QString::fromStdWString(procName)
                                     , QString::fromWCharArray(windowTitle)
                                     , ""
                                     , m_level
                                     , m_curIndex++
                                     , m_totalIndex++));

        m_curIndex = 0;
        m_level++;
		EnumChildWindows(hWnd, EnumChildRealTimeWindowsProc, lParam);
		return FALSE; // 只需命中一次
	}

	return TRUE;
}

BOOL WinInfo_Win::EnumChildRealTimeWindowsProc(HWND hWnd, LPARAM lParam)
{
	POINT pos;
	pos.x = ((int)(short)LOWORD(lParam));
	pos.y = ((int)(short)HIWORD(lParam));

	if (WindowsContainsPoint(hWnd, pos) && WindowsFilter(hWnd)) {
        RECT rt;
		DWORD processId;
        
        wchar_t windowTitle[MAX_PATH] = L"";
        ::GetWindowRect(hWnd, &rt);
		::GetWindowThreadProcessId(hWnd, &processId);
		::GetWindowText(hWnd, windowTitle, MAX_PATH);
        std::wstring procPath = getWindowPath(processId);
        std::wstring procName = windowPath2Name(procPath);

        m_hWndTarget = hWnd;
        WinID winId;
        winId._hWnd = (void *)hWnd;
        m_vWinData.push_back(WinData(winId
                                     , false
                                     , QRect(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top)
                                     , QString::fromStdWString(procPath)
                                     , QString::fromStdWString(procName)
                                     , QString::fromWCharArray(windowTitle)
                                     , ""
                                     , m_level
                                     , m_curIndex++
                                     , m_totalIndex++));
        m_curIndex = 0;
        m_level++;
		EnumChildWindows(hWnd, EnumChildRealTimeWindowsProc, lParam);
		return FALSE;
	}

	return TRUE;
}

BOOL WinInfo_Win::EnumChildWindowsProc(HWND hWnd, LPARAM lParam)
{
    if (hWnd && WindowsFilter(hWnd)) {
        RECT rt;
        DWORD processId;
        m_level++;
        m_curIndex = 0;
        wchar_t windowTitle[MAX_PATH] = L"";
        ::GetWindowRect(hWnd, &rt);
        ::GetWindowThreadProcessId(hWnd, &processId);
        ::GetWindowText(hWnd, windowTitle, MAX_PATH);
        std::wstring procPath = getWindowPath(processId);
        std::wstring procName = windowPath2Name(procPath);

        m_hWndTarget = hWnd;
        WinID winId;
        winId._hWnd = (void*)hWnd;
        m_vWinData.push_back(WinData(winId
            , false
            , QRect(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top)
            , QString::fromStdWString(procPath)
            , QString::fromStdWString(procName)
            , QString::fromWCharArray(windowTitle)
            , ""
            , m_level
            , m_curIndex++
            , m_totalIndex++));

        //EnumChildWindows(hWnd, EnumChildRealTimeWindowsProc, lParam);
    }

    return TRUE;
}

// MSDN !!!: EnumWindows continues until the last top-level window is enumerated or the callback function returns FALSE.
BOOL WinInfo_Win::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	if (hWnd && WindowsFilter(hWnd)) {  // 句柄存在且可见
        RECT rt;
		DWORD processId;
        wchar_t windowTitle[MAX_PATH] = L"";
        ::GetWindowRect(hWnd, &rt);
		::GetWindowThreadProcessId(hWnd, &processId);
		::GetWindowText(hWnd, windowTitle, MAX_PATH);
        std::wstring procPath = getWindowPath(processId);
        std::wstring procName = windowPath2Name(procPath);

        WinID winId;
        winId._hWnd = (void *)hWnd;
        m_vWinData.push_back(WinData(winId
                                     , false
                                     , QRect(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top)
                                     , QString::fromStdWString(procPath)
                                     , QString::fromStdWString(procName)
                                     , QString::fromWCharArray(windowTitle)
                                     , ""
                                     , m_level
                                     , m_curIndex++
                                     , m_totalIndex++));

        //if (!EnumChildWindows(hWnd, EnumChildWindowsProc, lParam))
        //    m_level++;
        // 关于等级看怎么设置一下
	}

	return TRUE;
}


BOOL WinInfo_Win::WindowsContainsPoint(HWND hWnd, POINT pt)
{
    RECT rtWin = { 0, 0, 0, 0};
	::GetWindowRect(hWnd, &rtWin);
	return PtInRect(&rtWin, pt);
}

HWND WinInfo_Win::getWinInfoFromCache(POINT pt)
{
    WinData winData;
    for (const auto& it : m_vWinData) {
        RECT rt;
        rt.left = it.rect.left();
        rt.top = it.rect.top();
        rt.right = it.rect.right();
        rt.bottom = it.rect.bottom();

        if (PtInRect(&rt, pt)) {
            if (winData.totalIndex <= it.totalIndex)
                winData = it;
		}
	}

    return (HWND)winData.id._hWnd;
}

BOOL WinInfo_Win::WindowsFilter(HWND hWnd)
{
    if (!hWnd)
        return FALSE;

    for (auto& it : m_vWinIdFilter) {
        if (hWnd == static_cast<HWND>(it._hWnd))
            return FALSE;
    }

    RECT rect;
    ::GetWindowRect(hWnd, &rect);

    if (rect.left == -32000 && rect.top == -32000   // 最小化
		|| rect.bottom - rect.top <= 0 || rect.right - rect.left <= 0)     // 宽度不存在
        return FALSE;

    // https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
    // https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
    DWORD styles = ::GetWindowLong(hWnd, GWL_STYLE);
    DWORD ex_styles = ::GetWindowLong(hWnd, GWL_EXSTYLE);
    if ((!(styles & WS_VISIBLE)) | ex_styles & WS_EX_TRANSPARENT | ex_styles & WS_EX_NOREDIRECTIONBITMAP)  // spy++ 抓的 不可见窗口类型
        return FALSE;

    return ::IsWindowVisible(hWnd);
}

std::wstring WinInfo_Win::getWindowPath(DWORD processId)
{
    wchar_t path[MAX_PATH] = L"";
    HANDLE hProc = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, processId);

#ifdef Q_CC_MINGW
    //pit, mingw does not support
#elif Q_CC_MSVC
    GetProcessImageFileName(hProc, path, MAX_PATH);
#endif

    ::CloseHandle(hProc);
    return std::wstring(path);
}

std::wstring WinInfo_Win::windowPath2Name(std::wstring path)
{
    auto pos = path.rfind(L'/');
    if (pos == -1)
        pos = path.rfind(L'\\');
    if (pos == -1)
        return L"";

    return path.substr(pos + 1);
}
