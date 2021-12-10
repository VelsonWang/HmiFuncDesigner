#include "qtemplatenamewidget.h"
#include "ui_qtemplatenamewidget.h"

#include "qtemplatetreewidget.h"

#include "../../../libs/core/qtoolbarbutton.h"

#include <QTreeWidgetItem>

QTemplateNameWidget::QTemplateNameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QTemplateNameWidget)
{
    ui->setupUi(this);

    QAction *ac;

    ac=new QAction(QIcon(":/images/up.png"),"",this);
    ac->setToolTip("Move to template");
    m_up_button=new QToolBarButton(ac,this);
    ui->horizontalLayout->addWidget(m_up_button);
    connect(ac,SIGNAL(triggered()),this,SIGNAL(up()));

    ac=new QAction(QIcon(":/images/removesubmitfield.png"),"",this);
    ac->setToolTip("Remove");
    m_remove_button=new QToolBarButton(ac,this);
    ui->horizontalLayout->addWidget(m_remove_button);
    connect(ac,SIGNAL(triggered()),this,SIGNAL(remove()));

    m_up_button->setVisible(false);
    m_remove_button->setVisible(false);

}

QTemplateNameWidget::~QTemplateNameWidget()
{
    delete ui;
}

void QTemplateNameWidget::set_temp_info(tagTemplateInfo *info)
{
    ui->name_label->setText(info->m_name);
    m_temp_info=info;
}

void QTemplateNameWidget::enterEvent(QEvent *)
{
    if(m_temp_info->m_type==0)
    {
        m_remove_button->setVisible(false);
        m_up_button->setVisible(false);
    }
    else if(m_temp_info->m_type==1)
    {
        m_remove_button->setVisible(true);
        m_up_button->setVisible(false);
    }
    else
    {
        m_remove_button->setVisible(true);
        m_up_button->setVisible(true);
    }
}

void QTemplateNameWidget::leaveEvent(QEvent *)
{
    m_remove_button->setVisible(false);
    m_up_button->setVisible(false);
}

void QTemplateNameWidget::set_icon(const QString &icon)
{
    ui->icon->setPixmap(QPixmap(icon));
}
