#ifndef QABSTRACTBUTTONHOST_H
#define QABSTRACTBUTTONHOST_H

#include "qwidgethost.h"

class QAbstractButtonHost : public QWidgetHost
{
    Q_OBJECT
public:
    QAbstractButtonHost(QAbstractHost *parent = 0);

public slots:
    void setText(const QString & text);
    QString text();

    void setCheckable(bool checkable);
    bool checkable();

    void setChecked(bool checked);
    bool checked();

protected slots:
    void clicked_slot();

protected:
    void initProperty() override;

};
#endif // QABSTRACTBUTTONHOST_H
