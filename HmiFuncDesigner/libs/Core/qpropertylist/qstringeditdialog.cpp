#include "qstringeditdialog.h"
#include "ui_qstringeditdialog.h"


#include "../qsoftcore.h"
#include "../undocommand/qstringchangedundocommand.h"

#include "../../shared/property/qabstractproperty.h"
#include "../../shared/qprojectcore.h"
#include "../../shared/qlanguagemanager.h"
#include "../../shared/qlanguage.h"
#include "../../shared/host/qabstracthost.h"

#include <QLineEdit>
#include <QPainter>
#include <QStyleOptionViewItemV3>
#include <QStyle>
#include <QApplication>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QUuid>

QWidget *QLanguageDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
    QWidget *ret=NULL;
    if(index.column()==1)
    {
        ret=new QLineEdit(parent);
    }
    if(ret!=NULL)
    {
        ret->setProperty("no-ManhattanStyle",true);
    }
    return ret;
}

void QLanguageDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect.adjusted(0,0,0,0));
}

QSize QLanguageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size=QItemDelegate::sizeHint(option,index);
    size.setHeight(21);
    return size;
}

void QLanguageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV3 opt=option;

    QColor c=QColor(255,255,191);

    if(opt.features & QStyleOptionViewItemV2::Alternate)
    {
        c=c.lighter(112);
    }
    painter->fillRect(option.rect,c);
    opt.state &=~ QStyle::State_HasFocus;
    QItemDelegate::paint(painter,opt,index);
    opt.palette.setCurrentColorGroup(QPalette::Active);
    QColor color=static_cast<QRgb>(qApp->style()->styleHint(QStyle::SH_Table_GridLineColor,&opt));
    painter->setPen(color);

    int right=option.direction==Qt::LeftToRight?option.rect.right():option.rect.left();
    painter->drawLine(right,option.rect.y(),right,option.rect.bottom());

    painter->drawLine(option.rect.left(),option.rect.bottom(),option.rect.right(),option.rect.bottom());
}

QStringEditDialog::QStringEditDialog(QAbstractProperty *property,QUndoStack* stack, QWidget *parent):
    QDialog(parent),
    m_property(property),
    m_undo_stack(stack),
    ui(new Ui::QStringEditDialog)
{
    setProperty("panelwidget",true);
    ui->setupUi(this);

    ui->translateTree->setItemDelegate(new QLanguageDelegate);
    ui->translateTree->setMinimumHeight(0);
    ui->text->setVisible(false);

    ui->translateTree->setAlternatingRowColors(true);
    ui->translateTree->setEditTriggers(QAbstractItemView::EditKeyPressed);
    ui->translateTree->setRootIsDecorated(false);
    //ui->translateTree->header()->setMovable(false);

    QLanguageManager *manager=QSoftCore::getCore()->getProjectCore()->get_language_manager();

    QList<QLanguage*> language=manager->get_all_languages();
    foreach(QLanguage* l,language)
    {
        QTreeWidgetItem* item=new QTreeWidgetItem(ui->translateTree);
        item->setText(0,l->get_language_name());
        tagTranslateInfo *info=l->get_translate(m_property->get_property("uuid").toString());
        QString trs=(info!=NULL?info->m_translate:"");
        item->setText(1,trs);
        item->setToolTip(1,trs);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        m_items.insert(item,l);
    }
    ui->text->setText(m_property->get_value().toString());

    connect(ui->translateTree,SIGNAL(clicked(QModelIndex)),ui->translateTree,SLOT(edit(QModelIndex)));
    ui->enabled->setChecked(m_property->get_property("tr").toBool());
    on_enabled_clicked();
}

QStringEditDialog::~QStringEditDialog()
{
    delete ui;
}

void QStringEditDialog::on_enabled_clicked()
{
    bool tran=ui->enabled->isChecked();
    ui->translateTree->setVisible(tran);
    ui->text->setVisible(!tran);
    resize(this->width(),tran?400:100);
    setProperty("panelwidget",tran);
}

void QStringEditDialog::on_okBtn_clicked()
{
    bool new_tr=ui->enabled->isChecked();
    bool old_tr=m_property->get_property("tr").toBool();
    QMap<QString,QString> old_translate;
    QMap<QString,QString> new_translate;
    QString old_text;
    QString new_text;
    QString uuid=m_property->get_property("uuid").toString();
    if(uuid=="")
    {
        uuid=QUuid::createUuid().toString();
    }
    if(new_tr)
    {
        QMapIterator<QTreeWidgetItem*,QLanguage*>  it(m_items);
        while(it.hasNext())
        {
            it.next();
            new_translate.insert(it.value()->get_uuid(),it.key()->text(1));
        }
    }
    else
    {
        new_text=ui->text->text();
    }
    if(old_tr)
    {
        QList<QLanguage*>   lan=QSoftCore::getCore()->getProjectCore()->get_language_manager()->get_all_languages();
        foreach(QLanguage* l,lan)
        {
            tagTranslateInfo *info=l->get_translate(uuid);
            old_translate.insert(l->get_uuid(),info==NULL?"":info->m_translate);
        }
    }
    else
    {
        old_text=m_property->get_value().toString();
    }


    QStringChangedUndoCommand *cmd=new QStringChangedUndoCommand(m_property->get_host()->get_uuid(),
                                                                 m_property->get_property("name").toString(),
                                                                 uuid,
                                                                 old_tr,
                                                                 new_tr,
                                                                 old_text,
                                                                 new_text,
                                                                 old_translate,
                                                                 new_translate);
    m_undo_stack->push(cmd);

    close();
}

void QStringEditDialog::on_cancelBtn_clicked()
{
    close();
}
