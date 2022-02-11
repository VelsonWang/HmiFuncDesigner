#ifndef CORELIBGLOBAL_H
#define CORELIBGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CORE_LIBRARY)
#  define CORELIB_EXPORT Q_DECL_EXPORT
#else
#  define CORELIB_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBGLOBAL_H
