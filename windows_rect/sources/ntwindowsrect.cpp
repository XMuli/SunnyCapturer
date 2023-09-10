#include "ntwindowsrect.h"

static std::vector<RectNode> g_rectNodes;

//NtWindowsRect::NtWindowsRect()
//{

//}


void RectNode::printf()
{
    std::wcout << L"---------------------------printf Start-------------------------------" << std::endl;
    std::wcout << L"rect(" << rect.left << L", " << rect.top << L", " << rect.right - rect.left << L" * " << rect.bottom - rect.top << L")" << std::endl;
    std::wcout << L"title:[" << title << L"]\n notes:[" << notes << L"]" << std::endl;
    std::wcout << L"ntHWnd:[" << ntHWnd << L"]\n x11HWnd:[" << x11HWnd << L"]" << std::endl;
    std::wcout << L"---------------------------printf End-------------------------------" << std::endl << std::endl;
}


// 自定义的一些过滤
bool WindowsRectFilter(HWND hwnd)
{
    return false;
}

BOOL EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    POINT pos;
    pos.x = ((int)(short)LOWORD(lParam));
    pos.y = ((int)(short)HIWORD(lParam));

    RectNode node;
    RECT& rect = node.rect;

    if (IsWindowVisible(hwnd)) {
        wchar_t windowText[MAX_PATH] = L"";
        GetWindowText(hwnd, windowText, MAX_PATH);
        node.title = windowText;

        GetWindowRect(hwnd, &rect);
        const int x = rect.left;
        const int y = rect.top;
        const int width = rect.right - rect.left;
        const int height = rect.bottom - rect.top;

        int idx = 0;
        if (PtInRect(&rect, pos) && node.title != L"Sunny") { // 仅仅选中当前的 pos 的所在窗口
            g_rectNodes.push_back(node);
            std::wcout << L"--->idx:" << idx++ << L"  rect(" << x << L", " << y << L", " << width << L" * " << height << L")"
                       << L" hwnd[" << hwnd << L"] windowText:[" << windowText << L"]" << std::endl;
        }
    }
    return TRUE;
}


BOOL EnumChildWindowsProc(HWND hwnd, LPARAM lParam)
{
    return TRUE;
}

bool enumWindowsRect(std::vector<RectNode>& rectNodes)
{
    POINT pos;
    GetCursorPos(&pos);

    std::wcout << L"--->pos(" << pos.x << L", " << pos.y << L")" << std::endl;
    rectNodes = g_rectNodes;
    g_rectNodes.clear();
    EnumWindows(EnumWindowsProc, MAKELPARAM(pos.x, pos.y));

    return g_rectNodes.size() ? true : false;
}
