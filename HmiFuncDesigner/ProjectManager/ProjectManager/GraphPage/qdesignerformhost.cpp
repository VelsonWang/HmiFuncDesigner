#include "qdesignerformhost.h"
#include "widgethost.h"
#include "qselectwidget.h"
#include "qdesignermimedata.h"
#include "QAbstractHost.h"
#include <QLayout>
#include <QPainter>
#include <QStringList>
#include <QUuid>
#include <QDebug>

QDesignerFormHost::QDesignerFormHost(QAbstractHost *host, QWidget *parent):
    QObject(parent),
    m_root_host(host),
    m_widget_host(new WidgetHost()),
    m_rubber(new QRubberBand(QRubberBand::Rectangle,(QWidget*)host->getObject())),
    m_selection(new Selection((QWidget*)host->getObject())),
    m_click(false)
{
    connect(m_root_host,SIGNAL(removeChildrenSignal(QList<QAbstractHost*>)),
            this,SLOT(removeHostSlot(QList<QAbstractHost*>)));
    connect(m_root_host,SIGNAL(insertChildrenSignal(QList<QAbstractHost*>,QList<int>)),
            this,SLOT(insertHostSlot(QList<QAbstractHost*>,QList<int>)));
    m_widget_to_host.insert(m_root_host->getObject(),m_root_host);
    installAllEvent(m_root_host);

    QList<QAbstractHost*> children = m_root_host->getChildren();

    while(children.size()>0)
    {
        QAbstractHost *h=children.takeFirst();
        connect(h,SIGNAL(removeChildrenSignal(QList<QAbstractHost*>)),
                this,SLOT(removeHostSlot(QList<QAbstractHost*>)));
        connect(h,SIGNAL(insertChildrenSignal(QList<QAbstractHost*>,QList<int>)),
                this,SLOT(insertHostSlot(QList<QAbstractHost*>,QList<int>)));
        m_widget_to_host.insert(h->getObject(),h);
        children += h->getChildren();
        if(h->property("accept_drop").toBool())
        {
            ((QWidget*)h->getObject())->setAcceptDrops(true);
        }
    }

    m_widget_host->setFormWidget((QWidget*)m_root_host->getObject());

    QLayout *l=parent->layout();

    l->addWidget(m_widget_host);
    m_widget_host->setVisible(false);
    ((QWidget*)m_root_host->getObject())->setVisible(true);
    ((QWidget*)m_root_host->getObject())->setAcceptDrops(true);
    connect(host, SIGNAL(destroyed()), this,SLOT(hostDestory()));
    connect(m_widget_host,SIGNAL(sizeChanged(QRect,QRect)),this,SLOT(formSizeChanged(QRect,QRect)));
    connect(m_selection,SIGNAL(sizeChanged(QWidget*,QRect,QRect)),this,SLOT(widgetSizeChanged(QWidget*,QRect,QRect)));
}

QDesignerFormHost::~QDesignerFormHost()
{
    ((QWidget*)m_root_host->getObject())->setParent(Q_NULLPTR);
    delete m_widget_host;
    m_widget_host=Q_NULLPTR;
    if(m_rubber!=Q_NULLPTR)
    {
        delete m_rubber;
    }

    delete m_selection;
}

bool QDesignerFormHost::eventFilter(QObject *o, QEvent *e)
{
    QAbstractHost *h = hostFromObject(o);
    if(h!=Q_NULLPTR)
    {
        switch(e->type())
        {
        case QEvent::Paint:
            return doPaintEvent(h,(QPaintEvent*)e);
        case QEvent::MouseButtonPress:
            return doMousePressEvent(h,(QMouseEvent*)e);
        case QEvent::MouseButtonRelease:
            return doMouseReleaseEvent(h,(QMouseEvent*)e);
        case QEvent::MouseMove:
            return doMouseMoveEvent(h,(QMouseEvent*)e);
        case QEvent::MouseButtonDblClick:
            return doMouseDoubleClickedEvent(h,(QMouseEvent*)e);
        case QEvent::DragEnter:
            return doDragEnterEvent(h,(QDragEnterEvent*)e);
        case QEvent::DragLeave:
            return doDragLeaveEvent(h,(QDragLeaveEvent*)e);
        case QEvent::Drop:
            return doDropEvent(h,(QDropEvent*)e);
        case QEvent::Resize:
        case QEvent::ParentChange:
        case QEvent::Move:
            if(h!=m_root_host)
            {
                m_selection->updateGeometry((QWidget*)h->getObject());
            }
            else
            {
                m_widget_host->updateFormGeometry();
            }
            break;
        default:
            break;
        }
    }
    return false;
}

