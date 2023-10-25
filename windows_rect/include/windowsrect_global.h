#ifndef WINDOWSRECT_GLOBAL_H
#define WINDOWSRECT_GLOBAL_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(WINDOWSRECT_LIBRARY)
#  define WINDOWSRECT_EXPORT Q_DECL_EXPORT
#else
#  define WINDOWSRECT_EXPORT Q_DECL_IMPORT
#endif

#include <iostream>
#include <string>



struct XRECT
{
    long    left;
    long    top;
    long    width;
    long    height;
    long    right;
    long    bottom;
};


struct RectNode
{
    XRECT rect;                   // 显示器坐标
    XRECT relativelyRect;         // 相对窗口坐标
    std::wstring title;
    std::wstring procPath;        // [\Device\HarddiskVolume3\Qt\Tools\QtCreator\bin\qtcreator.exe]
    std::wstring procPathDevice;  // C:\Qt\Tools\QtCreator\bin\qtcreator.exe
    std::wstring exeName;
    std::wstring notes;           // 备注

    void* ntHWnd;               // NT OS: HWND
    unsigned long ntPocessId;   // NT OS: DWORD
    unsigned long x11HWnd;      // Linux OS; typedef XID Window;

    void printf()
    {
        std::wcout << L"---------------------------printf Start-------------------------------" << std::endl;
        std::wcout << L"rect(" << rect.left << L", " << rect.top << L", " << rect.right - rect.left << L" * " << rect.bottom - rect.top << L")" << std::endl;
        std::wcout << L"title:[" << title << L"]\n notes:[" << notes << L"]" << std::endl;
        std::wcout << L"ntHWnd:[" << ntHWnd << L"] ntPocessId:[" << ntPocessId << L"]" << std::endl;
        std::wcout << L"x11HWnd:[" << x11HWnd << L"]" << std::endl;
        std::wcout << L"---------------------------printf End-------------------------------" << std::endl << std::endl;
    }
};


#endif // WINDOWSRECT_GLOBAL_H
