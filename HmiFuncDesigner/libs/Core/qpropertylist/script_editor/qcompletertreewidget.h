#ifndef QCOMPLETETREEWIDGET_H
#define QCOMPLETETREEWIDGET_H

#include "qhostobjectmap.h"

#include <QTreeWidget>
#include <QMap>

class QTreeWidgetItem;

class QCompleterTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QCompleterTreeWidget(QWidget *parent = 0);

    void    init(tagReturnList* info,const QString &text);
    tagReturnList   *returnInfo();
    QString         get_source_text();

    void    move_position(const QRect &re);

protected slots:
    void itemselect(QTreeWidgetItem* item,int index);
signals:
    void    select(const QString &text);
protected:
    bool eventFilter(QObject *o, QEvent *e);
protected:
    tagReturnList       m_return_info;
    QString             m_text;
};

#endif // QCOMPLETETREEWIDGET_H
