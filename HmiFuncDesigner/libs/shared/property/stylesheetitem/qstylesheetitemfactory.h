#ifndef QSTYLESHEETITEMFACTORY_H
#define QSTYLESHEETITEMFACTORY_H

#include "../../sharedlibglobal.h"
#include <QString>

class QAbstractStylesheetItem;

class SHAREDLIB_EXPORT QStylesheetItemFactory
{
public:
    static QAbstractStylesheetItem* createItem(const QString &name);
};

#endif // QSTYLESHEETITEMFACTORY_H
