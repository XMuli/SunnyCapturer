#ifndef NTWINDOWSRECT_H
#define NTWINDOWSRECT_H

#include <Windows.h>
#include <windef.h>
#include <winuser.h>
#include <string>
#include <vector>
#include "windowsrect_global.h"

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

RECT  xrect2rect(const XRECT& rt);
XRECT rect2xrect(const RECT& rt);
std::wstring windowPathFromProcessID(DWORD processId);
std::wstring windowExeName(std::wstring path);


void enumWindowsRectInfo(std::vector<RectNode> &rectNodes, const POINT& pos);

#endif // NTWINDOWSRECT_H
