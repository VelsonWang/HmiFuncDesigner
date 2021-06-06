#ifndef QIMAGEEDITOR_H
#define QIMAGEEDITOR_H

#include "qbuttoncommoneditor.h"


class QImageEditor : public QButtonCommonEditor
{
    Q_OBJECT
public:
    Q_INVOKABLE QImageEditor(QAbstractProperty* property, QUndoStack* stack, QWidget *parent = 0);

protected:
    void onBtnClicked();

};

#endif
