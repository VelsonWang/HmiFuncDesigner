#ifndef QTAGTEXTLISTEDITOR_H
#define QTAGTEXTLISTEDITOR_H

#include "qbuttoncommoneditor.h"


class QTagTextListEditor : public QButtonCommonEditor
{
    Q_OBJECT
public:
    Q_INVOKABLE QTagTextListEditor(QAbstractProperty* m_property, QUndoStack* stack, QWidget *parent = 0);

protected:
    void onBtnClicked();

};

#endif
