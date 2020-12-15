#ifndef PROJDATA_GLOBAL_H
#define PROJDATA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PROJDATA_LIBRARY)
#  define CORELIB_EXPORT Q_DECL_EXPORT
#else
#  define CORELIB_EXPORT Q_DECL_IMPORT
#endif

#endif // PROJDATA_GLOBAL_H
