#ifndef NTWINDOWSRECT_H
#define NTWINDOWSRECT_H

#include <Windows.h>
#include <windef.h>
#include <winuser.h>
#include <iostream>
#include <string>
#include <vector>
#include "windowsrect_global.h"



RECT  xrect2rect(const XRECT& rt);
XRECT rect2xrect(const RECT& rt);

void enumWindowsRectInfo(std::vector<RectNode> &rectNodes, const POINT& pos);

#endif // NTWINDOWSRECT_H
