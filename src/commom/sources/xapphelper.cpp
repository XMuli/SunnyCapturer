#include "xapphelper.h"

#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QtGlobal>
#include <QDebug>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>


bool ensureSingleInstance(const QString &uniqueKey)
{
    QSharedMemory sharedMemory;
    sharedMemory.setKey(uniqueKey);

    // 尝试创建共享内存，如果已经存在，表示已经有一个实例在运行, 判断是为了确保在同一台计算机上只能运行一个相同实例的程序。
    if (!sharedMemory.create(1)) {
        qDebug() << "There is already an instance of the application running (by QSharedMemory)!";
        return false;
    }



    // 创建系统信号量 -> 尝试获取系统信号量，如果已经被其他实例持有，程序就退出, 判断是为了确保在多个进程同时启动时，只有一个进程能够继续执行。QSystemSemaphore用于创建系统信号量，如果系统信号量已经被其他实例持有（比如由于上一次程序异常退出导致信号量未被释放），则acquire函数会返回false，
    QSystemSemaphore systemSemaphore(uniqueKey, 1, QSystemSemaphore::Open);
    if (!systemSemaphore.acquire()) {
        qDebug() << "There is already an instance of the application running (by QSystemSemaphore)!";
        return false;
    }

    // 如果程序成功获取了共享内存和系统信号量，表示可以继续运行
    return true;
}

void releaseSystemSemaphore(const QString &uniqueKey)
{
    // 释放系统信号量
    QSystemSemaphore systemSemaphore(uniqueKey, 1, QSystemSemaphore::Open);
    systemSemaphore.release();
}


#if defined(Q_OS_WIN)

void GetExceptionDescription(DWORD errCode, QString &err)
{
    //    errCode = 0xc0000005;
    LPTSTR lpMsgBuf = NULL;
    HMODULE Hand = LoadLibrary(TEXT("ntdll.dll"));
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS/*FORMAT_MESSAGE_FROM_SYSTEM*/| FORMAT_MESSAGE_FROM_HMODULE,
                  Hand,
                  errCode,
                  MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
                  (LPTSTR)&lpMsgBuf,
                  0,NULL);
    err = QString::fromWCharArray( lpMsgBuf );
    qDebug() << "err:" << err;
    LocalFree(lpMsgBuf);
}

// 程式异常捕获-保存数据代码-创建 Dump 文件
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    const QString& createDir = QString("%1/%2_Dumps")
                             .arg(QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first())
                             .arg(XPROJECT_NAME);
    QDir dir;
    dir.mkpath(createDir);
    const QString& createPath = QString("%1/dump_%2.dmp").arg(createDir).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz"));
    qCritical() << "Trigger an exception! createPath:" << createPath;

    std::wstring wlpstr = createPath.toStdWString();
    LPCWSTR lpcwStr = wlpstr.c_str();

    HANDLE hDumpFile = CreateFile(lpcwStr,
                                  GENERIC_WRITE,
                                  0,
                                  NULL,
                                  CREATE_ALWAYS,
                                  FILE_ATTRIBUTE_NORMAL,
                                  NULL);

    if (hDumpFile != INVALID_HANDLE_VALUE){
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = FALSE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    }

    // 这里弹出一个错误对话框并退出程序
    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode,16)),errAdr(QString::number((UINT)((UINT_PTR)record->ExceptionAddress),16));;
    QString errstr;
    GetExceptionDescription(record->ExceptionCode,errstr);
    if(record->NumberParameters>0){
        if(record->ExceptionInformation[0]==0){
            errstr+="\n访问冲突,线程试图读取不可访问的数据";
        }else if(record->ExceptionInformation[0]==1){
            errstr+="\n访问冲突,线程尝试写入不可访问的地址";
        }
    }

    QString msg = QString("Run Crash! \n\nError code: %1 \nError address: %2 \nDescribe: %3 \n\nDump path: %4")
                      .arg(errCode).arg(errAdr).arg(errstr).arg(createPath);
    QMessageBox::StandardButton ret = QMessageBox::critical(nullptr, QObject::tr("CRASH"), msg, QMessageBox::Open | QMessageBox::Close);

    if (ret == QMessageBox::Open && dir.exists()) QDesktopServices::openUrl(QUrl::fromLocalFile(createDir));
    return EXCEPTION_EXECUTE_HANDLER;
}

#endif

