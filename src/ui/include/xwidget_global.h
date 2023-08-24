#ifndef XWIDGET_GLOBAL_H
#define XWIDGET_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QDebug>

#if defined(XWIDGET_LIBRARY)
#  define XWIDGET_EXPORT Q_DECL_EXPORT
#else
#  define XWIDGET_EXPORT Q_DECL_IMPORT
#endif

#endif // XWIDGET_GLOBAL_H
