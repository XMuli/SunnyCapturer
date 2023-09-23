#ifndef WINDOWSRECT_H
#define WINDOWSRECT_H
#include <vector>
#include "windowsrect_global.h"

extern "C" WINDOWSRECT_EXPORT bool enumWindowsRect(std::vector<RectNode>& rectNodes);

#endif // WINDOWSRECT_H
