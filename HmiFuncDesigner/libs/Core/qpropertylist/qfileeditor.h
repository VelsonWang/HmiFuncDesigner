#ifndef QFILEEDITOR_H
#define QFILEEDITOR_H

#include "qbuttoncommoneditor.h"


class QFileEditor : public QButtonCommonEditor
{
    Q_OBJECT
public:
    Q_INVOKABLE QFileEditor(QAbstractProperty* m_property, QUndoStack* stack, QWidget *parent = 0);

protected:
    void onBtnClicked();

};

#endif
