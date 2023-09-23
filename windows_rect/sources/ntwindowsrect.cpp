#include "ntwindowsrect.h"

static std::vector<RectNode> g_rectNodes;


// 自定义的一些过滤
bool WindowsRectFilter(HWND hwnd)
{
    return false;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    POINT pos;
    pos.x = ((int)(short)LOWORD(lParam));
    pos.y = ((int)(short)HIWORD(lParam));

    RectNode node;
    RECT rect;

    if (IsWindowVisible(hwnd)) {
        wchar_t windowText[MAX_PATH] = L"";
        GetWindowText(hwnd, windowText, MAX_PATH);
        node.title = windowText;

        GetWindowRect(hwnd, &rect);

        node.rect = rect2xrect(rect);

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


BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam)
{
    return TRUE;
}

RECT xrect2rect(const XRECT &rt)
{
    RECT rect;
    rect.left = rt.left;
    rect.top = rt.top;
    rect.right = rt.right;
    rect.bottom = rt.bottom;

    return rect;
}

void enumWindowsRectInfo(std::vector<RectNode> &rectNodes, const POINT &pos)
{
    std::wcout << L"--->pos(" << pos.x << L", " << pos.y << L")" << std::endl;
    g_rectNodes.clear();
    EnumWindows(EnumWindowsProc, MAKELPARAM(pos.x, pos.y));
    rectNodes = g_rectNodes;
}

XRECT rect2xrect(const RECT &rt)
{
    XRECT xrect;
    xrect.left = rt.left;
    xrect.top = rt.top;
    xrect.right = rt.right;
    xrect.bottom = rt.bottom;
    xrect.width = rt.right - rt.left;
    xrect.height = rt.bottom - rt.top;

    return xrect;
}
