#ifndef QABSTRACTRESOURCEEDITOR_H
#define QABSTRACTRESOURCEEDITOR_H

#include "corelibglobal.h"
#include <QWidget>

struct tagFileInfo;

class CORELIB_EXPORT QAbstractResourceEditor : public QWidget
{
    Q_OBJECT
public:
    explicit QAbstractResourceEditor(QWidget *parent = 0);

    virtual QWidget* toolBar() = 0;
    virtual QString showName() = 0;
    virtual bool support(const QString &type) = 0;
    virtual void setFile(tagFileInfo *info) = 0;

};

#endif // QABSTRACTRESOURCEEDITOR_H
