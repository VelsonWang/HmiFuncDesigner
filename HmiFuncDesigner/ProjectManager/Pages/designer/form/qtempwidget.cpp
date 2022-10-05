#include "qtempwidget.h"
#include "ui_qtempwidget.h"
#include "qtemplatetreewidget.h"
#include "../../../libs/shared/xmlobject.h"
#include "../../../libs/shared/qhostfactory.h"
#include "../../../libs/shared/host/qabstracthost.h"
#include "../../../libs/core/qnamevalidator.h"
#include <QFile>
#include <QPainter>
#include <QFileDialog>

QTempWidget::QTempWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QTempWidget),
    m_current(NULL),
    m_name_edit(new QNameEdit("[_a-zA-Z][_a-zA-Z0-9]{,64}"))
{
    ui->setupUi(this);
    ui->horizontalLayout_3->addWidget(m_name_edit);
}

QTempWidget::~QTempWidget()
{
    qDeleteAll(m_info_to_host.values());
    delete ui;
}

void QTempWidget::set_current(tagTemplateInfo *info)
{
    if(m_current == info) {
        return;
    }
    QAbstractHost* pHostObj = m_info_to_host.value(info);
    if(pHostObj == NULL) {
        QFile f(info->m_file_name);
        if(!f.open(QFile::ReadOnly)) {
            return;
        }
        QString s = f.readAll();

        XMLObject xml;
        if(!xml.load(s, 0)) {
            return;
        }
        pHostObj = QHostFactory::createHost(&xml);
        if(pHostObj == NULL) {
            return;
        }
        pHostObj->setDefault();
        pHostObj->setID(QString::number(pHostObj->allocID()));
        m_info_to_host.insert(info, pHostObj);
        pHostObj->getObject()->installEventFilter(this);
    }

    m_current = info;

    QWidget* wid = (QWidget*)pHostObj->getObject();

    QPixmap widgetPixmap = wid->grab();
    QSize sz = widgetPixmap.size();
    if(sz.width() > 400) {
        sz.setWidth(400);
    } else if(sz.width() < 160) {
        sz.setWidth(160);
    }
    if(sz.height() > 300) {
        sz.setHeight(300);
    } else if(sz.height() < 160) {
        sz.setHeight(160);
    }
    widgetPixmap = widgetPixmap.scaled(sz, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->pixmap_view->setPixmap(widgetPixmap);
}

bool QTempWidget::eventFilter(QObject *o, QEvent *e)
{
    if(o->isWidgetType() && e->type() == QEvent::Paint && o->parent() == NULL) {
        QWidget* wid = (QWidget*)o;
        QPainter p(wid);
        p.setBrush(QColor(220, 220, 220));
        p.setPen(QColor(128, 128, 128));
        p.drawRect(0, 0, wid->width() - 1, wid->height() - 1);
    }
    return QWidget::eventFilter(o, e);
}

void QTempWidget::on_exists_button_clicked()
{
    QString name = QFileDialog::getOpenFileName(this,
                   tr("Select File"),
                   QDir::currentPath(),
                   tr("Page File(*.xml)"));
    if(name != NULL) {
        emit add_file(name);
    }
}


void QTempWidget::on_okbtn_clicked()
{
    QString str = m_name_edit->value();
    if(str == "") {
        return;
    }
    QAbstractHost* h = m_info_to_host.value(m_current);
    if(h != NULL) {
        h->setPropertyValue("objectName", str);
        m_info_to_host.remove(m_current);
        emit ok(h);
    }
}

void QTempWidget::on_cancelbtn_clicked()
{
    emit cancel();
}

void QTempWidget::set_name_list(const QStringList &list)
{
    m_name_edit->setIntermediateList(list);
}
