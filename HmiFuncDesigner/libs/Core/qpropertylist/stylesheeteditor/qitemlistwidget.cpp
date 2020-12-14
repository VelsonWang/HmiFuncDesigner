#include "qitemlistwidget.h"
#include "ui_qitemlistwidget.h"

#include "../../qtoolbarbutton.h"

QItemListWidget::QItemListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QItemListWidget)
{
    ui->setupUi(this);

    QAction *ac;

    ac=new QAction(QIcon(":/images/changed.png"),"",this);
    ac->setToolTip("Remove");
    m_changed_button=new QToolBarButton(ac,this);
    ui->horizontalLayout->addWidget(m_changed_button);
    connect(ac,SIGNAL(triggered()),this,SIGNAL(changed()));

    ac=new QAction(QIcon(":/images/removesubmitfield.png"),"",this);
    ac->setToolTip("Remove");
    m_remove_button=new QToolBarButton(ac,this);
    ui->horizontalLayout->addWidget(m_remove_button);
    connect(ac,SIGNAL(triggered()),this,SIGNAL(remove()));


    m_remove_button->setVisible(false);
    m_changed_button->setVisible(false);

}

QItemListWidget::~QItemListWidget()
{
    delete ui;
}

void QItemListWidget::set_text(const QString &text)
{
    if(text=="Normal")
    {
        m_remove_button->setVisible(false);
        m_changed_button->setVisible(false);
    }
    ui->text->setText(text);
    this->setToolTip(text);
}

void QItemListWidget::enterEvent(QEvent *)
{
    if(ui->text->text()!="Normal")
    {
        m_remove_button->setVisible(true);
        m_changed_button->setVisible(true);
    }
}

void QItemListWidget::leaveEvent(QEvent *)
{
    m_remove_button->setVisible(false);
    m_changed_button->setVisible(false);
}

void QItemListWidget::set_icon(const QString &icon)
{
    ui->icon->setPixmap(QPixmap(icon));
}
