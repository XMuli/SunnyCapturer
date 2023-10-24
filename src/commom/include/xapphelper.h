#ifndef XAPPHELPER_H
#define XAPPHELPER_H

#include <QString>

constexpr const char* szUniqueKey = "SunnyUniqueKey"; // 使用唯一的标识符来创建共享内存和系统信号量
bool ensureSingleInstance(const QString &uniqueKey = "SunnyUniqueKey");
void releaseSystemSemaphore(const QString &uniqueKey = "SunnyUniqueKey");


#ifdef _MSC_VER  // 在 MSVC 编译器下的代码
    #include <Windows.h>
    #include <DbgHelp.h>
    #include <string>
    #include <QDebug>
    #include <QDir>
    #include <QDateTime>
    #include <QMessageBox>
    #include <QCoreApplication>

    #pragma comment(lib, "user32.lib")
    #pragma comment(lib, "dbghelp.lib")

    void GetExceptionDescription(DWORD errCode,QString& err);
    LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException);
#endif


#endif // XAPPHELPER_H
