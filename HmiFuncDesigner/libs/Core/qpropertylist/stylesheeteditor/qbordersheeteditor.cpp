#include "qbordersheeteditor.h"
#include "ui_qbordersheeteditor.h"
#include "qresourceview.h"
#include "../../qsoftcore.h"
#include "../../qfilemanager.h"
#include "../../../gradienteditor/qtgradientdialog.h"
#include "../../../shared/qprojectcore.h"
#include "../../../shared/property/stylesheetitem/qabstractstylesheetitem.h"

#include <QColorDialog>
#include <QPainter>

QBorderSheetEditor::QBorderSheetEditor(QWidget *parent) :
    QBaseEditorWidget(parent),
    ui(new Ui::QBorderSheetEditor)
{
    ui->setupUi(this);

    QAction *ac;

    ac=new QAction(tr("Gradient"),this);
    connect(ac,SIGNAL(triggered()),this,SLOT(left_gradient()));
    ui->left_color_button->addAction(ac);
    connect(ui->left_color_button,SIGNAL(clicked()),this,SLOT(left_color()));

    ac=new QAction(tr("Gradient"),this);
    connect(ac,SIGNAL(triggered()),this,SLOT(top_gradient()));
    ui->top_color_button->addAction(ac);
    connect(ui->top_color_button,SIGNAL(clicked()),this,SLOT(top_color()));

    ac=new QAction(tr("Gradient"),this);
    connect(ac,SIGNAL(triggered()),this,SLOT(right_gradient()));
    ui->right_color_button->addAction(ac);
    connect(ui->right_color_button,SIGNAL(clicked()),this,SLOT(right_color()));

    ac=new QAction(tr("Gradient"),this);
    connect(ac,SIGNAL(triggered()),this,SLOT(bottom_gradient()));
    ui->bottom_color_button->addAction(ac);
    connect(ui->bottom_color_button,SIGNAL(clicked()),this,SLOT(bottom_color()));

    QStringList list;
    list<<"dashed"<<"dot-dash"<<"dot-dot-dash"<<"dotted"<<"double"
       <<"solid"<<"none";
    ui->left_style->addItems(list);
    ui->top_style->addItems(list);
    ui->right_style->addItems(list);
    ui->bottom_style->addItems(list);


    ui->left_style->setCurrentIndex(6);
    ui->top_style->setCurrentIndex(6);
    ui->right_style->setCurrentIndex(6);
    ui->bottom_style->setCurrentIndex(6);
}

QBorderSheetEditor::~QBorderSheetEditor()
{
    delete ui;
}
void QBorderSheetEditor::init(QAbstractStylesheetItem *item)
{
    tagStylesheetItem temp=item->value().value<tagStylesheetItem>();

    m_start_resource=temp.m_attributes.value("image").toString();
}

void QBorderSheetEditor::take_resource(QUndoCommand *cmd)
{
    if(m_start_resource!=m_tempResource)
    {
        if(cmd!=Q_NULLPTR)
        {
            if(m_start_resource!="")
            {
            }
        }
    }
}

void QBorderSheetEditor::add_resource(QUndoCommand *cmd)
{
    if(m_start_resource!=m_tempResource)
    {
        if(cmd!=Q_NULLPTR)
        {
            if(m_tempResource!="")
            {
            }
        }
        if(m_tempResource!="")
        {
        }
        m_tempResource="";
    }
}

