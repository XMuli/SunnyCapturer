#ifndef WINDOWSRECT_H
#define WINDOWSRECT_H

#include "windowsrect_global.h"
#include <iostream>
#include <string>

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
    RECT rect;
    std::wstring title;
    std::wstring notes; // 备注

    HWND ntHWnd;            // NT OS
    unsigned long x11HWnd;  // Linux OS

    void printf();
};

class WINDOWSRECT_EXPORT WindowsRect
{
public:
    WindowsRect();
    ~WindowsRect();

    void detectionWindowsRect();
    RectNode rectNode() const;

#ifdef _WIN32
    bool startWindowsHook();
    bool endWindowsHook();
#endif


private:
    RectNode m_rectNode;
};

#endif // WINDOWSRECT_H
