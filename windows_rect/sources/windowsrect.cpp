#include "windowsrect.h"


#ifdef _WIN32
static HHOOK g_hook = nullptr;

// 处理鼠标事件的回调函数
LRESULT CALLBACK CallBackProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    switch (wParam) {
        case WM_LBUTTONDOWN: {
            POINT pos;
            bool ret = GetCursorPos(&pos);
            if(ret) std::wcout << pos.x << L" " << pos.y;

            std::wcout << L"鼠标左键按下" << std::endl;
            break;
        } default: {
            break;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);   // 注意这一行一定不能少，否则会出大问题
}
#elif linux

#elif __APPLE__ || __MACH__
#endif


static std::vector<RectNode> g_rectNodes;


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
    std::wcout << L"hwnd:" << hwnd << L"  pos:(" << pos.x << L", " << pos.x << L")" << std::endl;
    if(!hwnd) return;

    RECT rect;
    bool ret = GetWindowRect(hwnd, &rect);     //获取窗口位置

    wchar_t windowText[MAX_PATH] = L"";
    GetWindowText(hwnd, windowText, MAX_PATH);

    m_rectNode.ntHWnd = hwnd;
    m_rectNode.title = windowText;
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

#ifdef _WIN32
bool WindowsRect::startWindowsHook()
{
    g_hook = SetWindowsHookExW(WH_MOUSE_LL, CallBackProc, GetModuleHandleW(nullptr), 0);  // 挂载全局鼠标钩子
    g_hook ? std::wcout <<  L"鼠标钩子挂接成功,线程ID:" << GetCurrentThreadId() << std::endl :  std::wcout <<  L"鼠标钩子挂接失败,error:" << GetLastError() << std::endl;
    return g_hook ? true : false;
}

bool WindowsRect::endWindowsHook()
{
    if (g_hook) {
        bool ret = UnhookWindowsHookEx(g_hook);
        ret ? std::wcout <<  L"卸载鼠标钩子,线程ID:" << GetCurrentThreadId() << std::endl :  std::wcout <<  L"卸载鼠标钩子失败,error:" << GetLastError() << std::endl;

        return ret;
    }

    std::wcout <<  L"g_hook is nullptr" << std::endl;
    return false;
}
#endif

void RectNode::printf()
{
    std::wcout << L"---------------------------printf Start-------------------------------" << std::endl;
    std::wcout << L"rect(" << rect.left << L", " << rect.top << L", " << rect.right - rect.left << L" * " << rect.bottom - rect.top << L")" << std::endl;
    std::wcout << L"title:[" << title << L"]\n notes:[" << notes << L"]" << std::endl;
    std::wcout << L"ntHWnd:[" << ntHWnd << L"]\n x11HWnd:[" << x11HWnd << L"]" << std::endl;
    std::wcout << L"---------------------------printf End-------------------------------" << std::endl << std::endl;

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

        if (node.title != L"Sunny") {
            static int i = 1;
            g_rectNodes.push_back(node);
            std::wcout << L"--->i" << i++ << L"  rect(" << x << L", " << y << L", " << width << L" * " << height << L")"
                       << L" hwnd[" << hwnd << L"] windowText:[" << windowText << L"]" << std::endl;
        }
    }
    return TRUE;
}


BOOL EnumChildWindowsProc(HWND hwnd, LPARAM lParam)
{
    return TRUE;
}

const RectNode enumWindowsRect(std::vector<RectNode>& rectNodes)
{
    POINT pos;
    GetCursorPos(&pos);

    std::wcout << L"--->pos(" << pos.x << L", " << pos.y << L")" << std::endl;
    g_rectNodes.clear();
    EnumWindows(EnumWindowsProc, MAKELPARAM(pos.x, pos.y));

    rectNodes = g_rectNodes;

    RectNode rectNode;
//    if (g_rectNodes.size()) {
//        rectNode = g_rectNodes.at(0);
//        g_rectNodes.clear();
//    }

    return rectNode;
}