void QBorderSheetEditor::set_item(QAbstractStylesheetItem *item)
{
    QBaseEditorWidget::set_item(item);

    QVariant value;

    QGradient g;

    value=m_item.m_attributes.value("left_color");
    g=value.value<QGradient>();
    ui->left_color_button->setIcon(get_icon(g));

    value=m_item.m_attributes.value("top_color");
    g=value.value<QGradient>();
    ui->top_color_button->setIcon(get_icon(g));

    value=m_item.m_attributes.value("right_color");
    g=value.value<QGradient>();
    ui->right_color_button->setIcon(get_icon(g));

    value=m_item.m_attributes.value("bottom_color");
    g=value.value<QGradient>();
    ui->bottom_color_button->setIcon(get_icon(g));

    ui->same_color->setChecked(m_item.m_attributes.value("color_same","true").toBool());
    if(ui->same_color->isChecked())
    {
        same_color();
    }

    disconnect(ui->left_style,SIGNAL(currentIndexChanged(int)),this,SLOT(left_style_currentIndexChanged(int)));
    disconnect(ui->right_style,SIGNAL(currentIndexChanged(int)),this,SLOT(right_style_currentIndexChanged(int)));
    disconnect(ui->bottom_style,SIGNAL(currentIndexChanged(int)),this,SLOT(bottom_style_currentIndexChanged(int)));
    disconnect(ui->top_style,SIGNAL(currentIndexChanged(int)),this,SLOT(top_style_currentIndexChanged(int)));

    disconnect(ui->left_width,SIGNAL(valueChanged(int)),this,SLOT(left_width_valueChanged(int)));
    disconnect(ui->right_width,SIGNAL(valueChanged(int)),this,SLOT(right_width_valueChanged(int)));
    disconnect(ui->bottom_width,SIGNAL(valueChanged(int)),this,SLOT(bottom_width_valueChanged(int)));
    disconnect(ui->top_width,SIGNAL(valueChanged(int)),this,SLOT(top_width_valueChanged(int)));


    value=m_item.m_attributes.value("left_style");
    ui->left_style->setCurrentIndex(ui->left_style->findText(value.toString()));

    ui->same_style->setChecked(m_item.m_attributes.value("style_same","true").toBool());
    if(ui->same_style->isChecked())
    {
        ui->right_style->setCurrentIndex(ui->right_style->findText(value.toString()));
        ui->bottom_style->setCurrentIndex(ui->bottom_style->findText(value.toString()));
        ui->top_style->setCurrentIndex(ui->top_style->findText(value.toString()));
    }
    else
    {
        value=m_item.m_attributes.value("right_style");
        ui->right_style->setCurrentIndex(ui->right_style->findText(value.toString()));
        value=m_item.m_attributes.value("bottom_style");
        ui->bottom_style->setCurrentIndex(ui->bottom_style->findText(value.toString()));
        value=m_item.m_attributes.value("top_style");
        ui->top_style->setCurrentIndex(ui->top_style->findText(value.toString()));
    }

    value=m_item.m_attributes.value("left_width");
    ui->left_width->setValue(value.toInt());

    ui->same_width->setChecked(m_item.m_attributes.value("width_same","true").toBool());
    if(ui->same_width->isChecked())
    {
        ui->right_width->setValue(value.toInt());
        ui->bottom_width->setValue(value.toInt());
        ui->top_width->setValue(value.toInt());
    }
    else
    {
        value=m_item.m_attributes.value("right_width");
        ui->right_width->setValue(value.toInt());
        value=m_item.m_attributes.value("bottom_width");
        ui->bottom_width->setValue(value.toInt());
        value=m_item.m_attributes.value("top_width");
        ui->top_width->setValue(value.toInt());
    }

    connect(ui->left_style,SIGNAL(currentIndexChanged(int)),this,SLOT(left_style_currentIndexChanged(int)));
    connect(ui->right_style,SIGNAL(currentIndexChanged(int)),this,SLOT(right_style_currentIndexChanged(int)));
    connect(ui->bottom_style,SIGNAL(currentIndexChanged(int)),this,SLOT(bottom_style_currentIndexChanged(int)));
    connect(ui->top_style,SIGNAL(currentIndexChanged(int)),this,SLOT(top_style_currentIndexChanged(int)));

    connect(ui->left_width,SIGNAL(valueChanged(int)),this,SLOT(left_width_valueChanged(int)));
    connect(ui->right_width,SIGNAL(valueChanged(int)),this,SLOT(right_width_valueChanged(int)));
    connect(ui->bottom_width,SIGNAL(valueChanged(int)),this,SLOT(bottom_width_valueChanged(int)));
    connect(ui->top_width,SIGNAL(valueChanged(int)),this,SLOT(top_width_valueChanged(int)));

}

void QBorderSheetEditor::left_color()
{
    QGradient g=m_item.m_attributes.value("left_color").value<QGradient>();
    QColor c;
    if(g.type()==QGradient::NoGradient)
    {
        c=QColor(255,255,255);
    }
    else
    {
        c=g.stops().at(0).second;
    }
    QGradient ret=get_color(c);
    if(ret.type()!=QGradient::NoGradient && g!=ret)
    {
        QVariant v;
        v.setValue<QGradient>(ret);
        m_item.m_attributes.insert("left_color",v);
        ui->left_color_button->setIcon(get_icon(ret));
        if(ui->same_color->isChecked())
        {
            same_color();
        }
        else
        {
            emit changed();
        }
    }
}