void QDesignerFormHost::show()
{
    m_widget_host->show();
    selectWidgetObj(NULL);
}

void QDesignerFormHost::hide()
{
    m_widget_host->hide();
}

QAbstractHost* QDesignerFormHost::hostFromObject(QObject *obj)
{
    QAbstractHost* h;
    while(obj!=Q_NULLPTR)
    {
        h=m_widget_to_host.value(obj);
        if(h!=Q_NULLPTR)
        {
            return h;
        }
        obj=obj->parent();
    }
    return Q_NULLPTR;
}

bool QDesignerFormHost::doPaintEvent(QAbstractHost *host, QPaintEvent *e)
{
    QWidget* wid=(QWidget*)host->getObject();
    if(host==m_root_host)
    {
        QPainter p(wid);
        p.setBrush(Qt::transparent);
        if(!host->property("grab").toBool())
        {
            p.setPen(QPen(QColor(128,128,128)));


            QRect re=e->rect();

            int x=re.x()/10;
            int y=re.y()/10;
            int w=re.width()/10+2;
            int h=re.height()/10+2;

            for(int i=0;i<w;i++)
            {
                for(int j=0;j<h;j++)
                {
                    p.drawPoint((x+i)*10,(y+j)*10);
                }
            }
        }
        else
        {
            p.fillRect(wid->rect(),QColor(128,128,128,80));
        }
        p.setPen(QColor(80,80,80));
        p.drawRect(0,0,wid->width()-1,wid->height()-1);
    }
    if(host->property("need_frame").toBool())
    {
        QPainter p(wid);
        p.setBrush(Qt::transparent);
        QPen pen(Qt::DotLine);
        pen.setColor(QColor(128,128,128));
        pen.setWidth(1);
        p.setPen(pen);
        p.drawRect(wid->rect().adjusted(0,0,-1,-1));
    }
    if(host->property("drag_enter").toBool())
    {
        QPainter p(wid);
        p.setBrush(Qt::transparent);
        p.fillRect(wid->rect(),QColor(128,128,128,80));
    }
    return false;
}

bool QDesignerFormHost::doMousePressEvent(QAbstractHost *host, QMouseEvent *e)
{
    if(m_root_host==host)
    {
        m_clickPoint=e->pos();
        m_rubber->setGeometry(QRect(m_clickPoint,QSize()));
        m_rubber->show();
        m_selection->clear();
    }
    else
    {
        m_move_point=e->pos();
        QWidget* wid=(QWidget*)host->getObject();
        if(m_selection->selectedWidgets().size()!=0)
        {
            if(m_selection->isWidgetSelected(wid))
            {
                if((e->modifiers() & Qt::ControlModifier)!=0)
                {
                    if(m_selection->current()==wid)
                    {
                        QWidget* w=m_selection->removeWidget(wid);
                        selectWidgetObj(w);
                        return true;
                    }
                    else
                    {
                        selectWidgetObj(wid);
                    }
                }
            }
            else
            {
                if((e->modifiers() & Qt::ControlModifier)==0)
                {
                    m_selection->clear();
                    selectWidgetObj(wid);
                }
                else
                {
                    QAbstractHost *curr=m_widget_to_host.value(m_selection->current());
                    QAbstractHost *s=m_widget_to_host.value(wid);
                    QAbstractHost* temp;
                    while(s->getParent()!=Q_NULLPTR)
                    {
                        temp=s->getParent();
                        if(temp==curr)
                        {
                            break;
                        }
                        if(temp==curr->getParent())
                        {
                            selectWidgetObj((QWidget*)s->getObject());
                            break;
                        }
                        s=temp;
                    }
                }
            }

        }
        else
        {
            selectWidgetObj(wid);
        }
        m_click=true;
    }
    return true;
}

