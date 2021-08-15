#ifndef QABSTRACTPAGEWIDGET_H
#define QABSTRACTPAGEWIDGET_H

#include "corelibglobal.h"

#include <QWidget>
#include <QUndoStack>

class CORELIB_EXPORT QAbstractPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QAbstractPageWidget(QWidget *parent = 0);


    void    setVisible(bool visible);

    virtual void    setUndoStack(QUndoStack *stack);
signals:
    void    selected(bool select);

public slots:
protected:
    QUndoStack      *m_undo_stack;
};

#endif // QABSTRACTPAGEWIDGET_H
