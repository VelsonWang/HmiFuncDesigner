#ifndef QBOOLEDITOR_H
#define QBOOLEDITOR_H

#include "qcommoneditor.h"

class QBoolEditor : public QCommonEditor
{
    Q_OBJECT
public:
    Q_INVOKABLE QBoolEditor(QAbstractProperty* m_property, QUndoStack* stack, QWidget *parent = 0);

protected:
    bool eventFilter(QObject *, QEvent *);
    void mousePressEvent(QMouseEvent *);

};

#endif // QBOOLEDITOR_H
