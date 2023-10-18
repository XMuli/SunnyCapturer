#include "windowsrect.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#include "ntwindowsrect.h"
#elif __linux__
#include "x11windowsrect.h"
#elif __APPLE__ || __MACH__
#endif



bool enumWindowsRect(std::vector<RectNode> &rectNodes)
{
    rectNodes.clear();

#ifdef _WIN32

    POINT pos;
    GetCursorPos(&pos);
    enumWindowsRectInfo(rectNodes, pos);

#elif __linux__

    std::wcout << L" test";

    X11WindowsRect x11WinRects;
    const Point& pt = x11WinRects.currPos();
    x11WinRects.enumWindowsRectInfo(rectNodes, pt);

#elif __APPLE__ || __MACH__
#endif

    return rectNodes.size() ? true : false;
}
