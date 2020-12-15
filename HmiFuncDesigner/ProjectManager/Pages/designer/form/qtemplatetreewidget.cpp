#include "qtemplatetreewidget.h"

#include "qtemplatenamewidget.h"

#include "../../../libs/core/qrenamedialog.h"
#include "../../../libs/shared/xmlobject.h"
#include "../../../libs/core/stylehelper.h"

#include <QItemDelegate>
#include <QHeaderView>
#include <QPainter>
#include <QMessageBox>
#include <QFile>
#include <QApplication>
#include <QMouseEvent>


class QTemplateDeletgate: public QItemDelegate
{
public:
    QTemplateDeletgate(QTemplateTreeWidget* view):m_view(view){}

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize   sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void    paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
    QTemplateTreeWidget *m_view;
};


void QTemplateDeletgate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV3 opt=option;
    QColor c;
    QTreeWidgetItem *item=m_view->itemFromIndex(index);
    if(item==m_view->m_default || item==m_view->m_more)
    {
        c=QColor(128,128,128);
    }
    else
    {
        c=QColor(255,220,150);
        if(opt.features & QStyleOptionViewItemV2::Alternate)
        {
            c=c.lighter(132);
        }
    }

    painter->fillRect(option.rect,c);
    opt.state &=~ QStyle::State_HasFocus;
    QItemDelegate::paint(painter,opt,index);
}

QSize QTemplateDeletgate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize sz=QItemDelegate::sizeHint(option,index);
    sz.setHeight(20);
    return sz;
}

QWidget* QTemplateDeletgate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTemplateNameWidget* wid=NULL;
    return wid;
}



QTemplateTreeWidget::QTemplateTreeWidget(const QString &type, QWidget *parent):
    QTreeWidget(parent),
    m_default(new QTreeWidgetItem(this)),
    m_more(new QTreeWidgetItem(this)),
    m_type(type),
    m_current_item(NULL)
{
    this->setItemDelegate(new QTemplateDeletgate(this));
    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->header()->hide();
    this->setFrameStyle(QFrame::NoFrame);
    this->setMinimumSize(150,150);

    QIcon icon=StyleHelper::drawIndicatorIcon(this->palette(),this->style());

    m_default->setFlags(Qt::ItemIsEnabled);
    m_default->setText(0,tr("Locale"));
    m_default->setIcon(0,icon);
    this->setItemExpanded(m_default,true);

    m_more->setFlags(Qt::ItemIsEnabled);
    m_more->setText(0,tr("Temporary"));
    m_more->setIcon(0,icon);
    this->setItemExpanded(m_more,true);

    QPalette p=this->palette();
    p.setColor(QPalette::Inactive,QPalette::Highlight,p.color(QPalette::Active,QPalette::Highlight));
    p.setColor(QPalette::Inactive,QPalette::HighlightedText,p.color(QPalette::Active,QPalette::HighlightedText));
    this->setPalette(p);

    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(double_clicked(QTreeWidgetItem*)));
}

QTemplateTreeWidget::~QTemplateTreeWidget()
{
    qDeleteAll(m_infos);
}


void QTemplateTreeWidget::add_item(tagTemplateInfo *info)
{
    QTreeWidgetItem *item=m_name_to_item.value(info->m_name);
    if(item!=NULL)
    {
        return;
    }

    item=new QTreeWidgetItem(info->m_type<2?m_default:m_more);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    m_item_to_info.insert(item,info);
    m_name_to_item.insert(info->m_name,item);
    m_info_to_item.insert(info,item);
    m_infos.append(info);
    QTemplateNameWidget *wid=new QTemplateNameWidget;
    wid->set_temp_info(info);
    connect(wid,SIGNAL(remove()),this,SLOT(remove()));
    connect(wid,SIGNAL(up()),this,SLOT(up()));
    this->setItemWidget(item,0,wid);

}

void QTemplateTreeWidget::set_select(tagTemplateInfo *info)
{
    QTreeWidgetItem* item=m_info_to_item.value(info);
    double_clicked(item);
}

void QTemplateTreeWidget::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    QStyleOptionViewItemV3 opt=options;
    QTreeWidgetItem* item=itemFromIndex(index);
    QColor c;
    if(item==m_default || item==m_more)
    {
        c=QColor(128,128,128);
        painter->fillRect(options.rect,c);
        opt.palette.setColor(QPalette::AlternateBase,c.lighter(112));
    }
    else
    {
        c=QColor(255,220,150);
        painter->fillRect(options.rect,c);
        opt.palette.setColor(QPalette::AlternateBase,c.lighter(132));
    }

    opt.state &=~ QStyle::State_HasFocus;
    QTreeWidget::drawRow(painter,opt,index);
}

void QTemplateTreeWidget::remove()
{
    QTemplateNameWidget *wid=(QTemplateNameWidget*)sender();
    QMapIterator<QTreeWidgetItem*,tagTemplateInfo*> it(m_item_to_info);
    QTreeWidgetItem *item=NULL;
    while(it.hasNext())
    {
        it.next();
        if(itemWidget(it.key(),0)==wid)
        {
            item=it.key();
            break;
        }
    }
    if(item==NULL)
    {
        return;
    }
    tagTemplateInfo *info=m_item_to_info.value(item);
    if(info->m_type<2)
    {
        if(QMessageBox::warning(this,tr("Remove"),tr("Do you want to delete this template?"),QMessageBox::Yes | QMessageBox::No)!=QMessageBox::Yes)
        {
            return;
        }
    }
    double_clicked(m_default->child(0));
    if(info->m_type==1)
    {
        QFile f(qApp->applicationDirPath()+"/template/"+m_type+"/"+info->m_file_name+".xml");
        f.remove();

    }
    m_item_to_info.remove(item);
    m_info_to_item.remove(info);
    m_name_to_item.remove(info->m_name);
    m_infos.removeAll(info);
    delete info;
    delete item;
    save();

}

