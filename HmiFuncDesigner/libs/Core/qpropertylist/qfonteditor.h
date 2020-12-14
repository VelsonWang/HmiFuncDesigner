#ifndef QFONTEDITOR_H
#define QFONTEDITOR_H

#include "qbuttoncommoneditor.h"

class QFontEditor : public QButtonCommonEditor
{
    Q_OBJECT
public:
    Q_INVOKABLE QFontEditor(QAbstractProperty* property,QUndoStack* stack,QWidget *parent = 0);
    
protected:
    void btn_clicked();
};

#endif // QFONTEDITOR_H
