#ifndef RUNNINGLIBGLOBAL_H
#define RUNNINGLIBGLOBAL_H

#include <QtCore/qglobal.h>


#if defined(RUNNING_LIBRARY)
#  define RUNNINGLIB_EXPORT Q_DECL_EXPORT
#else
#  define RUNNINGLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBGLOBAL_H
