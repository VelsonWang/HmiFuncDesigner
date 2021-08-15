#ifndef QSTYLESHEETEDITOR_H
#define QSTYLESHEETEDITOR_H

#include "../qbuttoncommoneditor.h"

#include <QUndoStack>

class QStyleSheetEditor : public QButtonCommonEditor
{
    Q_OBJECT
public:
    Q_INVOKABLE QStyleSheetEditor(QAbstractProperty* property,QUndoStack* stack,QWidget *parent = 0);
    
protected:
    void onBtnClicked();

protected:
    QUndoStack  *m_undo_stack;
};

#endif // QSTYLESHEETEDITOR_H
