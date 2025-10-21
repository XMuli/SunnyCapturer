#include "ntwindowsrect.h"
#include <psapi.h>

static std::vector<RectNode> g_rectNodes;
static HWND                  g_filterSelfHwnd;   //需要过滤掉自身体的句柄

BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam)
{
    POINT pt;
    pt.x = ((int)(short)LOWORD(lParam));
    pt.y = ((int)(short)HIWORD(lParam));

    RectNode node;
    wchar_t title[MAX_PATH] = L"";
    GetWindowText(hwnd, title, MAX_PATH);
    node.title = title;
    node.ntHWnd = hwnd;

    GetWindowThreadProcessId(hwnd, &node.ntPocessId);
    node.procPath = windowPathFromProcessID(node.ntPocessId);
    node.exeName = windowExeName(node.procPath);

    RECT rect;
    // DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(RECT)); // [方法一，推荐]获取真实的边框矩形，不包含阴影
    GetWindowRect(hwnd, &rect);  // 子窗口遍历，改动点
    node.rect = rect2xrect(rect);

    if (IsWindowVisible(hwnd) // && !filterSpecialWindows(hwnd, node)    // 子窗口遍历，改动点
        && PtInRect(&rect, pt) && g_filterSelfHwnd != hwnd) { // 仅仅选中当前的 pt 的所在窗口
        g_rectNodes.push_back(node);
        EnumChildWindows(hwnd, EnumChildWindowsProc, lParam);
        return FALSE;
    }

    return TRUE;
}

// MSDN !!!: EnumWindows continues until the last top-level window is enumerated or the callback function returns FALSE.
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    POINT pt;
    pt.x = ((int)(short)LOWORD(lParam));
    pt.y = ((int)(short)HIWORD(lParam));

    RectNode node;
    wchar_t title[MAX_PATH] = L"";
    GetWindowText(hwnd, title, MAX_PATH);
    node.title = title;
    node.ntHWnd = hwnd;

    // 调试-卡住断点
    // size_t found1 = node.title.find(L"Notepad++");
    // if (found1 != std::wstring::npos)
    //     int a = 1;

    // if (hwnd == HWND(0x310e58))
    //     int b = 2;

    // size_t found2 = node.procPath.find(L"SystemSettings.exe");
    // if (found2 != std::wstring::npos)
    //     int a = 2;

    GetWindowThreadProcessId(hwnd, &node.ntPocessId);
    node.procPath = windowPathFromProcessID(node.ntPocessId);
    node.exeName = windowExeName(node.procPath);

    RECT rect;
    #if defined(_MSC_VER)
    // [方法一，推荐]获取真实的边框矩形，不包含阴影
        DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(RECT));
    #elif defined(__MINGW32__)
    // [方法二] 获取窗口矩形尺寸（包括标题栏+四周阴影）
        GetWindowRect(hwnd, &rect);
    #endif
    node.rect = rect2xrect(rect);

    // [方法二] 获取客户区域坐标（不含标题栏+四周阴影）
    // RECT clientRect;
    // GetClientRect(hwnd, &clientRect);
    // int titleBarHeight = GetSystemMetrics(SM_CYCAPTION); // 获取标题栏的高度

    if (IsWindowVisible(hwnd) && !filterSpecialWindows(hwnd, node)  // 这一行必须保留，和下面组成 且
        && PtInRect(&rect, pt) && g_filterSelfHwnd != hwnd) { // 仅仅选中当前的 pt 的所在窗口
        g_rectNodes.push_back(node);

        // const int x = rect.left;
        // const int y = rect.top;
        // const int width = rect.right - rect.left;
        // const int height = rect.bottom - rect.top;
        // int idx = 0;
        // std::wcout << L"--->idx:" << idx++ << L"  rect(" << x << L", " << y << L", " << width << L" * " << height << L")"
        //            << L" hwnd[" << hwnd << L"] title:[" << title << L"]"
        //            << L" node.ntPocessId[" << node.ntPocessId << L" procPath[" << node.procPath << L"] exeName:[" << node.exeName << L"]" << std::endl;


        // [另外一个方法，已经尝试失败]获取光标当前位置, https://ningto.com/post/edit/1D18B9EAD804A0EA0E0DAFA392FD8756
        // POINT relativePt = pt;
        // ScreenToClient(hwnd, &relativePt);  // 转化为相对坐标
        // HWND childHwnd1 = ChildWindowFromPointEx(hwnd, relativePt, CWP_SKIPINVISIBLE|CWP_SKIPTRANSPARENT);  // 获取下一个子窗口
        // HWND childHwnd2 = RealChildWindowFromPoint(hwnd, relativePt);  // 尝试另外一个;

        EnumChildWindows(hwnd, EnumChildWindowsProc, lParam);
        return FALSE;    // "找到了指定的篡改口"只需命中一次
    }

    return TRUE;  // 非所寻找的"顶级窗口", 所以继续遍历
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

void enumWindowsRectInfo(std::vector<RectNode> &rectNodes, const POINT &pos, const CrossHwnd &selfHwnd)
{
    g_rectNodes.clear();
    g_filterSelfHwnd = HWND(selfHwnd.ntHWnd);
    EnumWindows(EnumWindowsProc, MAKELPARAM(pos.x, pos.y));
    rectNodes = g_rectNodes;
}

