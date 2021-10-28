#include "qcompletertreewidget.h"
#include "qhostobjectmap.h"
#include "../../../shared/host/qabstracthost.h"
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <QFontMetrics>
#include <QItemDelegate>
#include <QPainter>
#include <QScrollBar>
#include <QKeyEvent>
#include <QStringList>
#include <QMetaMethod>

#define FONT_SIZE 9

class QCompleterDelegate : public QItemDelegate
{
public :
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

void QCompleterDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    QColor c;
    c = QColor(255, 230, 151);
    if(opt.features & QStyleOptionViewItem::Alternate) {
        c = c.lighter(130);
    }

    painter->fillRect(option.rect, c);
    opt.state &= ~ QStyle::State_HasFocus;
    opt.font.setPointSize(FONT_SIZE);
    QItemDelegate::paint(painter, opt, index);
}

QSize QCompleterDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize sz = QItemDelegate::sizeHint(option, index);
    sz.setHeight(20);
    return sz;
}

QCompleterTreeWidget::QCompleterTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
    this->header()->hide();
    this->setRootIsDecorated(false);
    this->setAlternatingRowColors(true);
    this->setFocusProxy((QWidget*)parent->parent());
    this->setItemDelegate(new QCompleterDelegate);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QFont f = this->font();
    f.setPointSize(FONT_SIZE);
    parent->parent()->installEventFilter(this);

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(itemselect(QTreeWidgetItem*, int)));
}

void QCompleterTreeWidget::move_position(const QRect &re)
{
    int width = parentWidget()->width();
    int height = parentWidget()->height();

    int h = (this->sizeHintForRow(0) * qMin(10, this->model()->rowCount()));
    int w = this->sizeHintForColumn(0);
    if(model()->rowCount() > 10) {
        w += 20;
    } else {
        w += 4;
    }


    int x, y;
    x = re.left();
    y = re.bottom();
    if(width < re.left() + w) {
        x = width - w;
    }
    if(height < re.bottom() + h) {
        y = re.top() - h;
    }

    this->setGeometry(x, y, w, h + 2);
}

bool QCompleterTreeWidget::eventFilter(QObject *o, QEvent *e)
{
    if(!this->isVisible()) {
        return false;
    }

    if (o != parent()->parent()) {
        return QObject::eventFilter(o, e);
    }

    switch (e->type()) {
        case QEvent::KeyPress: {
            QKeyEvent *ke = static_cast<QKeyEvent *>(e);

            QModelIndex curIndex = this->currentIndex();
            QModelIndexList selList = this->selectionModel()->selectedIndexes();

            const int key = ke->key();
            if ((key == Qt::Key_Up || key == Qt::Key_Down) && selList.isEmpty() && curIndex.isValid()) {
                setCurrentIndex(curIndex);
                return true;
            }
            if(ke->text() == ".") {
                if(curIndex.isValid()) {
                    QTreeWidgetItem *item = itemFromIndex(curIndex);
                    itemselect(item, 0);
                }
                return true;
            }
            switch (key) {
                case Qt::Key_End:
                    if (ke->modifiers() & Qt::ControlModifier) {
                        return false;
                    } else {
                        QModelIndex lastIndex = model()->index(model()->rowCount() - 1, 0);
                        setCurrentIndex(lastIndex);
                        return true;
                    }
                case Qt::Key_Home:
                    if (ke->modifiers() & Qt::ControlModifier) {
                        return false;
                    } else {
                        QModelIndex firstIndex = model()->index(0, 0);
                        setCurrentIndex(firstIndex);
                        return true;
                    }
                case Qt::Key_Up:
                    if (!curIndex.isValid()) {
                        int rowCount = this->model()->rowCount();
                        QModelIndex lastIndex = model()->index(rowCount - 1, 0);
                        setCurrentIndex(lastIndex);
                        return true;
                    } else if (curIndex.row() == 0) {
                        setCurrentIndex(QModelIndex());
                        return true;
                    } else {
                        QModelIndex c = model()->index(curIndex.row() - 1, 0);
                        setCurrentIndex(c);
                        return true;
                    }
                case Qt::Key_Down:
                    if (!curIndex.isValid()) {
                        QModelIndex firstIndex = model()->index(0, 0);
                        setCurrentIndex(firstIndex);
                        return true;
                    } else if (curIndex.row() == model()->rowCount() - 1) {
                        setCurrentIndex(QModelIndex());
                        return true;
                    } else {
                        QModelIndex c = model()->index(curIndex.row() + 1, 0);
                        setCurrentIndex(c);
                        return true;
                    }

                case Qt::Key_PageUp:
                case Qt::Key_PageDown:
                    return false;
            }

            switch (key) {
                case Qt::Key_Return:
                case Qt::Key_Enter:
                case Qt::Key_Tab:
                    if (curIndex.isValid()) {
                        QTreeWidgetItem *item = this->itemFromIndex(curIndex);
                        itemselect(item, 0);
                    }
                    return true;
                case Qt::Key_Backtab:
                case Qt::Key_Escape:
                    hide();
                    return true;
                default:
                    return false;
            }
        }
        default:
            return false;
    }
    return false;
}

void QCompleterTreeWidget::itemselect(QTreeWidgetItem *item, int)
{
    hide();
    emit select(item->text(0));
}

void QCompleterTreeWidget::init(tagReturnList *info, const QString &text)
{
    m_returnInfo.m_returns = info->m_returns;
    m_returnInfo.m_name_to_return = info->m_name_to_return;
    m_text = text;
    this->clear();
    QTreeWidgetItem *item;
    foreach(const tagReturnInfo &ret, info->m_returns) {
        item = new QTreeWidgetItem(this);
        item->setText(0, ret.m_param);
        item->setIcon(0, QIcon(":/images/script_" + ret.m_type + ".png"));
    }
    setCurrentItem(this->topLevelItem(0));
}

tagReturnList* QCompleterTreeWidget::returnInfo()
{
    return &m_returnInfo;
}

QString QCompleterTreeWidget::get_source_text()
{
    return m_text;
}
