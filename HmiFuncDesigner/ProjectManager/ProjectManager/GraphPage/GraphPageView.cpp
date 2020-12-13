#include "GraphPageView.h"
#include "QFormListWidget.h"

#include "qpushbuttonhost.h"
#include <QVBoxLayout>
#include <QStringList>
#include <QPainter>
#include <QUuid>
#include <QDebug>

GraphPageView::GraphPageView(QtVariantPropertyManager *propertyMgr,
                             QtTreePropertyBrowser *propertyEditor,
                             QWidget *parent)
    : QWidget(parent),
      m_formListWidget(new QFormListWidget(this)),
      m_pPropertyMgrObj(propertyMgr),
      m_pPropertyEditorObj(propertyEditor)
{
    this->setAutoFillBackground(false);
    QVBoxLayout *l=new QVBoxLayout;
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(m_formListWidget);
    this->setLayout(l);

    connect(m_formListWidget, SIGNAL(selectObj(QAbstractHost*)),
            this, SIGNAL(selectObj(QAbstractHost*)));
    connect(m_formListWidget, SIGNAL(selectObj(QAbstractHost*)),
            this, SLOT(onGraphPageSelected(QAbstractHost*)));
//    connect(this, SIGNAL(property_edit_signal(QAbstractProperty*,QVariant)),
//            m_formListWidget, SLOT(property_edit_slot(QAbstractProperty*,QVariant)));

    ProjectInfoManager &projInfoMgr = ProjectData::getInstance()->projInfoMgr_;
    this->resize(projInfoMgr.getGraphPageWidth(), projInfoMgr.getGraphPageHeight());

//    GraphPage *pGraphPageObj = new GraphPage(projInfoMgr.getGraphPageWidth(),
//                                             projInfoMgr.getGraphPageHeight(),
//                                             m_pPropertyMgrObj,
//                                             m_pPropertyEditorObj);
//    QAbstractHost* pFormObj1 = pGraphPageObj;
//    pFormObj1->init();
//    pFormObj1->setAttribute(HOST_TYPE, FORM_TITLE);
//    pFormObj1->setUuid(QUuid::createUuid().toString());
//    pFormObj1->setObjectName("form1");
//    m_formListWidget->insertForm(pFormObj1);
//    m_formListWidget->showForm(pFormObj1);

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

GraphPageView::~GraphPageView()
{
}

void GraphPageView::setSelectObj(QAbstractHost *host)
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
    m_formListWidget->setSelectObj(host);
}


void GraphPageView::onGraphPageSelected(QAbstractHost* host) {
    if(host) qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << host->objectName();

    GraphPage *pGraphPageObj = dynamic_cast<GraphPage *>(host);
    if(pGraphPageObj) {
        pGraphPageObj->selected();
    }
}


GraphPage *GraphPageView::createGraphPage(const QString &szName) {
    this->hide();
    ProjectInfoManager &projInfoMgr = ProjectData::getInstance()->projInfoMgr_;
    GraphPage *pGraphPageObj = new GraphPage(projInfoMgr.getGraphPageWidth(),
                                             projInfoMgr.getGraphPageHeight(),
                                             m_pPropertyMgrObj,
                                             m_pPropertyEditorObj);
    QAbstractHost* pFormObj = pGraphPageObj;
    pFormObj->init();
    pFormObj->setAttribute(HOST_TYPE, FORM_TITLE);
    pFormObj->setUuid(QUuid::createUuid().toString());
    pFormObj->setObjectName(szName);
    m_formListWidget->insertForm(pFormObj);
    m_formListWidget->showForm(pFormObj);
    m_formListWidget->updateGeometry(pGraphPageObj);
    this->show();
    return pGraphPageObj;
}