bool QDesignerFormHost::doMouseReleaseEvent(QAbstractHost *host, QMouseEvent *e)
{
    (void)host;
    (void)e;
    if(!m_clickPoint.isNull())
    {
        m_clickPoint=QPoint();
        QRect re=m_rubber->geometry();
        m_rubber->setGeometry(QRect(0,0,0,0));
        m_rubber->hide();

        foreach(QObject* obj,m_widget_to_host.keys())
        {
            if(obj==m_root_host->getObject())
            {
                continue;
            }
            QAbstractHost *h=m_widget_to_host.value(obj);
            if(h->getParent()!=m_root_host)
            {
                continue;
            }
            QWidget* wid=(QWidget*)obj;
            if(!(re & wid->geometry()).isNull())
            {
                m_selection->addWidget(wid);
            }
        }

        if(m_selection->selectedWidgets().size()>0)
        {
            selectWidgetObj(m_selection->selectedWidgets().first());
        }
        else
        {
            selectWidgetObj(Q_NULLPTR);
        }

    }
    else
    {
        m_click=false;
    }
    return true;
}

bool QDesignerFormHost::doMouseMoveEvent(QAbstractHost *host, QMouseEvent *e)
{
    if(!m_clickPoint.isNull())
    {
        QRect re(QRect(m_clickPoint,e->pos()).normalized());
        m_rubber->setGeometry(re);
    }
    else
    {
        if(m_click)
        {
            if((e->pos()-m_clickPoint).manhattanLength()<10)
            {
                return true;
            }
            m_click=false;
            QWidgetList list=m_selection->selectedWidgets();
            if(list.size()>0)
            {
                QList<QDesignerDnDItemInterface*>   items;

                foreach(QWidget* wid,list)
                {
                    items.append(new FormDnItem(m_widget_to_host.value(wid),e->globalPos()));
                    wid->hide();
                    m_selection->hide(wid);
                }
                QDesignerMimeData::execDrag(items,(QWidget*)m_root_host->getObject());
                foreach(QWidget* wid,list)
                {
                    wid->show();
                    m_selection->show(wid);
                }
            }
        }
    }
    return true;
}

bool QDesignerFormHost::doMouseDoubleClickedEvent(QAbstractHost *host, QMouseEvent *e)
{
    if(host!=m_root_host)
    {
    }
    return true;
}

bool QDesignerFormHost::doDragEnterEvent(QAbstractHost *host, QDragEnterEvent *e)
{
    const QDesignerMimeData *mimeData=qobject_cast<const QDesignerMimeData*>(e->mimeData());
    if(mimeData!=Q_NULLPTR)
    {
        mimeData->acceptEvent(e);
    }

    if(host!=m_root_host)
    {
        host->setProperty("drag_enter",true);
        ((QWidget*)host->getObject())->update();
    }
    return true;
}

bool QDesignerFormHost::doDragLeaveEvent(QAbstractHost *host, QDragLeaveEvent *e)
{
    if(host!=m_root_host)
    {
        host->setProperty("drag_enter",false);
        ((QWidget*)host->getObject())->update();
    }
    return true;
}

