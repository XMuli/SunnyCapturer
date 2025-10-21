/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.05.11
 * Description: Linux 系统下智能窗口识别
 ******************************************************************/
#include "../core/xlog.h"  // !!! NOTE:Qt headers must be placed in front of X11 headers
#include "wininfo_x11.h"
#include <QString>
#include <QLabel>

const QSize IMAGE_SIZE(400, 300);
const QSize ITEM_SIZE(400, 300);

void WinInfo_x11::setWinIdFilter(WinID target)
{
    m_vWinIdFilter.push_back(target);
}

WinData* WinInfo_x11::getWinInfoFromPoint(QPoint pt, bool bPrevCache)
{
    getAllWinInfo();

//    if (bPrevCache) {
//        getAllWinInfoCache();
//        hWndTarget = getWinInfoFromCache(pos);
//    } else {
//        hWndTarget = getAllWinInfoRealTime(pos);
//    }

    for (WinData& it : m_vWinData) {
        const QRect rt(it.rect);

        if (rt.contains(pt) && !it.bFilter) {
            return &it;
        }
    }

    return nullptr;
}

WinInfo_x11::WinInfo_x11()
    : IWinInfo()
    , m_pDisplay(XOpenDisplay(nullptr))
{

}

WinInfo_x11::~WinInfo_x11()
{
    if (m_pDisplay)
        XCloseDisplay(m_pDisplay);

    m_pDisplay = nullptr;
}

void WinInfo_x11::getWinIdList(std::list<Window>& vec)
{
    if (!bSupportQuery()) {
        XLOG_ERROR("Unable to query window list because window manager does not support extended window manager Hints");
        return;
    }

    Atom actualType = 0;
    int format = 0;
    unsigned long num = 0;
    unsigned long bytes = 0;
    Window *data = 0;
    Atom netClList = XInternAtom(m_pDisplay, "_NET_CLIENT_LIST", true);
    vec.clear();

    for (int i = 0; i < ScreenCount(m_pDisplay); ++i) {
        Window rootWin = RootWindow(m_pDisplay, i);

        int status = XGetWindowProperty(m_pDisplay, rootWin, netClList, 0L, ~0L, false
                                        , AnyPropertyType, &actualType, &format, &num, &bytes, (uint8_t **)&data);

        if (status != Success) {
            XLOG_ERROR("Failed getting root window properties");
            continue;
        }

        for (unsigned long i = 0; i < num; ++i)
            vec.push_back(data[i]);

        XFree(data);
    }
}


void WinInfo_x11::getAllWinInfo()
{
    m_vWinData.clear();
    int index = 0;

    std::list<Window> vec;
    getWinIdList(vec);
    for (auto win : vec) {    //获取窗口属性
        XWindowAttributes attrs;  // 调试发现 attrs.x和 attrs.y [some windows]都是 0
        XGetWindowAttributes(m_pDisplay, win, &attrs);    //XGetImage获取XImage，并通过转换得到QPixmap

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
        XTranslateCoordinates(m_pDisplay, win, attrs.root, 0, 0, &x, &y,&child);

        WinID winId;
        winId._xWindow = win;
        m_vWinData.push_back(WinData(winId
                                     , false
                                     , QRect(x, y, attrs.width, attrs.height)
                                     , ""
                                     , ""
                                     , QString(getWindowAtom(winId._xWindow, "_NET_WM_NAME").c_str())
                                     , ""
                                     , 0
                                     , index++
                                     , 0));

    }

    // 获取整个屏幕图像并显示到QListWidget中
    //获取屏幕Window属性使用RootWindow函数获取
    Window rootwin = RootWindow(m_pDisplay, 0);
    XWindowAttributes attrs;
    XGetWindowAttributes(m_pDisplay, rootwin, &attrs);
    WinID winId;
    winId._xWindow = rootwin;
    m_vWinData.push_back(WinData(winId
                                 , false
                                 , QRect(attrs.x, attrs.y, attrs.width, attrs.height)
                                 , ""
                                 , ""
                                 , QString(getWindowAtom(winId._xWindow, "_NET_WM_NAME").c_str())
                                 , ""
                                 , 0
                                 , index++
                                 , 0));


    for (const auto& it : m_vWinData) {
        if (it.title.compare(QString("PicShot"))  == 0) {
            m_vWinIdFilter.push_back(it.id);
            break;
        }
    }

    // delete fliter winInfo
    for (auto& itData : m_vWinData) {
        for (const auto& itFilter : m_vWinIdFilter) {
            if (itData.id._xWindow == itFilter._xWindow)
                itData.bFilter = true;
        }
    }

//    XImage * pImage = XGetImage(m_pDisplay, rootwin, 0, 0, attrs.width, attrs.height, AllPlanes, ZPixmap);
//    QImage image = QImage((const uchar *)(pImage->data), pImage->width, pImage->height, pImage->bytes_per_line, QImage::Format_RGB32);
//    QPixmap pixmap = QPixmap::fromImage(image);
//    QListWidgetItem *listWidgetItemScreen = new QListWidgetItem(QIcon(pixmap.scaled(IMAGE_SIZE)), "Screen");
//    listWidgetItemScreen->setSizeHint(ITEM_SIZE);
//    m_listWidget->insertItem(index++, listWidgetItemScreen);

}

bool WinInfo_x11::bSupportQuery()
{
    Atom netSupportingWmCheck = XInternAtom(m_pDisplay, "_NET_SUPPORTING_WM_CHECK", true);

    Atom actualType = 0;
    int format = 0;
    unsigned long num = 0;
    unsigned long bytes = 0;
    unsigned char *data = nullptr;
    int status = XGetWindowProperty(m_pDisplay, DefaultRootWindow(m_pDisplay), netSupportingWmCheck, 0L, 1L, false
                                    , XA_WINDOW, &actualType, &format, &num, &bytes, &data);

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
        status = XGetWindowProperty(m_pDisplay, hWnd, netSupportingWmCheck, 0L, 1L, false
                                    , XA_WINDOW, &actualType, &format, &num, &bytes, &data);

        if (status != Success || num == 0 || hWnd != ((Window *)data)[0])
            hWnd = 0;

        if (status == Success && data)
            XFree(data);
    }

    return hWnd != 0;
}

std::string WinInfo_x11::getWindowAtom(Window win, const char *atom)
{
    XTextProperty tp;
    Atom netWmName = XInternAtom(m_pDisplay, atom, false);
    XGetTextProperty(m_pDisplay, win, &tp, netWmName);

    if (!tp.nitems) {
        XGetWMName(m_pDisplay, win, &tp);
        return "error";
    }

    std::string m_listWin = "unknown";

    if (tp.encoding == XA_STRING) {
        m_listWin = (char *)tp.value;
    } else {
        int n = 0;
        char **list = 0;
        int ret = XmbTextPropertyToTextList(m_pDisplay, &tp, &list, &n);

        if (ret >= Success && n > 0 && *list) {
            m_listWin = *list;
            XFreeStringList(list);
        }
    }

    XFree(tp.value);

    return m_listWin;
}

std::string WinInfo_x11::getWindowName(Window win)
{
    return getWindowAtom(win, "_NET_WM_NAME");
}
