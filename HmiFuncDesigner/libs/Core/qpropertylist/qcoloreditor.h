#ifndef QCOLOREDITOR_H
#define QCOLOREDITOR_H

#include "qbuttoncommoneditor.h"

class QColorEditor : public QButtonCommonEditor
{
    Q_OBJECT
public:
    Q_INVOKABLE QColorEditor(QAbstractProperty* property, QUndoStack* stack, QWidget *parent = 0);

protected:
    void onBtnClicked();

};

#endif
