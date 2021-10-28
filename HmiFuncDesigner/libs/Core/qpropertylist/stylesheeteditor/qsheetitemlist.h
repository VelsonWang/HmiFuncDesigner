#ifndef QSHEETITEMLIST_H
#define QSHEETITEMLIST_H

#include <QTreeWidget>

class QAbstractStylesheetItem;

class QSheetItemList : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QSheetItemList(QWidget *parent = 0);

    void add(QAbstractStylesheetItem *item);
    void changed_item(QAbstractStylesheetItem* item);

    void setSelect(QAbstractStylesheetItem *item);

protected slots:
    void remove();
    void changed();
    void double_clicked(QTreeWidgetItem *item);

protected:
    void contextMenuEvent(QContextMenuEvent *);

signals:
    void add();
    void remove(QAbstractStylesheetItem* item);
    void changed(QAbstractStylesheetItem* item);
    void select(QAbstractStylesheetItem *item);

protected:
    QMap<QAbstractStylesheetItem*, QTreeWidgetItem*> m_sheetToItem;
    QMap<QTreeWidgetItem*, QAbstractStylesheetItem*> m_itemToSheet;
    QTreeWidgetItem *m_selectItem;
};

#endif // QSHEETITEMLIST_H
