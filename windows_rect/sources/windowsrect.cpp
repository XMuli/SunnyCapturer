#include "windowsrect.h"

WindowsRect::WindowsRect()
{
}

WindowsRect::~WindowsRect()
{

}

// 检测窗口光标坐在位置的窗口矩形
void WindowsRect::detectionWindowsRect()
{
#ifdef _WIN32
    POINT pos;
    GetCursorPos(&pos);

    HWND hwnd = nullptr;
    hwnd = WindowFromPoint(pos);                // 通过鼠标位置获取窗口句柄
    wcout << L"hwnd:" << hwnd << L"  pos:(" << pos.x << L", " << pos.x << L")" << endl;
    if(!hwnd) return;

    RECT rect;
    bool ret = GetWindowRect(hwnd, &rect);     //获取窗口位置
    if(!ret) return;

    m_rectNode.ntHWnd = hwnd;
    m_rectNode.rect = rect;

#elif linux

#elif __APPLE__ || __MACH__

#endif

    m_rectNode.printf();
}

RectNode WindowsRect::rectNode() const
{
    return m_rectNode;
}

void RectNode::printf()
{
    wcout << L"rect(" << rect.left << L", " << rect.top << L", " << rect.right - rect.left << L" * " << rect.bottom - rect.top << L")" << endl;
    wcout << L"title:[" << title << L"]\n notes:[" << notes << L"]" << endl;
    wcout << L"ntHWnd:[" << ntHWnd << L"]\n x11HWnd:[" << x11HWnd << L"]" << endl;
}
