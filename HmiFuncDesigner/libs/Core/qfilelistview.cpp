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
    if(m_view->m_itemToFile.keys().contains(item)) {
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
    m_canRemove(can_remove),
    m_current(NULL),
    m_showPicture(show_picture),
    m_doubleClicked(double_clicked)
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

    connect(QSoftCore::getCore()->getFileManager(), SIGNAL(sigInsertGroup(tagFileGroupInfo*, int)),
            this, SLOT(insertGroup(tagFileGroupInfo*, int)));
    connect(QSoftCore::getCore()->getFileManager(), SIGNAL(sigRemoveGroup(tagFileGroupInfo*)),
            this, SLOT(removeGroup(tagFileGroupInfo*)));

    connect(QSoftCore::getCore()->getFileManager(), SIGNAL(sigInsertFile(tagFileGroupInfo*, tagFileInfo*, int)),
            this, SLOT(insertFile(tagFileGroupInfo*, tagFileInfo*, int)));
    connect(QSoftCore::getCore()->getFileManager(), SIGNAL(sigRemoveFile(tagFileGroupInfo*, tagFileInfo*)),
            this, SLOT(removeFile(tagFileGroupInfo*, tagFileInfo*)));

    if(m_doubleClicked) {
        connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(double_clicked(QTreeWidgetItem*)));
    } else {
        connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(double_clicked(QTreeWidgetItem*)));
    }
    init();
}

void QFileListView::init()
{
    QList<tagFileGroupInfo*>    groups = QSoftCore::getCore()->getFileManager()->getAllGroup();

    QFileListItemWidget *wid;

    QStringList list = QStringList() << "png" << "bmp" << "jpg";
    foreach(tagFileGroupInfo *group, groups) {
        QTreeWidgetItem *p = new QTreeWidgetItem(this);

        wid = new QFileListItemWidget(this, p);
        wid->setText(group->m_group_name);
        connect(wid, SIGNAL(remove()), this, SLOT(buttonClicked()));
        setItemWidget(p, 0, wid);
        p->setIcon(0, m_icon);
        p->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        p->setExpanded(true);
        m_itemToGroup.insert(p, group);
        m_groupToItem.insert(group, p);

        foreach(tagFileInfo *f, group->m_files) {
            if(m_showPicture && !list.contains(f->m_exp)) {
                continue;
            }
            QTreeWidgetItem *item = new QTreeWidgetItem(p);
            wid = new QFileListItemWidget(this, item);
            connect(wid, SIGNAL(remove()), this, SLOT(buttonClicked()));
            wid->setText(f->m_file_name);
            setItemWidget(item, 0, wid);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            m_itemToFile.insert(item, f);
            m_fileToItem.insert(f, item);
        }

        if(m_showPicture && p->childCount() == 0) {
            m_itemToGroup.remove(p);
            m_groupToItem.remove(group);
            delete p;
        }
    }
}

