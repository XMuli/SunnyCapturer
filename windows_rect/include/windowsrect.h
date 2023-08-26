#ifndef WINDOWSRECT_H
#define WINDOWSRECT_H

#include "windowsrect_global.h"
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
    #include <Windows.h>

    #include <windef.h>
    #include <winuser.h>
#elif linux
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
#elif __APPLE__ || __MACH__
// 在 macOS 平台下的代码
#else
// 在其他平台下的代码
#endif

//union RectHWnd
//{
//    void* ntHWnd;              // NT OS
//    unsigned long x11HWnd;     // Linux OS
//};

struct RectNode
{
    RECT rect;             // 显示器坐标
    RECT relativelyRect;   // 相对窗口坐标
    std::wstring title;
    std::wstring notes; // 备注

    unsigned long x11HWnd;  // Linux OS

    HWND  ntHWnd;            // NT OS
    DWORD ntPocessId;

    void printf();
};

class WINDOWSRECT_EXPORT WindowsRect
{
public:
    WindowsRect();
    ~WindowsRect();

// 思路1-------------------
    void detectionWindowsRect();
    RectNode rectNode() const;

#ifdef _WIN32
    bool startWindowsHook();
    bool endWindowsHook();
#endif
// 思路2-------------------



// end-------------------

private:
    RectNode m_rectNode;
};


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam);

//WINDOWSRECT_EXPORT
extern "C" __declspec(dllexport)  const RectNode enumWindowsRect(std::vector<RectNode>& rectNodes);

#endif // WINDOWSRECT_H
