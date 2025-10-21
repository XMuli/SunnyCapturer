#include "x11windowsrect.h"
#include <locale> // 添加这行
#include <codecvt>
#include <stdint.h>

X11WindowsRect::X11WindowsRect()
    : m_display(XOpenDisplay(nullptr))
{
}

X11WindowsRect::~X11WindowsRect()
{
    if (m_display) XCloseDisplay(m_display);
    m_display = nullptr;
}

void X11WindowsRect::enumWindowsRectInfo(std::vector<RectNode> &rectNodes, const Point& pos)
{
    rectNodes.clear();

    std::list<Window> list;
    windowsHWndToList(list);
    for (auto hwnd : list) {    //获取窗口属性
        XWindowAttributes attrs;  // 调试发现 attrs.x和 attrs.y [some windows]都是 0
        XGetWindowAttributes(m_display, hwnd, &attrs);    //XGetImage获取XImage，并通过转换得到QPixmap

        // some bug: 只截屏显示在桌面的部分，截去在屏幕外的部分
        //        int width = DisplayWidth(m_pDisplay, 0);
        //        int height = DisplayHeight(m_pDisplay, 0);
        //        int x1 = attrs.x;
        //        int y1 = attrs.y;
        //        int x2 = attrs.width + x1;
        //        int y2 = attrs.height + y1;
        //        if (x1 < 0)
        //            x1 = 0;
        //        if (y1 < 0)
        //            y1 = 0;
        //        if (x2 > width)
        //            x2 = width;
        //        if (y2 > height)
        //            y2 = height;

        // 获取当前窗口在屏幕上的坐标
        Window child;
        int x, y;
        XTranslateCoordinates(m_display, hwnd, attrs.root, 0, 0, &x, &y,&child);

        RectNode node;
        node.rect.left = x;
        node.rect.top = y;
        node.rect.width = attrs.width;
        node.rect.height = attrs.height;
        node.rect.right = x + attrs.width;
        node.rect.bottom = y + attrs.height;
        node.x11HWnd = hwnd;

        // string to wstring
        std::string szTitle = windowAtom(hwnd, "_NET_WM_NAME"); // get windows title name text
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::wstring wszTitle = converter.from_bytes(szTitle);
        node.title = wszTitle;


        if (node.title != L"Sunny" && node.rect.left <= pos.x &&  pos.x <= node.rect.right && node.rect.top <= pos.y &&  pos.y <= node.rect.bottom) {
            rectNodes.push_back(node);
        }

    }
}

Point X11WindowsRect::currPos()
{
    Point pos;
    if (m_display == nullptr) {
        std::wcout << L"m_display is nullptr!";
        return pos;
    }

    Window root = DefaultRootWindow(m_display); // 获取根窗口
    int rootX, rootY, winX, winY;
    unsigned int mask;
    Window child;

    // 获取鼠标的当前位置坐标（相对于根窗口）
    if (XQueryPointer(m_display, root, &root, &child, &rootX, &rootY, &winX, &winY, &mask)) {
        pos.x = rootX;
        pos.y = rootY;

        std::wcout << L"鼠标相对于根窗口的位置 m_pos(" << pos.x << L"," << pos.y << L")";
    } else {
        std::wcout << L"get m_pos failed!";
    }

    return pos;
}

void X11WindowsRect::windowsHWndToList(std::list<Window> &list)
{
    if (!isSupportQuery()) {
        std::wcout << L"Unable to query window list because window manager does not support extended window manager Hints";
        return;
    }

    Atom actualType = 0;
    int format = 0;
    unsigned long num = 0;
    unsigned long bytes = 0;
    Window *data = 0;
    Atom netClList = XInternAtom(m_display, "_NET_CLIENT_LIST", true);
    list.clear();

    for (int i = 0; i < ScreenCount(m_display); ++i) {
        Window rootWin = RootWindow(m_display, i);
        int status = XGetWindowProperty(m_display, rootWin, netClList, 0L, ~0L, false, AnyPropertyType, &actualType, &format, &num, &bytes, (uint8_t **)&data);

        if (status != Success) {
            std::wcout << L"Failed getting root window properties";
            continue;
        }

        for (unsigned long i = 0; i < num; ++i) list.push_back(data[i]);
        XFree(data);
    }
}

std::string X11WindowsRect::windowAtom(Window hwnd, const char *atom)
{
    XTextProperty tp;
    Atom netWmName = XInternAtom(m_display, atom, false);
    XGetTextProperty(m_display, hwnd, &tp, netWmName);

    if (!tp.nitems) {
        XGetWMName(m_display, hwnd, &tp);
        return "error";
    }

    std::string title = "empty";

    if (tp.encoding == XA_STRING) {
        title = (char *)tp.value;
    } else {
        int n = 0;
        char **list = 0;
        int ret = XmbTextPropertyToTextList(m_display, &tp, &list, &n);

        if (ret >= Success && n > 0 && *list) {
            title = *list;
            XFreeStringList(list);
        }
    }

    XFree(tp.value);
    return title;
}

/*!
 * \brief X11WindowsRect::isSupportQuery
 * \return true-支持NetWM协议的窗口; fase- not support
 * \note: 检查X Window System环境中是否支持"NetWM"（Net Window Manager）协议中的 _NET_SUPPORTING_WM_CHECK 属性。NetWM协议是一种用于窗口管理器（Window Manager）与客户端应用程序之间进行通信的协议，它定义了一些属性和消息，以便管理窗口的外观和行为。
 */
bool X11WindowsRect::isSupportQuery()
{
    Atom netSupportingWmCheck = XInternAtom(m_display, "_NET_SUPPORTING_WM_CHECK", true);
    Atom actualType = 0;
    int format = 0;
    unsigned long num = 0;
    unsigned long bytes = 0;
    unsigned char *data = nullptr;
    int status = XGetWindowProperty(m_display, DefaultRootWindow(m_display), netSupportingWmCheck, 0L, 1L, false , XA_WINDOW, &actualType, &format, &num, &bytes, &data);

    Window hWnd = 0;
    if (status == Success) {
        if (num > 0)
            hWnd = ((Window *)data)[0];

        if (data) {
            XFree(data);
            data = nullptr;
        }
    }

    if (hWnd) {
        status = XGetWindowProperty(m_display, hWnd, netSupportingWmCheck, 0L, 1L, false , XA_WINDOW, &actualType, &format, &num, &bytes, &data);
        if (status != Success || num == 0 || hWnd != ((Window *)data)[0])
            hWnd = 0;
        if (status == Success && data)
            XFree(data);
    }

    return hWnd != 0;
}


