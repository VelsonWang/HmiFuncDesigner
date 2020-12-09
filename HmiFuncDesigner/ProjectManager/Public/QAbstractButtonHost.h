#ifndef QABSTRACTBUTTONHOST_H
#define QABSTRACTBUTTONHOST_H

#include "QWidgetHost.h"

class QAbstractButtonHost :public QWidgetHost
{
    Q_OBJECT
public:
    QAbstractButtonHost(QAbstractHost *parent = Q_NULLPTR);

public slots:
    void    setText(const QString & text);
    QString text();

    void    setCheckable(bool checkable);
    bool    checkable();

    void    setChecked(bool checked);
    bool    checked();

protected slots:
    void clickedSlot();

};
#endif // QABSTRACTBUTTONHOST_H
