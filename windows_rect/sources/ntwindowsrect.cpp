#include "ntwindowsrect.h"
#include <psapi.h>

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
//        wchar_t procPathDevice[MAX_PATH] = L"";
        GetWindowText(hwnd, windowText, MAX_PATH);
        node.title = windowText;

        GetWindowRect(hwnd, &rect);
        GetWindowThreadProcessId(hwnd, &node.ntPocessId);
        node.procPath = windowPathFromProcessID(node.ntPocessId);

//        DWORD result = QueryDosDevice(node.procPath.c_str(), procPathDevice, MAX_PATH);
//        if (result != 0) { // 如果转换成功，targetPath 中将包含目标路径信息
//            std::wcout << L"Converted Path: " << procPathDevice << std::endl;
//        } else { // 转换失败
//            GetModuleFileName(NULL, procPathDevice, MAX_PATH);
//            std::wcerr << L"Error converting path. node.procPath.c_str():" << node.procPath.c_str() << L" procPathDevice:" << procPathDevice << L"  GetLastError():" << GetLastError() << std::endl;
//        }

//        node.procPathDevice = std::wstring(procPathDevice);
        node.exeName = windowExeName(node.procPath);
        node.rect = rect2xrect(rect);

        const int x = rect.left;
        const int y = rect.top;
        const int width = rect.right - rect.left;
        const int height = rect.bottom - rect.top;

        int idx = 0;
        if (PtInRect(&rect, pos) && node.title != L"Sunny") { // 仅仅选中当前的 pos 的所在窗口
            g_rectNodes.push_back(node);
            std::wcout << L"--->idx:" << idx++ << L"  rect(" << x << L", " << y << L", " << width << L" * " << height << L")"
                       << L" hwnd[" << hwnd << L"] windowText:[" << windowText << L"]"
                       << L" node.ntPocessId[" << node.ntPocessId << L" procPath[" << node.procPath << L"] exeName:[" << node.exeName << L"]" << std::endl;
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

std::wstring windowPathFromProcessID(DWORD processId)
{
    wchar_t path[MAX_PATH] = L"";
    HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, processId);
    GetProcessImageFileName(hProc, path, MAX_PATH);
    CloseHandle(hProc);
    return std::wstring(path);
}

std::wstring windowExeName(std::wstring path)
{
    auto pos = path.rfind(L'/');
    if (pos == -1) pos = path.rfind(L'\\');
    if (pos == -1) return L"";

    return path.substr(pos + 1);
}