void QFileListView::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    QTreeWidgetItem *item = this->itemFromIndex(index);

    QColor c;
    QStyleOptionViewItem opt = options;
    if(m_itemToGroup.keys().contains(item)) {
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

tagFileInfo* QFileListView::getFile(QTreeWidgetItem *item)
{
    return m_itemToFile.value(item);
}

tagFileGroupInfo* QFileListView::getGroup(QTreeWidgetItem *item)
{
    return m_itemToGroup.value(item);
}

void QFileListView::insertGroup(tagFileGroupInfo *g, int index)
{
    QTreeWidgetItem *p = new QTreeWidgetItem;
    this->insertTopLevelItem(index, p);
    QFileListItemWidget *wid = new QFileListItemWidget(this, p);
    connect(wid, SIGNAL(remove()), this, SLOT(buttonClicked()));
    wid->setText(g->m_group_name);
    setItemWidget(p, 0, wid);
    p->setIcon(0, m_icon);
    p->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    p->setExpanded(true);
    m_itemToGroup.insert(p, g);
    m_groupToItem.insert(g, p);

    foreach(tagFileInfo *f, g->m_files) {
        QTreeWidgetItem *item = new QTreeWidgetItem(p);
        wid = new QFileListItemWidget(this, item);
        connect(wid, SIGNAL(remove()), this, SLOT(buttonClicked()));
        wid->setText(f->m_file_name);
        setItemWidget(item, 0, wid);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        m_itemToFile.insert(item, f);
        m_fileToItem.insert(f, item);
    }
}

void QFileListView::removeGroup(tagFileGroupInfo *g)
{
    foreach(tagFileInfo* info, g->m_files) {
        QTreeWidgetItem *item = m_fileToItem.value(info);
        if(item != NULL) {
            m_itemToFile.remove(item);
            m_fileToItem.remove(info);
        }
    }

    QTreeWidgetItem *item = m_groupToItem.value(g);
    if(item != NULL) {
        m_groupToItem.remove(g);
        m_itemToGroup.remove(item);
        delete item;
    }
    tagFileInfo *select = QSoftCore::getCore()->getFileManager()->getFile(m_selectUuid);
    if(select == NULL || select->m_group_uuid == g->m_uuid) {
        selectFirst(g, NULL);
    }
}

void QFileListView::insertFile(tagFileGroupInfo *g, tagFileInfo *f, int index)
{
    QTreeWidgetItem *p = m_groupToItem.value(g);
    if(p == NULL) {
        return;
    }
    p->setExpanded(true);
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    p->insertChild(index, item);
    QFileListItemWidget *wid = new QFileListItemWidget(this, item);
    connect(wid, SIGNAL(remove()), this, SLOT(buttonClicked()));
    wid->setText(f->m_file_name);
    setItemWidget(item, 0, wid);
    m_itemToFile.insert(item, f);
    m_fileToItem.insert(f, item);
    select(f);
}

void QFileListView::removeFile(tagFileGroupInfo *g, tagFileInfo *f)
{
    QTreeWidgetItem *item = m_fileToItem.value(f);
    if(item != NULL) {
        m_itemToFile.remove(item);
        m_fileToItem.remove(f);
        delete item;

        if(f->m_uuid == m_selectUuid) {
            m_current = NULL;
            selectFirst(g, f);
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

bool QFileListView::canRemove()
{
    return m_canRemove;
}

void QFileListView::buttonClicked()
{
    QFileListItemWidget* wid = (QFileListItemWidget*)sender();

    QMapIterator<tagFileInfo*, QTreeWidgetItem*>     it(m_fileToItem);
    while(it.hasNext()) {
        it.next();
        if(itemWidget(it.value(), 0) == wid) {
            emit sigRemoveFile(it.key());
            return;
        }
    }

    QMapIterator<tagFileGroupInfo*, QTreeWidgetItem*>     itt(m_groupToItem);
    while(itt.hasNext()) {
        itt.next();
        if(itemWidget(itt.value(), 0) == wid) {
            emit sigRemoveGroup(itt.key());
            return;
        }
    }
}

void QFileListView::doubleClicked(QTreeWidgetItem *item)
{
    tagFileInfo *file = m_itemToFile.value(item);
    if(file != NULL) {
        if(m_selectUuid != file->m_uuid) {
            select(file);
        }
    }
}

void QFileListView::selectFirst(tagFileGroupInfo *g, tagFileInfo *f)
{
    QList<tagFileGroupInfo*> list = QSoftCore::getCore()->getFileManager()->getAllGroup();
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
        m_selectUuid = f->m_uuid;
    } else {
        m_selectUuid = "";
    }
    if(m_current != NULL) {
        QFileListItemWidget *wid = (QFileListItemWidget*)itemWidget(m_current, 0);
        if(wid != NULL) {
            wid->setIcon("");
        }
    }
    m_current = m_fileToItem.value(f);
    if(m_current != NULL) {
        QFileListItemWidget *wid = (QFileListItemWidget*)itemWidget(m_current, 0);
        if(wid != NULL) {
            wid->setIcon(":/images/check.png");
        }
        setCurrentItem(m_current);
    }
    emit sigSelectFile(f);
}
