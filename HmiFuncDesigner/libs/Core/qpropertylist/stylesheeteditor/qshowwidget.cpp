#include "qshowwidget.h"
#include "ui_qshowwidget.h"

#include "../../../shared/host/qabstracthost.h"

#include <QPainter>

QShowWidget::QShowWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QShowWidget),
    m_host(Q_NULLPTR)
{
    ui->setupUi(this);

    this->resize(10,10);
}

QShowWidget::~QShowWidget()
{
    delete ui;
}

void QShowWidget::on_okbtn_clicked()
{
    emit ok();
}

void QShowWidget::on_cancelbtn_clicked()
{
    emit cancel();
}

void QShowWidget::set_item_sheet(const QString &sheet)
{
    ui->plainTextEdit->setPlainText(sheet);
}

void QShowWidget::set_host(QAbstractHost *host)
{
    m_host=host;
    QWidget* wid=(QWidget*)host->getObject();
    QSize sz=wid->size();
    if(sz.width()<145)
    {
        wid->setFixedWidth(sz.width());
    }
    if(sz.height()<145)
    {
        wid->setFixedHeight(sz.height());
    }
    QHBoxLayout *h=new QHBoxLayout;
    h->setMargin(0);
    h->setSpacing(0);
    QVBoxLayout *v=new QVBoxLayout;
    v->setMargin(2);
    v->setSpacing(0);
    v->addWidget(wid);
    h->addLayout(v);
    ui->base_widget->setLayout(h);

    wid->setVisible(true);

    wid->installEventFilter(this);
}

bool QShowWidget::eventFilter(QObject *o, QEvent *e)
{

    if(m_host!=Q_NULLPTR && o==m_host->getObject())
    {
        if(e->type()==QEvent::Paint)
        {
            if(m_host->property("need_frame").toBool())
            {
                QWidget* wid=(QWidget*)o;
                QPainter p(wid);
                p.setBrush(Qt::transparent);
                QPen pen(Qt::DotLine);
                pen.setColor(QColor(128,128,128));
                pen.setWidth(1);
                p.setPen(pen);
                p.drawRect(wid->rect().adjusted(0,0,-1,-1));
            }
        }
    }
    return false;
}

void QShowWidget::on_clear_clicked()
{
    emit clear();
}

void QShowWidget::on_clearall_clicked()
{
    emit clearall();
}
