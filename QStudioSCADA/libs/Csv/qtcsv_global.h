#ifndef QTCSV_GLOBAL_H
#define QTCSV_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTCSV_LIBRARY)
#  define QTCSVSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTCSVSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTCSV_GLOBAL_H
