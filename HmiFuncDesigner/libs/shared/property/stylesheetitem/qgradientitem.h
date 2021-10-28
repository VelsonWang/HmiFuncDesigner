#ifndef QLINEARGRADIENTITEM_H
#define QLINEARGRADIENTITEM_H

#include "../../sharedlibglobal.h"
#include "qabstractstylesheetitem.h"
#include <QGradient>

class QGradientItem : public QAbstractStylesheetItem
{
    Q_OBJECT
public:
    QGradientItem(QAbstractStylesheetItem *parent = NULL);

    void read(XMLObject *xml);

    void setValue(const QVariant &value);
    void setDefault(const QVariant &value);

protected:
    void subValueChanged();

protected:
    void makeValue();

};

#endif // QLINEARGRADIENTITEM_H