bool QDesignerFormHost::doDropEvent(QAbstractHost *host, QDropEvent *e)
{
    const QDesignerMimeData *mimeData=qobject_cast<const QDesignerMimeData*>(e->mimeData());
    if(mimeData==Q_NULLPTR)
    {
        return true;
    }

    mimeData->acceptEvent(e);

    QList<QDesignerDnDItemInterface*>   list=mimeData->items();

    foreach(QDesignerDnDItemInterface *item,list)
    {
        if(item->type()==QDesignerDnDItemInterface::CopyDrop)
        {
//            QAbstractHost *h=QHostFactory::createHost(item->name());
//            if(h!=Q_NULLPTR)
//            {
//                h->set_uuid(QUuid::createUuid().toString());
//                QRect re=h->get_property_value("geometry").toRect();
//                re.moveTo(e->pos()-item->hotSpot());
//                h->set_property_value("geometry",re);
//                h->set_default();
//                QList<QAbstractHost*> list;
//                QList<int>          index;
//                list.append(h);
//                index.append(host->get_child_count());
//                new QAddHostUndoCommand(host,list,index,AHT_ADD,cmd);
//            }
        }
        else
        {
            QAbstractHost *h=item->host();
            QRect re=h->getObject()->property("geometry").toRect();
            QRect new_re=re;
            new_re.moveTo(e->pos()-item->hotSpot());
            h->getObject()->setProperty("geometry",new_re);
        }
    }

    if(host!=m_root_host)
    {
        host->setProperty("drag_enter",false);
        ((QWidget*)host->getObject())->update();
    }

    return true;
}

void QDesignerFormHost::hostDestory()
{
    m_rubber=Q_NULLPTR;
}

void QDesignerFormHost::selectWidgetObj(QWidget *wid)
{
    if(wid==NULL)
    {
         m_selection->clear();
         m_widget_host->showHandle();
         emit selectObj(m_root_host);
    }
    else
    {
        m_selection->addWidget(wid);
        m_selection->setCurrent(wid);
        m_widget_host->hideHandle();
        emit selectObj(m_widget_to_host.value(wid));
    }
}

QAbstractHost* QDesignerFormHost::getSelectWidgetObj()
{
    QWidget* wid=m_selection->current();
    if(wid==Q_NULLPTR)
    {
        return m_root_host;
    }
    else
    {
        return m_widget_to_host.value(wid);
    }
}

QAbstractHost* QDesignerFormHost::getRootHost()
{
    return m_root_host;
}

void QDesignerFormHost::setSelectWidgetObj(QAbstractHost *host)
{
    selectWidgetObj(Q_NULLPTR);

    if(m_root_host!=host)
    {
        selectWidgetObj((QWidget*)host->getObject());
    }
}

void QDesignerFormHost::sameLeft()
{
    if(m_selection->selectedWidgets().size()<=1)
    {
        return;
    }

    QRect base_rect=m_selection->current()->geometry();
    foreach(QWidget* wid,m_selection->selectedWidgets())
    {
        QAbstractHost *h=m_widget_to_host.value(wid);
        QRect re=wid->geometry();
        re.moveTo(base_rect.left(),re.top());
        if(re!=wid->geometry())
        {
            //new QPropertyChangedUndoCommand(h->get_uuid(),"geometry",wid->geometry(),re,cmd);
        }
    }
}

void QDesignerFormHost::sameTop()
{
    if(m_selection->selectedWidgets().size()<=1)
    {
        return;
    }

    QRect base_rect=m_selection->current()->geometry();
    foreach(QWidget* wid,m_selection->selectedWidgets())
    {
        QAbstractHost *h=m_widget_to_host.value(wid);
        QRect re=wid->geometry();
        re.moveTo(re.left(),base_rect.top());
        if(re!=wid->geometry())
        {
            //new QPropertyChangedUndoCommand(h->get_uuid(),"geometry",wid->geometry(),re,cmd);
        }
    }
}

void QDesignerFormHost::sameRight()
{
    if(m_selection->selectedWidgets().size()<=1)
    {
        return;
    }

    QRect base_rect=m_selection->current()->geometry();
    foreach(QWidget* wid,m_selection->selectedWidgets())
    {
        QAbstractHost *h=m_widget_to_host.value(wid);
        QRect re=wid->geometry();
        re.moveTo(base_rect.left()+base_rect.width()-re.width(),re.top());
        if(re!=wid->geometry())
        {
            //new QPropertyChangedUndoCommand(h->get_uuid(),"geometry",wid->geometry(),re,cmd);
        }
    }
}

