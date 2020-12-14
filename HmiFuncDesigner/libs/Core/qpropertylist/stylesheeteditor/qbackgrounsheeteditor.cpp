#include "qbackgrounsheeteditor.h"
#include "ui_qbackgrounsheeteditor.h"

#include "qresourceview.h"
#include "../../qsoftcore.h"
#include "../../qfilemanager.h"
#include "../../undocommand/qresourceaddundocommand.h"

#include "../../../shared/resource/qresourcemanager.h"
#include "../../../shared/resource/qresourcefile.h"
#include "../../../shared/qprojectcore.h"
#include "../../../shared/property/stylesheetitem/qabstractstylesheetitem.h"

#include <QColorDialog>
#include <QGradient>

QBackgrounSheetEditor::QBackgrounSheetEditor(QWidget *parent) :
    QBaseEditorWidget(parent),
    ui(new Ui::QBackgrounSheetEditor)
{
    ui->setupUi(this);

    ui->repeat_combo->addItems(QStringList()<<"repeat-x"<<"repeat-y"<<"repeat-xy"<<"no-repeat");
    ui->repeat_combo->setCurrentIndex(2);

    ui->position_combo->addItems(QStringList()<<"left top"<<"left center"<<"left bottom"<<"center top"<<"center"<<"center bottom"
                             <<"right top"<<"right center"<<"right bottom");
    ui->position_combo->setCurrentIndex(0);

    connect(ui->repeat_combo,SIGNAL(currentIndexChanged(int)),this,SLOT(repeat_changed()));
    connect(ui->position_combo,SIGNAL(currentIndexChanged(int)),this,SLOT(position_changed()));
}

QBackgrounSheetEditor::~QBackgrounSheetEditor()
{
    delete ui;
    if(m_tempResource!="")
    {
        QSoftCore::getCore()->getProjectCore()->get_resource_manager()->removeResource(m_tempResource);
    }
}

void QBackgrounSheetEditor::on_colorBtn_clicked()
{
    QColor c;
    QVariant v=m_item.m_attributes.value("color");
    bool valid=false;
    if(v.isValid())
    {
        QGradient g=v.value<QGradient>();
        QGradientStops s=g.stops();
        if(s.size()>0)
        {
            c=s.first().second;
            valid=true;
        }
        else
        {
            c=QColor(255,255,255);
        }
    }
    else
    {
        c=QColor(255,255,255);
    }
    QColor cc=QColorDialog::getColor(c,this);
    if(cc.isValid())
    {
        QGradient g=QLinearGradient(0,0,1,0);
        g.setColorAt(0,cc);
        g.setColorAt(1,cc);
        QVariant v;
        v.setValue<QGradient>(g);
        m_item.m_attributes.insert("color",v);
        emit changed();
    }
}

void QBackgrounSheetEditor::on_gradientBtn_clicked()
{
    QVariant v;
    v=m_item.m_attributes.value("color");
    QGradient g=get_gradient(v.value<QGradient>());
    if (g.type()!=QGradient::NoGradient)
    {
        v.setValue<QGradient>(g);
        m_item.m_attributes.insert("color",v);
        emit changed();
    }
}

void QBackgrounSheetEditor::on_imageBtn_clicked()
{
    QResourceView dlg(this);

    QString name=m_item.m_attributes.value("image").toString();

    tagResourceInfo *info=QSoftCore::getCore()->getProjectCore()->get_resource_manager()->get_file()->resource(name);
    if(info!=NULL)
    {
        dlg.set_file(info->m_resourceData);
    }
    dlg.exec();

    if(dlg.get_ret()==1)
    {
        tagFileInfo *info=dlg.get_file();
        if(info!=NULL)
        {
            tagFileGroupInfo *g=QSoftCore::getCore()->getFileManager()->get_group(info->m_group_uuid);
            QString s=g->m_group_name+"/"+info->m_file_name;
            if(s!=name)
            {
                QSoftCore::getCore()->getProjectCore()
                        ->get_resource_manager()->addResource(s,info->m_file_data);
                m_item.m_attributes.insert("image",s);
                if(m_tempResource!="")
                {
                    QSoftCore::getCore()->getProjectCore()->get_resource_manager()->removeResource(m_tempResource);
                }
                m_tempResource=s;
                emit changed();
            }
        }
    }
}

void QBackgrounSheetEditor::set_item(QAbstractStylesheetItem *item)
{
    QBaseEditorWidget::set_item(item);

    QString str;

    str=m_item.m_attributes.value("position","left top").toString();
    ui->position_combo->setCurrentIndex(ui->position_combo->findText(str));

    str=m_item.m_attributes.value("repeat","repeat-xy").toString();
    ui->repeat_combo->setCurrentIndex(ui->repeat_combo->findText(str));

    if(m_tempResource!="")
    {
        QSoftCore::getCore()->getProjectCore()->get_resource_manager()->removeResource(m_tempResource);
    }
}

void QBackgrounSheetEditor::repeat_changed()
{
    QString str=ui->repeat_combo->currentText();
    if(m_item.m_attributes.value("repeat").toString()!=str)
    {
        m_item.m_attributes.insert("repeat",str);
        emit changed();
    }
}

void QBackgrounSheetEditor::position_changed()
{
    QString str=ui->position_combo->currentText();
    if(m_item.m_attributes.value("position").toString()!=str)
    {
        m_item.m_attributes.insert("position",str);
        emit changed();
    }
}

void QBackgrounSheetEditor::take_resource(QUndoCommand *cmd)
{
    if(m_start_resource!=m_tempResource)
    {
        QResourceManager *manager=QSoftCore::getCore()->getProjectCore()->get_resource_manager();

        if(cmd!=NULL)
        {
            if(m_start_resource!="")
            {
                tagResourceInfo *info=manager->get_file()->resource(m_start_resource);
                if(info!=NULL)
                {
                    new QResourceAddUndoCommand(info->m_resourceData,m_start_resource,RAT_REMOVE,cmd);
                }
            }
        }
    }
}

void QBackgrounSheetEditor::add_resource(QUndoCommand *cmd)
{
    if(m_start_resource!=m_tempResource)
    {
        QResourceManager *manager=QSoftCore::getCore()->getProjectCore()->get_resource_manager();

        if(cmd!=NULL)
        {
            if(m_tempResource!="")
            {
                tagResourceInfo *info=manager->get_file()->resource(m_tempResource);
                if(info!=NULL)
                {
                    new QResourceAddUndoCommand(info->m_resourceData,m_tempResource,RAT_ADD,cmd);
                }
            }
        }
        if(m_tempResource!="")
        {
            manager->removeResource(m_tempResource);
        }
        m_tempResource="";
    }
}

void QBackgrounSheetEditor::init(QAbstractStylesheetItem *item)
{
    tagStylesheetItem temp=item->value().value<tagStylesheetItem>();

    m_start_resource=temp.m_attributes.value("image").toString();
}