// 还有一种更简单的方法，代码很简单，我后来没有用这种方法的原因是，某些区域上面的方法能框选出来而这种方法不行，不知道问题出在哪里？？？
bool getSmallestWindowFromCursor(const CrossHwnd &selfHwnd)
{
    HWND hwnd = NULL;
    POINT pt;

    while (true) {
        // 获得当前鼠标位置
        ::GetCursorPos(&pt);
        hwnd = ::ChildWindowFromPointEx(::GetDesktopWindow(), pt, CWP_SKIPDISABLED | CWP_SKIPINVISIBLE);  /// while 不能解决当前问题， hwnd 还是下一个窗口

        // 如果找到的窗口是 Sunny 窗口，或者是要过滤的自身窗口，则继续寻找下一个窗口
        if (hwnd == selfHwnd.ntHWnd || hwnd == reinterpret_cast<HWND>(0x001506FA))
            continue;

        break;
    }

    // 如果找到了合适的窗口，则进行后续操作
    if (hwnd) {
        HWND temp_hwnd = hwnd;
        while (true) {
            ::GetCursorPos(&pt); // 更新鼠标位置
            ::ScreenToClient(temp_hwnd, &pt);
            temp_hwnd = ::ChildWindowFromPointEx(temp_hwnd, pt, CWP_SKIPINVISIBLE);
            if (temp_hwnd == NULL || temp_hwnd == hwnd) {
                break;
            }
            hwnd = temp_hwnd;
        }
        RECT r;
        ::GetWindowRect(hwnd, &r);

        RectNode node;
        node.rect = rect2xrect(r);
        node.ntHWnd = hwnd;
        g_rectNodes.push_back(node);

        return true;
    }
    return false;
}

void getWindowsRectInfo(std::vector<RectNode> &rectNodes, const POINT &pos, const CrossHwnd &selfHwnd)
{
    g_rectNodes.clear();
    // EnumWindows(EnumWindowsRealTimeProc, MAKELPARAM(pos.x, pos.y));
    getSmallestWindowFromCursor(selfHwnd);
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

std::wstring windowPathFromProcessID(DWORD processId)
{
#if 0
    // 获取形式: [\Device\HarddiskVolume3\Qt\Tools\QtCreator\bin\qtcreator.exe]
    // wchar_t path[MAX_PATH] = L"";
    // HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, processId);
    // GetProcessImageFileName(hProc, path, MAX_PATH);
    // CloseHandle(hProc);
    // return std::wstring(path);
#else
    // 获取形式: C:\Qt\Tools\QtCreator\bin\qtcreator.exe
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (hProcess != NULL) {
        wchar_t exePath[MAX_PATH];
        DWORD bufferSize = MAX_PATH;

        if (GetModuleFileNameExW(hProcess, NULL, exePath, bufferSize) != 0) {
            CloseHandle(hProcess);
            return std::wstring(exePath);
        }
        CloseHandle(hProcess);
    }
    return L"";

#endif
}

std::wstring windowExeName(std::wstring path)
{
    auto pos = path.rfind(L'/');
    if (pos == -1) pos = path.rfind(L'\\');
    if (pos == -1) return L"";

    return path.substr(pos + 1);
}

bool isTaskbarWindow(HWND hWnd)
{
    LONG_PTR style = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
    return !(style & WS_EX_APPWINDOW);
}

bool isWindowMinimized(HWND hWnd)
{
    // return IsIconic(hWnd);

    WINDOWPLACEMENT placement;
    GetWindowPlacement(hWnd, &placement);
    return placement.showCmd == SW_SHOWMINIMIZED;
}

bool isWindowMaximized(HWND hWnd) {
    // return IsZoomed(hWnd);

    WINDOWPLACEMENT placement;
    GetWindowPlacement(hWnd, &placement);
    return placement.showCmd == SW_SHOWMAXIMIZED;
}


bool filterSpecialWindows(HWND hWnd, const RectNode& node)
{
    // [特例1] systemsettings.exe 即 Windows 10 22H2 的 "设置"窗口; 其处于最小化时候，遍历窗口函数可以捕捉到这个实际不可见的窗口，在屏幕左上角区域
    // 可以通过 Spy 软件来捕捉和确认， 左上的"显示系统活动窗口 - 任务栏窗口 - 找名字为 `Windows.UI.Core.CoreWindow` 的项", 通过底部的句柄箭头跳转即可查看详情
    // 获取窗口的类名
    wchar_t className[MAX_PATH] = L"";
    GetClassName(hWnd, className, MAX_PATH);
    std::wstring str = className;
    bool bClassName = (str == L"Windows.UI.Core.CoreWindow");
    const bool& isSystemsettings = isTaskbarWindow(hWnd) && bClassName;

    // [特例2] ApplicationFrameHost.exe, 会出现在屏幕的左下角
    const bool& isSystemApplicationFrameHost = node.exeName == std::wstring(L"ApplicationFrameHost.exe");

    return isSystemsettings || isSystemApplicationFrameHost;
}