void QDesignerFormHost::sameBottom()
{
    if(m_selection->selectedWidgets().size()<=1)
    {
        return;
    }

    QRect base_rect=m_selection->current()->geometry();
    foreach(QWidget* wid,m_selection->selectedWidgets())
    {
        QAbstractHost *h=m_widget_to_host.value(wid);
        QRect re=wid->geometry();
        re.moveTo(re.left(),base_rect.top()+base_rect.height()-re.height());
        if(re!=wid->geometry())
        {
            //new QPropertyChangedUndoCommand(h->get_uuid(),"geometry",wid->geometry(),re,cmd);
        }
    }
}

void QDesignerFormHost::sameWidth()
{
    if(m_selection->selectedWidgets().size()<=1)
    {
        return;
    }

    QRect base_rect=m_selection->current()->geometry();
    foreach(QWidget* wid,m_selection->selectedWidgets())
    {
        QAbstractHost *h=m_widget_to_host.value(wid);
        QRect re=wid->geometry();
        re.setWidth(base_rect.width());
        if(re!=wid->geometry())
        {
            //new QPropertyChangedUndoCommand(h->get_uuid(),"geometry",wid->geometry(),re,cmd);
        }
    }
}

void QDesignerFormHost::sameHeight()
{
    if(m_selection->selectedWidgets().size()<=1)
    {
        return;
    }

    QRect base_rect=m_selection->current()->geometry();
    foreach(QWidget* wid,m_selection->selectedWidgets())
    {
        QAbstractHost *h=m_widget_to_host.value(wid);
        QRect re=wid->geometry();
        re.setHeight(base_rect.height());
        if(re!=wid->geometry())
        {
            //new QPropertyChangedUndoCommand(h->get_uuid(),"geometry",wid->geometry(),re,cmd);
        }
    }
}

void QDesignerFormHost::sameGeometry()
{
    if(m_selection->selectedWidgets().size()<=1)
    {
        return;
    }

    QRect base_rect=m_selection->current()->geometry();
    foreach(QWidget* wid,m_selection->selectedWidgets())
    {
        QAbstractHost *h=m_widget_to_host.value(wid);
        QRect re=wid->geometry();
        re.setWidth(base_rect.width());
        re.setHeight(base_rect.height());
        if(re!=wid->geometry())
        {
            //new QPropertyChangedUndoCommand(h->get_uuid(),"geometry",wid->geometry(),re,cmd);
        }
    }
}

void QDesignerFormHost::sameVCenter()
{
    if(m_selection->selectedWidgets().size()<=1)
    {
        return;
    }

    QRect base_rect=m_selection->current()->geometry();
    foreach(QWidget* wid,m_selection->selectedWidgets())
    {
        QAbstractHost *h=m_widget_to_host.value(wid);
        QRect re=wid->geometry();
        re.moveTo(base_rect.left()+(base_rect.width()-re.width())/2,re.top());
        if(re!=wid->geometry())
        {
            //new QPropertyChangedUndoCommand(h->get_uuid(),"geometry",wid->geometry(),re,cmd);
        }
    }
}

void QDesignerFormHost::sameHCenter()
{
    if(m_selection->selectedWidgets().size()<=1)
    {
        return;
    }

    QRect base_rect=m_selection->current()->geometry();
    foreach(QWidget* wid,m_selection->selectedWidgets())
    {
        QAbstractHost *h=m_widget_to_host.value(wid);
        QRect re=wid->geometry();
        re.moveTo(re.left(),base_rect.top()+(base_rect.height()-re.height())/2);
        if(re!=wid->geometry())
        {
            //new QPropertyChangedUndoCommand(h->get_uuid(),"geometry",wid->geometry(),re,cmd);
        }
    }
}

void QDesignerFormHost::formSizeChanged(const QRect &old, const QRect &now)
{
    if(old!=now)
    {
        //QPropertyChangedUndoCommand *cmd=new QPropertyChangedUndoCommand(m_root_host->get_uuid(),"geometry",old,now);
        //m_undo_stack->push(cmd);

    }
}

