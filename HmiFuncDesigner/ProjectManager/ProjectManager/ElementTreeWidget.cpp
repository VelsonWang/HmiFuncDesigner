#include "ElementTreeWidget.h"
#include "PluginManager.h"
#include "StyleHelper.h"
#include <QMimeData>
#include <QDrag>
#include <QApplication>
#include <QHeaderView>
#include <QItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QPalette>
#include <QMouseEvent>
#include <QDebug>

class QWidgetDelegate : public QItemDelegate
{
public :
    QWidgetDelegate(ElementTreeWidget* view) : m_view(view){}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
    ElementTreeWidget *m_view;
};

void QWidgetDelegate::paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const {
    QStyleOptionViewItemV3 opt = option;
    QColor c;
    QTreeWidgetItem *item = m_view->itemFromIndex(index);
    if(m_view->m_itemToInfo.keys().contains(item)) {
        c = QColor(255, 240, 191);
        if(opt.features & QStyleOptionViewItemV2::Alternate) {
            c = c.lighter(112);
        }
    } else {
        c=opt.palette.color(QPalette::Dark);
    }

    painter->fillRect(option.rect,c);
    opt.state &= ~QStyle::State_HasFocus;
    QItemDelegate::paint(painter,opt,index);
}

QSize QWidgetDelegate::sizeHint(const QStyleOptionViewItem &option,
                                const QModelIndex &index) const {
    QSize sz = QItemDelegate::sizeHint(option,index);
    sz.setHeight(25);
    return sz;
}

////////////////////////////////////////////////////////////////////////////////

ElementTreeWidget::ElementTreeWidget(QTreeWidget *parent)
    : QTreeWidget(parent) {
    setFocusPolicy(Qt::NoFocus);
    setIndentation(0);
    setRootIsDecorated(false);
    setColumnCount(1);
    header()->hide();
    header()->setSectionResizeMode(QHeaderView::Stretch);
    setTextElideMode(Qt::ElideMiddle);
    setVerticalScrollMode(ScrollPerPixel);
    setFrameStyle(QFrame::NoFrame);
    setAcceptDrops(true);
    setAlternatingRowColors(true);
    setIconSize(QSize(24,24));
    setItemDelegate(new QWidgetDelegate(this));
    m_expandIcon = StyleHelper::drawIndicatorIcon(this->palette(), this->style());
    this->expandAll();
}

ElementTreeWidget::~ElementTreeWidget() {

    QList<TItemInfo*> listItemInfo = m_infoToItem.keys();
    m_infoToItem.clear();
    m_itemToInfo.clear();
    m_nameToGroup.clear();
    qDeleteAll(listItemInfo);
    listItemInfo.clear();
}

void ElementTreeWidget::addElements(const QString &szGroupName) {
    QMap<QString, IDrawApplicationPlugin*> pluginMap = PluginManager::getInstance()->getPluginByType(szGroupName);
    QMapIterator<QString, IDrawApplicationPlugin*> iter(pluginMap);
    while(iter.hasNext()) {
        iter.next();
        IDrawApplicationPlugin* plugin = iter.value();
        if(plugin != Q_NULLPTR) {
            TItemInfo *pInfoObj = new TItemInfo();
            pInfoObj->showGroup = szGroupName;
            pInfoObj->showIcon = plugin->getElementIcon();
            pInfoObj->showName = plugin->getElementName();
            addWidget(pInfoObj);
        }
    }
}

void ElementTreeWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidgetItem *pItemObj = itemAt(event->pos());
    if(pItemObj) {
        if(m_nameToGroup.values().contains(pItemObj)) {
            if(event->pos().x()+header()->offset()<20) {
                pItemObj->setExpanded(!pItemObj->isExpanded());
            }
        } else if(m_itemToInfo.keys().contains(pItemObj)) {
            if(event->button()!=Qt::LeftButton) {
                return;
            }
            TItemInfo *info = m_itemToInfo.value(pItemObj);
            if(info != Q_NULLPTR) {
                startPos = event->pos();
            }
        }
    }

    QTreeWidget::mousePressEvent(event);
}

void ElementTreeWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
         int distance = (event->pos() - startPos).manhattanLength();
         if (distance >= QApplication::startDragDistance())
             startDrag();
     }
     QTreeWidget::mouseMoveEvent(event);
}

void ElementTreeWidget::startDrag() {
    QTreeWidgetItem *pItemObj = currentItem();
    if (pItemObj) {
        TItemInfo *info = m_itemToInfo.value(pItemObj);
        if(info != Q_NULLPTR) {
            QMimeData *mimeData = new QMimeData;
            mimeData->setText(info->showName);
            QDrag *drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(QPixmap(pItemObj->icon(0).pixmap(50,50)));
            drag->start(Qt::MoveAction);
        }
    }
}

void ElementTreeWidget::drawRow(QPainter *painter,
                                const QStyleOptionViewItem &options,
                                const QModelIndex &index) const{
    QTreeWidgetItem *item = itemFromIndex(index);

    QColor c;
    QStyleOptionViewItem opt = options;
    if(m_nameToGroup.values().contains(item)) {
        c = opt.palette.color(QPalette::Dark);
    } else {
        c = QColor(255,240,191);
    }

    painter->fillRect(options.rect, c);
    opt.palette.setColor(QPalette::AlternateBase, c.lighter(112));
    QTreeWidget::drawRow(painter, opt, index);
}



void ElementTreeWidget::addWidget(TItemInfo *info) {
    if(info->showGroup == "" || info->showName == "") return;
    QTreeWidgetItem *pItemObj = m_nameToGroup.value(info->showGroup);
    QTreeWidgetItem *item;
    if(pItemObj == Q_NULLPTR) {
        pItemObj = new QTreeWidgetItem(this);
        pItemObj->setText(0, info->showGroup);
        pItemObj->setFlags(Qt::ItemIsEnabled);
        pItemObj->setIcon(0, m_expandIcon);
        m_nameToGroup.insert(info->showGroup, pItemObj);
    }

    item = new QTreeWidgetItem(pItemObj);
    item->setText(0, info->showName);
    item->setIcon(0, info->showIcon);
    item->setFlags(Qt::ItemIsEnabled);
    m_infoToItem.insert(info, item);
    m_itemToInfo.insert(item, info);
}

