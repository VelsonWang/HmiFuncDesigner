#include "qbooleditor.h"
#include "../../shared/property/qabstractproperty.h"
#include <QMouseEvent>

QBoolEditor::QBoolEditor(QAbstractProperty *property, QUndoStack *stack, QWidget *parent) :
    QCommonEditor(property, stack, parent)
{
    m_icon->installEventFilter(this);
    m_text->installEventFilter(this);
}

bool QBoolEditor::eventFilter(QObject *o, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress && (m_icon == o || m_text == o)) {
        if(((QMouseEvent*)e)->button() == Qt::LeftButton) {
            if(m_property) {
                m_property->notifyEditValue(!m_property->get_value().toBool());
            }
            return true;
        }
    }
    return QCommonEditor::eventFilter(o, e);
}

void QBoolEditor::mousePressEvent(QMouseEvent *)
{
    if(m_property) {
        m_property->notifyEditValue(!m_property->get_value().toBool());
    }
}

