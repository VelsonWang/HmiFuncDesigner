#ifndef QBUTTONCOMMONEDITOR_H
#define QBUTTONCOMMONEDITOR_H

#include "qcommoneditor.h"

class QButtonCommonEditor : public QCommonEditor
{
    Q_OBJECT
public:
    QButtonCommonEditor(QAbstractProperty* m_property, QUndoStack* stack, QWidget *parent = 0);

protected:
    bool eventFilter(QObject *, QEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

protected slots:
    virtual void onBtnClicked();
};

#endif // QBUTTONCOMMONEDITOR_H
