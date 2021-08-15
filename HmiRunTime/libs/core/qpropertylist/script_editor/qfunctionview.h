#ifndef QFUNCTIONVIEW_H
#define QFUNCTIONVIEW_H

#include <QTreeWidget>
#include <QMetaMethod>

class QAbstractHost;

class QFunctionView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QFunctionView(QWidget *parent = 0);

public slots:
    void setHost(QAbstractHost* host);

    void findNext(const QString &text);
    void findPrev(const QString &text);

protected slots:
    void doubleClicked(QTreeWidgetItem *item, int column);

protected:
    void addItem(const QMetaMethod & method,const QString &toolTip);
    void clearAll();

signals:
    void select(const QMetaMethod &function);

public slots:

protected:
    QMap<QTreeWidgetItem*, QMetaMethod> m_itemToMethod;
    QList<QTreeWidgetItem*> m_items;
};

#endif // QFUNCTIONVIEW_H
