#include "qfilelistitemwidget.h"
#include "ui_qfilelistitemwidget.h"

#include "qfilelistview.h"
#include "qtoolbarbutton.h"
#include "qfilemanager.h"

QFileListItemWidget::QFileListItemWidget(QFileListView *view,
                                         QTreeWidgetItem *item,
                                         QWidget *parent):
    QWidget(parent),
    ui(new Ui::QFileListItemWidget),
    m_item(item),
    m_view(view)
{
    ui->setupUi(this);

    QAction *ac;

    ac=new QAction(QIcon(":/images/removesubmitfield.png"),"",this);
    ac->setToolTip("Remove");
    m_remove_button=new QToolBarButton(ac,this);
    ui->horizontalLayout->addWidget(m_remove_button);
    connect(ac,SIGNAL(triggered()),this,SIGNAL(remove()));

    m_remove_button->setVisible(false);
}

QFileListItemWidget::~QFileListItemWidget()
{
    delete ui;
}

void QFileListItemWidget::set_text(const QString &text)
{
    ui->text->setText(text);
    this->setToolTip(text);
}

void QFileListItemWidget::enterEvent(QEvent *)
{
    if(m_view->can_remove())
    {
        tagFileGroupInfo *g=m_view->get_group(m_item);
        if(g!=NULL)
        {
            foreach(tagFileInfo *info,g->m_files)
            {
                if(info->m_type==FILE_SYSTEM)
                {
                    return;
                }
            }
            m_remove_button->setVisible(true);
        }
        else
        {
            tagFileInfo *f=m_view->get_file(m_item);
            m_remove_button->setVisible(f->m_type!=FILE_SYSTEM);
        }
    }
}

void QFileListItemWidget::leaveEvent(QEvent *)
{
    m_remove_button->setVisible(false);
}

void QFileListItemWidget::set_icon(const QString &icon)
{
    ui->icon->setPixmap(QPixmap(icon));
}
