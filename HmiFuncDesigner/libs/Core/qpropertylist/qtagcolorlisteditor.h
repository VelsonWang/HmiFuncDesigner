#ifndef QTAGCOLORLISTEDITOR_H
#define QTAGCOLORLISTEDITOR_H

#include "qbuttoncommoneditor.h"


class QTagColorListEditor : public QButtonCommonEditor
{
    Q_OBJECT
public:
    Q_INVOKABLE QTagColorListEditor(QAbstractProperty* property, QUndoStack* stack, QWidget *parent = 0);

protected:
    void onBtnClicked();

};

#endif
