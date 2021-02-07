#ifndef QTEXTITEM_H
#define QTEXTITEM_H

#include "qabstractstylesheetitem.h"
#include "qgradientitem.h"

class SHAREDLIB_EXPORT QTextItem : public QAbstractStylesheetItem
{
Q_OBJECT
public:
    QTextItem(QAbstractStylesheetItem *parent=NULL);

    void setValue(const QVariant &value);
    QString makeStylesheet();
	
protected:
    void subValueChanged();
	
protected:
    QGradientItem *m_color;
};

#endif // QTEXTITEM_H
