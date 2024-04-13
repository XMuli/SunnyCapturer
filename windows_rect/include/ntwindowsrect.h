#ifndef NTWINDOWSRECT_H
#define NTWINDOWSRECT_H

#include <Windows.h>
#include <windef.h>
#include <winuser.h>
#include <dwmapi.h>
#include <string>
#include <vector>
#include "windowsrect_global.h"

#pragma comment(lib, "dwmapi.lib")


RECT  xrect2rect(const XRECT& rt);
XRECT rect2xrect(const RECT& rt);
std::wstring windowPathFromProcessID(DWORD processId);
std::wstring windowExeName(std::wstring path);

void enumWindowsRectInfo(std::vector<RectNode> &rectNodes, const POINT& pos);


bool isTaskbarWindow(HWND hWnd);
bool isWindowMinimized(HWND hWnd);
bool isWindowMaximized(HWND hWnd);
bool isSystemsettingsMinAndTaskbar(HWND hWnd);  // [特例] 屏蔽 systemsettings.exe 即 Windows 10 22H2 的 "设置"窗口最小化时候

#endif // NTWINDOWSRECT_H
