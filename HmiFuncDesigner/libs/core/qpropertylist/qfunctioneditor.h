#ifndef QFUNCTIONEDITOR_H
#define QFUNCTIONEDITOR_H

#include "qbuttoncommoneditor.h"

class QFunctionEditor : public QButtonCommonEditor
{
    Q_OBJECT
public:
    Q_INVOKABLE QFunctionEditor(QAbstractProperty* m_property, QUndoStack* stack, QWidget *parent = 0);

protected:
    void onBtnClicked();

};

#endif // QFONTEDITOR_H
