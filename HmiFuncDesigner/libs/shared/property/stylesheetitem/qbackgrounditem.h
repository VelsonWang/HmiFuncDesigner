#ifndef QBACKGROUNDITEM_H
#define QBACKGROUNDITEM_H

#include "../../sharedlibglobal.h"

#include "qabstractstylesheetitem.h"

class QGradientItem;

class QBackgroundItem : public QAbstractStylesheetItem
{
    Q_OBJECT
public:
    QBackgroundItem(QAbstractStylesheetItem *parent=NULL);

    void setValue(const QVariant &value);

    QString makeStylesheet();
	
protected:
    void subValueChanged();
	
protected:
    QGradientItem *m_color;
    QAbstractStylesheetItem *m_image;
    QAbstractStylesheetItem *m_repeat;
    QAbstractStylesheetItem *m_position;
};
#endif // QBACKGROUNDITEM_H
