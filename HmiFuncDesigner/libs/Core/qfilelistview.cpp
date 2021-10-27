#include "qfilelistview.h"

#include "qfilemanager.h"
#include "qsoftcore.h"
#include "stylehelper.h"
#include "qfilelistitemwidget.h"

#include <QHeaderView>
#include <QItemDelegate>
#include <QPainter>
#include <QEvent>
#include <QApplication>
#include <QMouseEvent>

class QFileDelegate: public QItemDelegate
{
public :
    QFileDelegate(QFileListView* view): m_view(view) {}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
    QFileListView     *m_view;
};

void QFileDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    QColor c;
    QTreeWidgetItem *item = m_view->itemFromIndex(index);
    if(m_view->m_item_to_file.keys().contains(item)) {
        c = QColor(255, 255, 191);
        if(opt.features & QStyleOptionViewItem::Alternate) {
            c = c.lighter(112);
        }
    } else {
        c = opt.palette.color(QPalette::Dark);
    }

    painter->fillRect(option.rect, c);
    opt.state &= ~ QStyle::State_HasFocus;
    QItemDelegate::paint(painter, opt, index);
}

QSize QFileDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize sz = QItemDelegate::sizeHint(option, index);
    sz.setHeight(25);
    return sz;
}

QFileListView::QFileListView(bool can_remove, bool show_picture, bool double_clicked, QWidget *parent):
    QTreeWidget(parent),
    m_can_remove(can_remove),
    m_current(NULL),
    m_show_picture(show_picture),
    m_double_clicked(double_clicked)
{
    this->setFrameStyle(QFrame::NoFrame);
    this->header()->hide();
    this->setAlternatingRowColors(true);
    this->setItemDelegate(new QFileDelegate(this));
    this->setRootIsDecorated(false);
    m_icon = StyleHelper::drawIndicatorIcon(this->palette(), this->style());
    this->setMinimumSize(100, 100);

    QPalette p = palette();
    p.setColor(QPalette::Inactive, QPalette::Highlight, p.color(QPalette::Active, QPalette::Highlight));
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Active, QPalette::HighlightedText));
    setPalette(p);

    connect(QSoftCore::getCore()->getFileManager(), SIGNAL(insert_group_signal(tagFileGroupInfo*, int)),
            this, SLOT(insert_group(tagFileGroupInfo*, int)));
    connect(QSoftCore::getCore()->getFileManager(), SIGNAL(remove_group_signal(tagFileGroupInfo*)),
            this, SLOT(remove_group(tagFileGroupInfo*)));

    connect(QSoftCore::getCore()->getFileManager(), SIGNAL(insert_file_signal(tagFileGroupInfo*, tagFileInfo*, int)),
            this, SLOT(insert_file(tagFileGroupInfo*, tagFileInfo*, int)));
    connect(QSoftCore::getCore()->getFileManager(), SIGNAL(remove_file_signal(tagFileGroupInfo*, tagFileInfo*)),
            this, SLOT(remove_file(tagFileGroupInfo*, tagFileInfo*)));

    if(m_double_clicked) {
        connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(double_clicked(QTreeWidgetItem*)));
    } else {
        connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(double_clicked(QTreeWidgetItem*)));
    }
    init();
}

void QFileListView::init()
{
    QList<tagFileGroupInfo*>    groups = QSoftCore::getCore()->getFileManager()->get_all_group();

    QFileListItemWidget *wid;

    QStringList list = QStringList() << "png" << "bmp" << "jpg";
    foreach(tagFileGroupInfo *group, groups) {
        QTreeWidgetItem *p = new QTreeWidgetItem(this);

        wid = new QFileListItemWidget(this, p);
        wid->set_text(group->m_group_name);
        connect(wid, SIGNAL(remove()), this, SLOT(button_clicked()));
        setItemWidget(p, 0, wid);
        p->setIcon(0, m_icon);
        p->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        p->setExpanded(true);
        m_item_to_group.insert(p, group);
        m_group_to_item.insert(group, p);

        foreach(tagFileInfo *f, group->m_files) {
            if(m_show_picture && !list.contains(f->m_exp)) {
                continue;
            }
            QTreeWidgetItem *item = new QTreeWidgetItem(p);
            wid = new QFileListItemWidget(this, item);
            connect(wid, SIGNAL(remove()), this, SLOT(button_clicked()));
            wid->set_text(f->m_file_name);
            setItemWidget(item, 0, wid);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            m_item_to_file.insert(item, f);
            m_file_to_item.insert(f, item);
        }

        if(m_show_picture && p->childCount() == 0) {
            m_item_to_group.remove(p);
            m_group_to_item.remove(group);
            delete p;
        }
    }
}

void QFileListView::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    QTreeWidgetItem *item = this->itemFromIndex(index);

    QColor c;
    QStyleOptionViewItem opt = options;
    if(m_item_to_group.keys().contains(item)) {
        c = opt.palette.color(QPalette::Dark);
    } else {
        c = QColor(255, 255, 191);
    }


    painter->fillRect(options.rect, c);
    opt.palette.setColor(QPalette::AlternateBase, c.lighter(112));
    QTreeWidget::drawRow(painter, opt, index);

    QColor color = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor));
    painter->setPen(color);
    painter->drawLine(opt.rect.x(), opt.rect.bottom(), opt.rect.right(), opt.rect.bottom());
}

tagFileInfo* QFileListView::get_file(QTreeWidgetItem *item)
{
    return m_item_to_file.value(item);
}

