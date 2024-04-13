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

#if 0
    // 另外一个思路: WindowFromPoint, ChildWindowFromPoint, ChildWindowFromPointEx(最有可能得函数),RealChildWindowFromPoint
    // 另外，有些截图软件能够识别到比窗口粒度更小的元素，比如Toolbar控件上的每个Item，他们用的应该是MSAA(Microsoft Active Accessibility),标准控件一般都支持该接口。
    // https://www.cnblogs.com/findumars/p/5786055.html
    // getWindowsRectInfo(rectNodes, pos);
#endif

#elif __linux__

    std::wcout << L" test";

    X11WindowsRect x11WinRects;
    const Point& pt = x11WinRects.currPos();
    x11WinRects.enumWindowsRectInfo(rectNodes, pt);

#elif __APPLE__ || __MACH__
#endif

    return rectNodes.size() ? true : false;
}
