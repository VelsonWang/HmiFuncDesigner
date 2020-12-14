#ifndef CORELIBGLOBAL_H
#define CORELIBGLOBAL_H

#include <QtCore/qglobal.h>

/*
 *coreLib这个库是提供给设计程序使用的
 *使用SHAREDLIB命名空间
 */


#if defined(CORE_LIBRARY)
#  define CORELIB_EXPORT Q_DECL_EXPORT
#else
#  define CORELIB_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBGLOBAL_H
