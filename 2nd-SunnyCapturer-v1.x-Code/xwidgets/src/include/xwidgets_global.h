#ifndef XWIDGETS_GLOBAL_H
#define XWIDGETS_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QDebug>

#if defined(XWIDGETS_LIBRARY)
#  define XWIDGETS_EXPORT Q_DECL_EXPORT
#else
#  define XWIDGETS_EXPORT Q_DECL_IMPORT
#endif

#endif // XWIDGETS_GLOBAL_H
