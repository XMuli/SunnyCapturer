#ifndef XAPPHELPER_H
#define XAPPHELPER_H


#include <QString>
constexpr const char* szUniqueKey = "SunnyUniqueKey"; // 使用唯一的标识符来创建共享内存和系统信号量

bool ensureSingleInstance(const QString &uniqueKey = "SunnyUniqueKey");
void releaseSystemSemaphore(const QString &uniqueKey = "SunnyUniqueKey");

#endif // XAPPHELPER_H