void QBorderSheetEditor::left_gradient()
{
    QGradient g=m_item.m_attributes.value("left_color").value<QGradient>();
    QGradient ret=get_gradient(g);
    if(ret.type()!=QGradient::NoGradient && g!=ret)
    {
        QVariant v;
        v.setValue<QGradient>(ret);
        m_item.m_attributes.insert("left_color",v);
        ui->left_color_button->setIcon(get_icon(ret));
        if(ui->same_color->isChecked())
        {
            same_color();
        }
        else
        {
            emit changed();
        }
    }
}

void QBorderSheetEditor::top_color()
{
    QGradient g=m_item.m_attributes.value("top_color").value<QGradient>();
    QColor c;
    if(g.type()==QGradient::NoGradient)
    {
        c=QColor(255,255,255);
    }
    else
    {
        c=g.stops().at(0).second;
    }
    QGradient ret=get_color(c);
    if(ret.type()!=QGradient::NoGradient && g!=ret)
    {
        QVariant v;
        v.setValue<QGradient>(ret);
        m_item.m_attributes.insert("top_color",v);
        ui->top_color_button->setIcon(get_icon(ret));
        emit changed();
    }
}

void QBorderSheetEditor::top_gradient()
{
    QGradient g=m_item.m_attributes.value("top_color").value<QGradient>();
    QGradient ret=get_gradient(g);
    if(ret.type()!=QGradient::NoGradient && g!=ret)
    {
        QVariant v;
        v.setValue<QGradient>(ret);
        m_item.m_attributes.insert("top_color",v);
        ui->top_color_button->setIcon(get_icon(ret));
        emit changed();
    }
}

void QBorderSheetEditor::right_color()
{
    QGradient g=m_item.m_attributes.value("right_color").value<QGradient>();
    QColor c;
    if(g.type()==QGradient::NoGradient)
    {
        c=QColor(255,255,255);
    }
    else
    {
        c=g.stops().at(0).second;
    }
    QGradient ret=get_color(c);
    if(ret.type()!=QGradient::NoGradient && g!=ret)
    {
        QVariant v;
        v.setValue<QGradient>(ret);
        m_item.m_attributes.insert("right_color",v);
        ui->right_color_button->setIcon(get_icon(ret));
        emit changed();
    }
}

void QBorderSheetEditor::right_gradient()
{
    QGradient g=m_item.m_attributes.value("right_color").value<QGradient>();
    QGradient ret=get_gradient(g);
    if(ret.type()!=QGradient::NoGradient && g!=ret)
    {
        QVariant v;
        v.setValue<QGradient>(ret);
        m_item.m_attributes.insert("right_color",v);
        ui->right_color_button->setIcon(get_icon(ret));
        emit changed();
    }
}

void QBorderSheetEditor::bottom_color()
{
    QGradient g=m_item.m_attributes.value("bottom_color").value<QGradient>();
    QColor c;
    if(g.type()==QGradient::NoGradient)
    {
        c=QColor(255,255,255);
    }
    else
    {
        c=g.stops().at(0).second;
    }
    QGradient ret=get_color(c);
    if(ret.type()!=QGradient::NoGradient && g!=ret)
    {
        QVariant v;
        v.setValue<QGradient>(ret);
        m_item.m_attributes.insert("bottom_color",v);
        ui->bottom_color_button->setIcon(get_icon(ret));
        emit changed();
    }
}

void QBorderSheetEditor::bottom_gradient()
{
    QGradient g=m_item.m_attributes.value("bottom_color").value<QGradient>();
    QGradient ret=get_gradient(g);
    if(ret.type()!=QGradient::NoGradient && g!=ret)
    {
        QVariant v;
        v.setValue<QGradient>(ret);
        m_item.m_attributes.insert("bottom_color",v);
        ui->bottom_color_button->setIcon(get_icon(ret));
        emit changed();
    }
}

void QBorderSheetEditor::on_same_color_clicked()
{
    m_item.m_attributes.insert("color_same",ui->same_color->isChecked());
    ui->right_color_button->setEnabled(!ui->same_color->isChecked());
    ui->top_color_button->setEnabled(!ui->same_color->isChecked());
    ui->bottom_color_button->setEnabled(!ui->same_color->isChecked());

    if(ui->same_color->isChecked())
    {
        same_color();
        emit changed();
    }
}

