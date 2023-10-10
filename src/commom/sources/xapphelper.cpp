#include "xapphelper.h"

#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QDebug>


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