tagFileGroupInfo* QFileListView::get_group(QTreeWidgetItem *item)
{
    return m_item_to_group.value(item);
}

void QFileListView::insert_group(tagFileGroupInfo *g, int index)
{
    QTreeWidgetItem *p = new QTreeWidgetItem;
    this->insertTopLevelItem(index, p);
    QFileListItemWidget *wid = new QFileListItemWidget(this, p);
    connect(wid, SIGNAL(remove()), this, SLOT(button_clicked()));
    wid->set_text(g->m_group_name);
    setItemWidget(p, 0, wid);
    p->setIcon(0, m_icon);
    p->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    p->setExpanded(true);
    m_item_to_group.insert(p, g);
    m_group_to_item.insert(g, p);

    foreach(tagFileInfo *f, g->m_files) {
        QTreeWidgetItem *item = new QTreeWidgetItem(p);
        wid = new QFileListItemWidget(this, item);
        connect(wid, SIGNAL(remove()), this, SLOT(button_clicked()));
        wid->set_text(f->m_file_name);
        setItemWidget(item, 0, wid);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        m_item_to_file.insert(item, f);
        m_file_to_item.insert(f, item);
    }
}

void QFileListView::remove_group(tagFileGroupInfo *g)
{
    foreach(tagFileInfo* info, g->m_files) {
        QTreeWidgetItem *item = m_file_to_item.value(info);
        if(item != NULL) {
            m_item_to_file.remove(item);
            m_file_to_item.remove(info);
        }
    }

    QTreeWidgetItem *item = m_group_to_item.value(g);
    if(item != NULL) {
        m_group_to_item.remove(g);
        m_item_to_group.remove(item);
        delete item;
    }
    tagFileInfo *select = QSoftCore::getCore()->getFileManager()->get_file(m_select_uuid);
    if(select == NULL || select->m_group_uuid == g->m_uuid) {
        select_first(g, NULL);
    }
}

void QFileListView::insert_file(tagFileGroupInfo *g, tagFileInfo *f, int index)
{
    QTreeWidgetItem *p = m_group_to_item.value(g);
    if(p == NULL) {
        return;
    }
    p->setExpanded(true);
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    p->insertChild(index, item);
    QFileListItemWidget *wid = new QFileListItemWidget(this, item);
    connect(wid, SIGNAL(remove()), this, SLOT(button_clicked()));
    wid->set_text(f->m_file_name);
    setItemWidget(item, 0, wid);
    m_item_to_file.insert(item, f);
    m_file_to_item.insert(f, item);
    select(f);
}

void QFileListView::remove_file(tagFileGroupInfo *g, tagFileInfo *f)
{
    QTreeWidgetItem *item = m_file_to_item.value(f);
    if(item != NULL) {
        m_item_to_file.remove(item);
        m_file_to_item.remove(f);
        delete item;

        if(f->m_uuid == m_select_uuid) {
            m_current = NULL;
            select_first(g, f);
        }
    }
}

void QFileListView::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton && event->pos().x() < 20) {
        QTreeWidgetItem *item = itemAt(event->pos());
        if(item != NULL) {
            item->setExpanded(!item->isExpanded());
        }
    }
}

bool QFileListView::can_remove()
{
    return m_can_remove;
}

void QFileListView::button_clicked()
{
    QFileListItemWidget* wid = (QFileListItemWidget*)sender();

    QMapIterator<tagFileInfo*, QTreeWidgetItem*>     it(m_file_to_item);
    while(it.hasNext()) {
        it.next();
        if(itemWidget(it.value(), 0) == wid) {
            emit remove_file_signal(it.key());
            return;
        }
    }

    QMapIterator<tagFileGroupInfo*, QTreeWidgetItem*>     itt(m_group_to_item);
    while(itt.hasNext()) {
        itt.next();
        if(itemWidget(itt.value(), 0) == wid) {
            emit remove_group_signal(itt.key());
            return;
        }
    }
}

void QFileListView::double_clicked(QTreeWidgetItem *item)
{
    tagFileInfo *file = m_item_to_file.value(item);
    if(file != NULL) {
        if(m_select_uuid != file->m_uuid) {
            select(file);
        }
    }
}

void QFileListView::select_first(tagFileGroupInfo *g, tagFileInfo *f)
{
    QList<tagFileGroupInfo*> list = QSoftCore::getCore()->getFileManager()->get_all_group();
    if(f == NULL) {
        list.removeAll(g);
    }
    foreach(tagFileGroupInfo* group, list) {
        if(group->m_files.size() > 0) {
            if(f == NULL) {
                select(group->m_files.first());
                return;
            } else {
                if(group->m_files.first() != f) {
                    select(group->m_files.first());
                    return;
                } else {
                    if(group->m_files.size() > 1) {
                        select(group->m_files.at(1));
                        return;
                    }
                }
            }
        }
    }
    select(NULL);
}

void QFileListView::select(tagFileInfo *f)
{
    if(f != NULL) {
        m_select_uuid = f->m_uuid;
    } else {
        m_select_uuid = "";
    }
    if(m_current != NULL) {
        QFileListItemWidget *wid = (QFileListItemWidget*)itemWidget(m_current, 0);
        if(wid != NULL) {
            wid->set_icon("");
        }
    }
    m_current = m_file_to_item.value(f);
    if(m_current != NULL) {
        QFileListItemWidget *wid = (QFileListItemWidget*)itemWidget(m_current, 0);
        if(wid != NULL) {
            wid->set_icon(":/images/check.png");
        }
        setCurrentItem(m_current);
    }
    emit select_file_signal(f);
}