void QDesignerFormHost::widgetSizeChanged(QWidget* wid,const QRect &old, const QRect &now)
{
    if(old!=now)
    {
        QAbstractHost *h=m_widget_to_host.value(wid);
        if(h!=Q_NULLPTR)
        {
            //QPropertyChangedUndoCommand *cmd=new QPropertyChangedUndoCommand(h->get_uuid(),"geometry",old,now);
            //m_undo_stack->push(cmd);
        }

    }
}

void QDesignerFormHost::insertHostSlot(QList<QAbstractHost *> hosts, QList<int>)
{
    m_selection->clear();

    int count=hosts.size();
    QList<QAbstractHost*> c;
    for(int i=0;i<count;i++)
    {
        QAbstractHost* h=hosts.at(i);
        installAllEvent(h);
        connect(h,SIGNAL(insertChildrenSignal(QList<QAbstractHost*>,QList<int>)),
                this,SLOT(insertHostSlot(QList<QAbstractHost*>,QList<int>)));
        connect(h,SIGNAL(removeChildrenSignal(QList<QAbstractHost*>)),
                this,SLOT(removeHostSlot(QList<QAbstractHost*>)));
        m_widget_to_host.insert(h->getObject(),h);
        if(h->property("accept_drop").toBool())
        {
            ((QWidget*)h->getObject())->setAcceptDrops(true);
        }
        c=h->getChildren();
        while(c.size()>0)
        {
            QAbstractHost *hh=c.takeFirst();
            connect(hh,SIGNAL(insertChildrenSignal(QList<QAbstractHost*>,QList<int>)),
                    this,SLOT(insertHostSlot(QList<QAbstractHost*>,QList<int>)));
            connect(hh,SIGNAL(removeChildrenSignal(QList<QAbstractHost*>)),
                    this,SLOT(removeHostSlot(QList<QAbstractHost*>)));
            m_widget_to_host.insert(hh->getObject(),hh);
            c+=hh->getChildren();
        }
        QWidget* wid=(QWidget*)h->getObject();
        m_selection->addWidget(wid);
        wid->show();
    }

    selectWidgetObj(m_selection->selectedWidgets().first());
}

void QDesignerFormHost::removeHostSlot(QList<QAbstractHost *> hosts)
{
    selectWidgetObj(Q_NULLPTR);
    foreach(QAbstractHost* h,hosts)
    {
        QList<QAbstractHost*> children=h->getChildren();
        removeAllEvent(h);
        disconnect(h,SIGNAL(insertChildrenSignal(QList<QAbstractHost*>,QList<int>)),
                this,SLOT(insertHostSlot(QList<QAbstractHost*>,QList<int>)));
        disconnect(h,SIGNAL(removeChildrenSignal(QList<QAbstractHost*>)),
                this,SLOT(removeHostSlot(QList<QAbstractHost*>)));
        m_widget_to_host.remove(h->getObject());
        while(children.size()>0)
        {
            QAbstractHost *hh=children.takeFirst();
            disconnect(hh,SIGNAL(insertChildrenSignal(QList<QAbstractHost*>,QList<int>)),
                    this,SLOT(insertHostSlot(QList<QAbstractHost*>,QList<int>)));
            disconnect(hh,SIGNAL(removeChildrenSignal(QList<QAbstractHost*>)),
                    this,SLOT(removeHostSlot(QList<QAbstractHost*>)));
            m_widget_to_host.remove(hh->getObject());
            children+=hh->getChildren();
        }
    }
}

void QDesignerFormHost::installAllEvent(QAbstractHost *host)
{
    QObjectList list;
    list.append(host->getObject());

    while(list.size()>0)
    {
        QObject* obj=list.takeFirst();
        if(obj==m_rubber)
        {
            continue;
        }
        obj->installEventFilter(this);
        list+=obj->children();
    }
}

void QDesignerFormHost::removeAllEvent(QAbstractHost *host)
{
    QObjectList list;
    list.append(host->getObject());

    while(list.size()>0)
    {
        QObject* obj=list.takeFirst();
        if(obj==m_rubber)
        {
            continue;
        }
        obj->removeEventFilter(this);
        list+=obj->children();
    }
}
