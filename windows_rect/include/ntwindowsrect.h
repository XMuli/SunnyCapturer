#ifndef NTWINDOWSRECT_H
#define NTWINDOWSRECT_H

#include <Windows.h>
#include <windef.h>
#include <winuser.h>
#include <iostream>
#include <string>
#include <vector>
#include "windowsrect_global.h"

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

//class NtWindowsRect
//{
//public:
//    NtWindowsRect();
//};


//#ifdef _WIN64
//BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
//#else
//BOOL CALLBACK EnumWindowsProc32(HWND hwnd, LPARAM lParam);
//#endif



extern "C" WINDOWSRECT_EXPORT bool enumWindowsRect(std::vector<RectNode>& rectNodes);

#endif // NTWINDOWSRECT_H
