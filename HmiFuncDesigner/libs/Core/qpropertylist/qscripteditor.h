#ifndef QSCRIPTEDITOR_H
#define QSCRIPTEDITOR_H

#include "qbuttoncommoneditor.h"

class QScriptEditor : public QButtonCommonEditor
{
    Q_OBJECT
public:
    Q_INVOKABLE QScriptEditor(QAbstractProperty* property,
                              QUndoStack* stack,
                              QWidget *parent = 0);

protected:
    void onBtnClicked();

protected:
    QUndoStack *undoStack;
};

#endif // QSCRIPTEDITOR_H
