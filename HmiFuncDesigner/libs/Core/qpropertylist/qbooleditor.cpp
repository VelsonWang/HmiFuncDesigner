#include "qbooleditor.h"
#include "../../shared/property/qabstractproperty.h"
#include <QMouseEvent>

QBoolEditor::QBoolEditor(QAbstractProperty *property, QUndoStack *stack, QWidget *parent) :
    QCommonEditor(property, stack, parent)
{
    icon->installEventFilter(this);
    text->installEventFilter(this);
}

bool QBoolEditor::eventFilter(QObject *o, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress && (icon == o || text == o)) {
        if(((QMouseEvent*)e)->button() == Qt::LeftButton) {
            if(property) {
                property->notifyEditValue(!property->get_value().toBool());
            }
            return true;
        }
    }
    return QCommonEditor::eventFilter(o, e);
}

void QBoolEditor::mousePressEvent(QMouseEvent *)
{
    if(property) {
        property->notifyEditValue(!property->get_value().toBool());
    }
}