void QBorderSheetEditor::same_color()
{
    m_item.m_attributes.insert("right_color",m_item.m_attributes.value("left_color"));
    m_item.m_attributes.insert("top_color",m_item.m_attributes.value("left_color"));
    m_item.m_attributes.insert("bottom_color",m_item.m_attributes.value("left_color"));

    QGradient g=m_item.m_attributes.value("left_color").value<QGradient>();
    QIcon icon=get_icon(g);

    ui->top_color_button->setIcon(icon);
    ui->right_color_button->setIcon(icon);
    ui->bottom_color_button->setIcon(icon);
    emit changed();
}

void QBorderSheetEditor::on_image_clicked()
{
    QResourceView dlg(this);

    QString name=m_item.m_attributes.value("image").toString();
    //dlg.set_file(info->m_resourceData);
    dlg.exec();

    if(dlg.get_ret()==1)
    {
        tagFileInfo *info=dlg.get_file();
        if(info!=Q_NULLPTR)
        {
            tagFileGroupInfo *g=QSoftCore::getCore()->getFileManager()->get_group(info->m_group_uuid);
            QString s=g->m_group_name+"/"+info->m_file_name;
            if(s!=name)
            {
                m_item.m_attributes.insert("image",s);
                if(m_tempResource!="")
                {

                }
                m_tempResource=s;
                emit changed();
            }
        }
    }
}

void QBorderSheetEditor::left_width_valueChanged(int arg1)
{
    m_item.m_attributes.insert("left_width",QString::number(arg1));
    if(ui->same_width->isChecked())
    {
        ui->top_width->setValue(arg1);
        ui->right_width->setValue(arg1);
        ui->bottom_width->setValue(arg1);
    }
    else
    {
        emit changed();
    }
}

void QBorderSheetEditor::top_width_valueChanged(int arg1)
{
    m_item.m_attributes.insert("top_width",QString::number(arg1));
    emit changed();
}

void QBorderSheetEditor::right_width_valueChanged(int arg1)
{
    m_item.m_attributes.insert("right_width",QString::number(arg1));
    emit changed();
}

void QBorderSheetEditor::bottom_width_valueChanged(int arg1)
{
    m_item.m_attributes.insert("bottom_width",QString::number(arg1));
    emit changed();
}

void QBorderSheetEditor::on_same_width_clicked()
{
    ui->top_width->setEnabled(!ui->same_width->isChecked());
    ui->right_width->setEnabled(!ui->same_width->isChecked());
    ui->bottom_width->setEnabled(!ui->same_width->isChecked());
    if(ui->same_width->isChecked())
    {
        int value=ui->left_width->value();
        ui->top_width->setValue(value);
        ui->right_width->setValue(value);
        ui->bottom_width->setValue(value);
    }
}

void QBorderSheetEditor::left_style_currentIndexChanged(int index)
{
    m_item.m_attributes.insert("left_style",ui->left_style->currentText());
    if(ui->same_width->isChecked())
    {
        ui->top_style->setCurrentIndex(index);
        ui->right_style->setCurrentIndex(index);
        ui->bottom_style->setCurrentIndex(index);
    }
    else
    {
        emit changed();
    }
}

void QBorderSheetEditor::top_style_currentIndexChanged(int index)
{
    m_item.m_attributes.insert("top_style",ui->left_style->currentText());
    emit changed();
}

void QBorderSheetEditor::right_style_currentIndexChanged(int index)
{
    m_item.m_attributes.insert("right_style",ui->left_style->currentText());
    emit changed();
}

void QBorderSheetEditor::bottom_style_currentIndexChanged(int index)
{
    m_item.m_attributes.insert("bottom_style",ui->left_style->currentText());
    emit changed();
}

void QBorderSheetEditor::on_same_style_clicked()
{
    ui->top_style->setEnabled(!ui->same_style->isChecked());
    ui->right_style->setEnabled(!ui->same_style->isChecked());
    ui->bottom_style->setEnabled(!ui->same_style->isChecked());
    if(ui->same_width->isChecked())
    {
        int index=ui->left_style->currentIndex();
        ui->top_style->setCurrentIndex(index);
        ui->right_style->setCurrentIndex(index);
        ui->bottom_style->setCurrentIndex(index);
        emit changed();
    }
}
