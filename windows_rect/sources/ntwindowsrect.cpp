#include "ntwindowsrect.h"
#include <psapi.h>




static std::vector<RectNode> g_rectNodes;

// 自定义的一些过滤
bool WindowsRectFilter(HWND hwnd)
{
    return false;
}


BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam)
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
        GetWindowThreadProcessId(hwnd, &node.ntPocessId);
        node.procPath = windowPathFromProcessID(node.ntPocessId);
        node.exeName = windowExeName(node.procPath);
        node.rect = rect2xrect(rect);

        if (PtInRect(&rect, pos) && node.title != L"Sunny") { // 仅仅选中当前的 pos 的所在窗口
            g_rectNodes.push_back(node);
            EnumChildWindows(hwnd, EnumChildWindowsProc, lParam);
            return FALSE;
        }
    }
    return TRUE;
}

// MSDN !!!: EnumWindows continues until the last top-level window is enumerated or the callback function returns FALSE.
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    POINT pos;
    pos.x = ((int)(short)LOWORD(lParam));
    pos.y = ((int)(short)HIWORD(lParam));

    RectNode node;
    RECT rect;

    wchar_t windowTextt[MAX_PATH] = L"";
    GetWindowText(hwnd, windowTextt, MAX_PATH);
    node.title = windowTextt;

    GetWindowThreadProcessId(hwnd, &node.ntPocessId);
    node.procPath = windowPathFromProcessID(node.ntPocessId);

    // 调试-卡住断点
    size_t found1 = node.title.find(L"Setting");
    if (found1 != std::wstring::npos) {
        int a = 1;
    }


    size_t found2 = node.procPath.find(L"SystemSettings.exe");
    if (found2 != std::wstring::npos) {
        int a = 2;
    }



    // 检查窗口是否处于最小化状态
    bool b1 = isWindowMinimized(hwnd);
    bool b2 = isWindowMaximized(hwnd);
    bool b3 = isTaskbarWindow(hwnd);




    if (IsWindowVisible(hwnd) && !isSystemsettingsMinAndTaskbar(hwnd)) {
        wchar_t windowText[MAX_PATH] = L"";

        GetWindowText(hwnd, windowText, MAX_PATH);
        node.title = windowText;

        // 调试-卡住断点
        std::wstring searchString = L"Setting";
        size_t found = node.title.find(searchString);
        if (found != std::wstring::npos) {
            int a = 1;
        }

        // [方法一，推荐]获取真实的边框矩形，不包含阴影
        DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(RECT));

        // [方法二] 获取窗口矩形尺寸（包括标题栏+四周阴影）
        // GetWindowRect(hwnd, &rect);

        // [方法二] 获取客户区域坐标（不含标题栏+四周阴影）
        // RECT clientRect;
        // GetClientRect(hwnd, &clientRect);
        // int titleBarHeight = GetSystemMetrics(SM_CYCAPTION); // 获取标题栏的高度

        // // 将客户区域坐标转换为屏幕坐标
        // POINT clientTopLeft = {clientRect.left, clientRect.top};
        // POINT clientBottomRight = {clientRect.right, clientRect.bottom};
        // ClientToScreen(hwnd, &clientTopLeft);
        // ClientToScreen(hwnd, &clientBottomRight);

        // rect.left = clientTopLeft.x;
        // rect.top = clientTopLeft.y;
        // rect.right = clientBottomRight.x;
        // rect.bottom = clientBottomRight.y;

        GetWindowThreadProcessId(hwnd, &node.ntPocessId);
        node.procPath = windowPathFromProcessID(node.ntPocessId);

        node.exeName = windowExeName(node.procPath);
        node.rect = rect2xrect(rect);

        // const int x = rect.left;
        // const int y = rect.top;
        // const int width = rect.right - rect.left;
        // const int height = rect.bottom - rect.top;
        // int idx = 0;
        if (PtInRect(&rect, pos) && node.title != L"Sunny") { // 仅仅选中当前的 pos 的所在窗口
            node.ntHWnd = hwnd;
            g_rectNodes.push_back(node);
//            std::wcout << L"--->idx:" << idx++ << L"  rect(" << x << L", " << y << L", " << width << L" * " << height << L")"
//                       << L" hwnd[" << hwnd << L"] windowText:[" << windowText << L"]"
//                       << L" node.ntPocessId[" << node.ntPocessId << L" procPath[" << node.procPath << L"] exeName:[" << node.exeName << L"]" << std::endl;

            EnumChildWindows(hwnd, EnumChildWindowsProc, lParam);
            return FALSE; // 只需命中一次
        }
    }
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
//    std::wcout << L"--->pos(" << pos.x << L", " << pos.y << L")" << std::endl;
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

int GetWindowShadowWidth(HWND hwnd)
{
    RECT rcExtendedFrameBounds;
    HRESULT hr = DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rcExtendedFrameBounds, sizeof(RECT));
    if (SUCCEEDED(hr)) {
        // 计算阴影宽度
        int shadowWidth = rcExtendedFrameBounds.right - rcExtendedFrameBounds.left - (GetSystemMetrics(SM_CXSIZEFRAME) * 2);
        return shadowWidth;
    } else {
        // 获取失败
        return -1;
    }
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


// [特例]systemsettings.exe 即 Windows 10 22H2 的 "设置"窗口; 其处于最小化时候，遍历窗口函数可以捕捉到这个实际不可见的窗口，在屏幕左上角区域
// 可以通过 Spy 软件来捕捉和确认， 左上的"显示系统活动窗口 - 任务栏窗口 - 找名字为 `Windows.UI.Core.CoreWindow` 的项", 通过底部的句柄箭头跳转即可查看详情
bool isSystemsettingsMinAndTaskbar(HWND hWnd)
{
    // 获取窗口的类名
    wchar_t className[MAX_PATH] = L"";
    GetClassName(hWnd, className, MAX_PATH);
    std::wstring str = className;
    bool bClassName = (str == L"Windows.UI.Core.CoreWindow");
    bool b1 = isTaskbarWindow(hWnd);
    return isTaskbarWindow(hWnd) && bClassName;
}
