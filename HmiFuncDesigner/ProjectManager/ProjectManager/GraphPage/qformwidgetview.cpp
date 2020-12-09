#include "qformwidgetview.h"
#include "qformlistwidget.h"
#include "GraphPage.h"
#include "qpushbuttonhost.h"
#include <QVBoxLayout>
#include <QStringList>
#include <QPainter>
#include <QUuid>

QFormWidgetView::QFormWidgetView(QWidget *parent) :
    QWidget(parent),
    m_formWidget(new QFormListWidget)
{
    this->setAutoFillBackground(false);
    QVBoxLayout *l=new QVBoxLayout;
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(m_formWidget);
    this->setLayout(l);

    connect(m_formWidget,SIGNAL(select(QAbstractHost*)),this,SIGNAL(select(QAbstractHost*)));
    connect(this,SIGNAL(property_edit_signal(QAbstractProperty*,QVariant)),m_formWidget,SLOT(property_edit_slot(QAbstractProperty*,QVariant)));
    this->resize(800, 600);

//    QAbstractHost* pFormObj1=new GraphPage;
//    pFormObj1->init();
//    pFormObj1->setAttribute(HOST_TYPE, FORM_TITLE);
//    pFormObj1->setUuid(QUuid::createUuid().toString());
//    pFormObj1->setObjectName("form1");
//    m_formWidget->insertForm(pFormObj1);
//    m_formWidget->showForm(pFormObj1);

//    //--------------------------------------------------------------------------

//    QAbstractHost* pBtnObj1=new QPushButtonHost();
//    pBtnObj1->init();
//    pBtnObj1->setAttribute(HOST_TYPE, "buttons.pushbutton");
//    pBtnObj1->setUuid(QUuid::createUuid().toString());
//    QRect re=pBtnObj1->property("geometry").toRect();
//    re.moveTo(100, 100);
//    re.setSize(QSize(60, 36));
//    pBtnObj1->getObject()->setProperty("geometry",re);
//    pBtnObj1->setDefault();

//    QList<QAbstractHost*> list;
//    QList<int> index;
//    list.append(pBtnObj1);
//    index.append(pFormObj1->getChildCount());

//    pFormObj1->insertChildren(index, list);

//    //--------------------------------------------------------------------------

//    QAbstractHost* pBtnObj2=new QPushButtonHost();
//    pBtnObj2->init();
//    pBtnObj2->setAttribute(HOST_TYPE, "buttons.pushbutton");
//    pBtnObj2->setUuid(QUuid::createUuid().toString());
//    QRect re2=pBtnObj2->property("geometry").toRect();
//    re2.moveTo(100, 160);
//    re2.setSize(QSize(80, 40));
//    pBtnObj2->getObject()->setProperty("geometry",re2);
//    pBtnObj2->setDefault();

//    QList<QAbstractHost*> list2;
//    QList<int> index2;
//    list2.append(pBtnObj2);
//    index2.append(pFormObj1->getChildCount());

//    pFormObj1->insertChildren(index2, list2);
}

QFormWidgetView::~QFormWidgetView()
{
}

void QFormWidgetView::setSelectObj(QAbstractHost *host)
{
    m_formWidget->setSelectObj(host);
}
