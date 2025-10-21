#ifndef X11WINDOWSRECT_H
#define X11WINDOWSRECT_H

/***********************************************
1. 标准C++库头文件
2. Qt库头文件
3. X11库头文件
4. 自己编写的头文件
***********************************************/

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <string>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include "windowsrect_global.h"


struct Point {
    long x = 0;
    long y = 0;
};

class X11WindowsRect
{
public:
    X11WindowsRect();
    ~X11WindowsRect();
    Point currPos();
    void enumWindowsRectInfo(std::vector<RectNode> &rectNodes, const Point &pos);

private:

    bool isSupportQuery(); // 检查X Window System环境中是否支持"NetWM"（Net Window Manager）协议中的 _NET_SUPPORTING_WM_CHECK 属性。
    void windowsHWndToList(std::list<Window>& list);
    std::string windowAtom(Window hwnd, const char *atom);  // able get windows title by atom

private:
    Display* m_display;
};

#endif // X11WINDOWSRECT_H
