#include "qpropertybaseeditor.h"

#include "../../shared/property/qabstractproperty.h"
#include "../../shared/qpropertyfactory.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QEvent>
#include <QApplication>
#include <QLabel>

QPropertyBaseEditor::QPropertyBaseEditor(QAbstractProperty *pro,QUndoStack* stack, QWidget *parent) :
    QWidget(parent),
    m_property(pro)
{
    this->setFocusPolicy(Qt::StrongFocus);
    m_resetButton=new QToolButton(this);
    m_resetButton->setVisible(m_property->getAttribute(ATTR_RESET_ABLEABLE).toBool());
    m_resetButton->setIcon(QIcon(":/images/reset.png"));
    m_resetButton->setIconSize(QSize(8,8));
    m_resetButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::MinimumExpanding));
    m_resetButton->setFocusPolicy(Qt::NoFocus);
    m_resetButton->setFocusProxy(this);
    m_resetButton->setEnabled(m_property->modified());

    connect(m_resetButton,SIGNAL(clicked()),this,SLOT(reset()));
    connect(m_property,SIGNAL(refresh()),this,SLOT(onPropertyRefresh()));

    this->setProperty("no-ManhattanStyle",true);

    m_widget=QPropertyFactory::create_editor(pro->getObjectProperty("type").toString(),pro,stack);
    if(m_widget==NULL)
    {
        QLabel *lable=new QLabel;
        lable->setFocusPolicy(Qt::StrongFocus);
        lable->setText(pro->get_value_text());
        lable->setMargin(3);
        m_widget=lable;
    }
    this->setFocusProxy(m_widget);
    m_widget->installEventFilter(this);

    QHBoxLayout *l=new QHBoxLayout;
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(m_widget);
    l->addWidget(m_resetButton);

    this->setLayout(l);
}

void QPropertyBaseEditor::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(this->rect(),Qt::white);
}

void QPropertyBaseEditor::reset()
{
    m_resetButton->setEnabled(false);
    m_property->reset();
}

bool QPropertyBaseEditor::eventFilter(QObject *o, QEvent *e)
{
    if(o==m_widget && e->type()==QEvent::FocusOut)
    {
        qApp->sendEvent(this,e);
    }
    return QWidget::eventFilter(o,e);
}

void QPropertyBaseEditor::onPropertyRefresh()
{
    m_resetButton->setEnabled(m_property->modified());
}
