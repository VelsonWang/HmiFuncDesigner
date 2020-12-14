#include "qbuttoncommoneditor.h"

#include "../../shared/property/qabstractproperty.h"

#include <QToolButton>
#include <QLayout>
#include <QMouseEvent>

QButtonCommonEditor::QButtonCommonEditor(QAbstractProperty *property,QUndoStack* stack, QWidget *parent):
    QCommonEditor(property,stack,parent)
{
    QToolButton *btn=new QToolButton(this);
    btn->setText("...");
    btn->setFixedSize(17,17);
    btn->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::MinimumExpanding));
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setFocusProxy(this);
    layout()->addWidget(btn);
    connect(btn,SIGNAL(clicked()),this,SLOT(btn_clicked()));

    m_icon->installEventFilter(this);
    m_text->installEventFilter(this);
}

void QButtonCommonEditor::btn_clicked()
{

}

void QButtonCommonEditor::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        btn_clicked();
    }
}

bool QButtonCommonEditor::eventFilter(QObject *o, QEvent *e)
{
    if(e->type()==QEvent::MouseButtonDblClick && (o==m_icon || o==m_text))
    {
        if(((QMouseEvent*)e)->button()==Qt::LeftButton)
        {
            btn_clicked();
            return true;
        }
    }
    return QCommonEditor::eventFilter(o,e);
}
