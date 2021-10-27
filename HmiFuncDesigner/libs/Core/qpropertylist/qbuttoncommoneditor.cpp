#include "qbuttoncommoneditor.h"
#include "../../shared/property/qabstractproperty.h"
#include <QToolButton>
#include <QLayout>
#include <QMouseEvent>

QButtonCommonEditor::QButtonCommonEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent) :
    QCommonEditor(property, stack, parent)
{
    QToolButton *btn = new QToolButton(this);
    btn->setText("...");
    btn->setFixedSize(17, 17);
    btn->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setFocusProxy(this);
    layout()->addWidget(btn);
    connect(btn, SIGNAL(clicked()), this, SLOT(onBtnClicked()));
    icon->installEventFilter(this);
    text->installEventFilter(this);
}

void QButtonCommonEditor::onBtnClicked()
{

}

void QButtonCommonEditor::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton) {
        onBtnClicked();
    }
}

bool QButtonCommonEditor::eventFilter(QObject *o, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonDblClick && (o == icon || o == text)) {
        if(((QMouseEvent*)e)->button() == Qt::LeftButton) {
            onBtnClicked();
            return true;
        }
    }
    return QCommonEditor::eventFilter(o, e);
}
