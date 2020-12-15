#include "qpagewidget.h"

#include "qtemplatetreewidget.h"
#include "qtempwidget.h"

#include "../../../libs/core/minisplitter.h"
#include "../../../libs/shared/xmlobject.h"
#include "../../../libs/core/qrenamedialog.h"
#include "../../../libs/core/qsoftcore.h"
#include "../../../libs/shared/qprojectcore.h"
#include "../../../libs/shared/qpagemanager.h"
#include "../../../libs/shared/host/qabstracthost.h"

#include <QFile>
#include <QVBoxLayout>
#include <QApplication>

QPageWidget::QPageWidget(const QString &type, QWidget *parent):
    QWidget(parent),
    m_tree_widget(new QTemplateTreeWidget(type)),
    m_temp_widget(new QTempWidget),
    m_type(type)
{
    MiniSplitter *s=new MiniSplitter;
    s->addWidget(m_tree_widget);
    s->addWidget(m_temp_widget);
    s->setStretchFactor(0,0);
    s->setStretchFactor(1,1);

    QVBoxLayout *v=new QVBoxLayout;
    v->setMargin(0);
    v->setSpacing(0);
    v->addWidget(s);
    setLayout(v);

    connect(m_tree_widget,SIGNAL(select(tagTemplateInfo*)),m_temp_widget,SLOT(set_current(tagTemplateInfo*)));
    m_tree_widget->init();
    connect(m_temp_widget,SIGNAL(add_file(QString)),this,SLOT(add_file(QString)));
    connect(m_temp_widget,SIGNAL(cancel()),this,SIGNAL(cancel()));
    connect(m_temp_widget,SIGNAL(ok(QAbstractHost*)),this,SIGNAL(ok(QAbstractHost*)));

    QList<QAbstractHost*> list=QSoftCore::getCore()->getProjectCore()->get_page_manager()->getPages_by_title(m_type);

    QStringList l;
    foreach(QAbstractHost* host,list)
    {
        l.append(host->getPropertyValue("objectName").toString());
    }

    m_temp_widget->set_name_list(l);
}

void QPageWidget::add_file(const QString &file)
{
    QFile f(file);
    if(!f.open(QFile::ReadOnly))
    {
        return;
    }
    QString str=f.readAll();
    XMLObject xml;
    if(!xml.load(str,0))
    {
        return;
    }
    if(xml.getTagName()!=m_type)
    {
        return;
    }
    tagTemplateInfo *info=new tagTemplateInfo;
    info->m_type=2;
    info->m_file_name=file;
    info->m_name=f.fileName();
    int index=info->m_name.lastIndexOf("/");
    info->m_name=info->m_name.mid(index+1);
    info->m_name=info->m_name.left(info->m_name.size()-4);
    if(m_tree_widget->get_all_name().contains(info->m_name))
    {
        QRenameDialog dlg(this);
        dlg.set_name_list(m_tree_widget->get_all_name());
        dlg.exec();
        if(dlg.get_ret()==1)
        {
            info->m_name=dlg.get_new_name();
        }
        else
        {
            delete info;
            return;
        }
    }
    m_tree_widget->add_item(info);
    m_tree_widget->set_select(info);
    m_tree_widget->save();
}
