#ifndef QSTRINGEDITOR_H
#define QSTRINGEDITOR_H

#include "qbuttoncommoneditor.h"

class QStringEditor : public QButtonCommonEditor
{
    Q_OBJECT
public:
    Q_INVOKABLE QStringEditor(QAbstractProperty* property, QUndoStack* stack, QWidget *parent = 0);

protected:
    void onBtnClicked();

protected:
    QUndoStack* undoStack;

};

#endif // QSTRINGEDITOR_H