void QTemplateTreeWidget::up()
{
    QTemplateNameWidget *wid=(QTemplateNameWidget*)sender();
    QMapIterator<QTreeWidgetItem*,tagTemplateInfo*> it(m_item_to_info);
    QTreeWidgetItem *item=NULL;
    while(it.hasNext())
    {
        it.next();
        if(itemWidget(it.key(),0)==wid)
        {
            item=it.key();
            break;
        }
    }
    if(item==NULL)
    {
        return;
    }
    tagTemplateInfo *info=m_item_to_info.value(item);
    if(info->m_type>1)
    {
        QString path=qApp->applicationDirPath()+"/template/"+m_type+"/"+info->m_name+".xml";
        QFile f1(info->m_file_name);
        QFile f2(path);
        f1.open(QFile::ReadOnly);
        f2.open(QFile::WriteOnly);
        f2.resize(0);
        f2.write(f1.readAll());
        f2.close();
        f1.close();
        info->m_file_name=info->m_name+".xml";
        info->m_type=1;
        m_item_to_info.remove(item);
        m_info_to_item.remove(info);
        m_name_to_item.remove(info->m_name);
        delete item;
        item=new QTreeWidgetItem(m_default);
        m_item_to_info.insert(item,info);
        m_info_to_item.insert(info,item);
        m_name_to_item.insert(info->m_name,item);
        QTemplateNameWidget* wid=new QTemplateNameWidget;
        wid->set_temp_info(info);
        connect(wid,SIGNAL(remove()),this,SLOT(remove()));
        connect(wid,SIGNAL(up()),this,SLOT(up()));
        setItemWidget(item,0,wid);
        save();
    }
}


void QTemplateTreeWidget::init()
{
    tagTemplateInfo *t=new tagTemplateInfo;
    t->m_name="Base "+m_type;
    t->m_file_name=":/files/default_form.xml";
    t->m_type=0;
    add_item(t);
    setCurrentItem(m_default->child(0));
    m_current_item=m_default->child(0);
    QTemplateNameWidget *wid=(QTemplateNameWidget*)itemWidget(m_current_item,0);
    wid->set_icon(":/images/check.png");
    emit select(t);
    QFile f(qApp->applicationDirPath()+"/template/"+m_type+"/templates.xml");
    if(!f.open(QFile::ReadOnly))
    {
        return;
    }

    QString buff=f.readAll();
    f.close();

    XMLObject xml;
    if(!xml.load(buff,0))
    {
        return;
    }

    if(xml.getTagName()!="Templates")
    {
        return;
    }

    QList<XMLObject*> list=xml.getChildren();

    foreach(XMLObject* c,list)
    {
        if(c->getTagName()=="Template")
        {
            int type=c->getProperty("level").toInt();
            QString path;
            if(type<2)
            {
                path=qApp->applicationDirPath()+"/template/"+m_type+"/"+c->getProperty("path");
            }
            else
            {
                path=c->getProperty("path");
            }
            QFile file(path);
            if(file.open(QFile::ReadOnly))
            {
                QString buff=file.readAll();
                XMLObject o;
                if(o.load(buff,0))
                {
                    if(o.getTagName()==m_type)
                    {
                        tagTemplateInfo *info=new tagTemplateInfo;
                        info->m_type=type;
                        info->m_name=c->getProperty("name");
                        info->m_file_name=c->getProperty("path");
                        add_item(info);
                    }
                }
            }
        }
    }
}

void QTemplateTreeWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
    if(event->x()<20)
    {
        QTreeWidgetItem *item=itemAt(event->pos());
        if(item==m_more || item==m_default)
        {
            if(item->childCount()>0)
            {
                item->setExpanded(!item->isExpanded());
            }
            else
            {
                item->setExpanded(false);
            }
        }
    }
}

void QTemplateTreeWidget::double_clicked(QTreeWidgetItem *item)
{
    tagTemplateInfo *info=m_item_to_info.value(item);
    if(info==NULL)
    {
        return;
    }

    if(m_current_item!=NULL)
    {
        QTemplateNameWidget *wid=(QTemplateNameWidget*)itemWidget(m_current_item,0);
        wid->set_icon("");
    }
    m_current_item=item;
    if(m_current_item!=NULL)
    {
        QTemplateNameWidget *wid=(QTemplateNameWidget*)itemWidget(m_current_item,0);
        wid->set_icon(":/images/check.png");
    }
    emit select(info);
}

void QTemplateTreeWidget::save()
{
    XMLObject xml;
    xml.setTagName("Templates");

    foreach(tagTemplateInfo* info,m_infos)
    {
        if(info->m_file_name.startsWith(":"))
        {
            continue;
        }
        XMLObject* o=new XMLObject(&xml);
        o->setTagName("Template");
        o->setProperty("path",info->m_file_name);
        o->setProperty("name",info->m_name);
        o->setProperty("level",QString::number(info->m_type));
    }

    QFile f(qApp->applicationDirPath()+"/template/"+m_type+"/templates.xml");
    if(!f.open(QFile::WriteOnly))
    {
        return;
    }

    QString str=xml.write();
    f.resize(0);
    f.write(str.toLocal8Bit());
    f.close();
}

QStringList QTemplateTreeWidget::get_all_name()
{
    return m_name_to_item.keys();
}
