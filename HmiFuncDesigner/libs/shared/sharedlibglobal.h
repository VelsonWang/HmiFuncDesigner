#ifndef SHAREDLIBGLOBAL_H
#define SHAREDLIBGLOBAL_H

#include <QtCore/qglobal.h>

/*
 *sharedLib是一个大家都需要加载的动态库
 *对于一些设计和运行都需要使用的到函数或类可以放到这个库中
 *使用SHAREDLIB命名空间
 */

#define DEFAULT_PLUGIN_NAME     QString("_None")
#define DEFAULT_PROPERTY_NAME   QString("None")


#if defined(SHARED_LIBRARY)
#  define SHAREDLIB_EXPORT Q_DECL_EXPORT
#else
#  define SHAREDLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBGLOBAL_H
