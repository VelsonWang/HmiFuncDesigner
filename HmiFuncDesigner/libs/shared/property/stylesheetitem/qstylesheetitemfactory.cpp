#include "qstylesheetitemfactory.h"
#include "qbackgrounditem.h"
#include "qborderitem.h"
#include "qtextitem.h"

QAbstractStylesheetItem* QStylesheetItemFactory::createItem(const QString &name)
{
    QAbstractStylesheetItem* ret = NULL;

    if(name == "background") {
        ret = new QBackgroundItem;
    } else if(name == "border") {
        ret = new QBorderItem;
    } else if(name == "text_sheet") {
        ret = new QTextItem;
    }

    return ret;
}
