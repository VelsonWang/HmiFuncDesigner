#include "qresourceview.h"

#include "qresourceshowwidget.h"

#include "../../qfilelistview.h"
#include "../../minisplitter.h"
#include "../../qfilemanager.h"

#include <QVBoxLayout>

QResourceView::QResourceView(QWidget *parent) :
    QDialog(parent),
    m_file_view(new QFileListView(false,true,false)),
    m_show_widget(new QResourceShowWidget),
    m_file_info(NULL),
    m_ret(0)
{
    MiniSplitter *s=new MiniSplitter;
    s->addWidget(m_file_view);
    s->addWidget(m_show_widget);

    QVBoxLayout *v=new QVBoxLayout;
    v->setMargin(0);
    v->setSpacing(0);
    v->addWidget(s);

    this->setLayout(v);

    this->resize(200,300);

    connect(m_file_view,SIGNAL(select_file_signal(tagFileInfo*)),this,SLOT(select(tagFileInfo*)));

    connect(m_show_widget,SIGNAL(cancel()),this,SLOT(close()));
    connect(m_show_widget,SIGNAL(ok()),this,SLOT(ok()));
}

QResourceView::~QResourceView()
{
}

void QResourceView::select(tagFileInfo *file)
{
    m_file_info=file;
    m_show_widget->set_image(file->m_file_data);
}

void QResourceView::set_file(const QByteArray &data)
{
    m_file_info=NULL;
    m_show_widget->set_image(data);
}

int QResourceView::get_ret()
{
    return m_ret;
}

tagFileInfo *QResourceView::get_file()
{
    return m_file_info;
}

void QResourceView::ok()
{
    m_ret=1;
    close();
}
