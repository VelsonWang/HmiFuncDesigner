#ifndef QFILELISTVIEW_H
#define QFILELISTVIEW_H

#include "corelibglobal.h"

#include <QTreeWidget>

struct tagFileInfo;
struct tagFileGroupInfo;

class CORELIB_EXPORT QFileListView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QFileListView(bool can_remove = true, bool show_picture = false, bool double_clicked = true, QWidget *parent = 0);

    void    init();

    tagFileInfo* get_file(QTreeWidgetItem* item);
    tagFileGroupInfo *get_group(QTreeWidgetItem* item);

    bool        can_remove();
protected:
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
    void mousePressEvent(QMouseEvent *event);

    void    select_first(tagFileGroupInfo *g, tagFileInfo *f);

    void    select(tagFileInfo* f);
signals:
    void    remove_group_signal(tagFileGroupInfo *g);
    void    remove_file_signal(tagFileInfo *f);
    void    select_file_signal(tagFileInfo *f);
protected slots:
    void    insert_group(tagFileGroupInfo *g, int index);
    void    remove_group(tagFileGroupInfo *g);

    void    insert_file(tagFileGroupInfo *g, tagFileInfo *f, int index);
    void    remove_file(tagFileGroupInfo *g, tagFileInfo *f);

    void    button_clicked();

    void    double_clicked(QTreeWidgetItem *item);
protected:
    friend class QFileDelegate;
protected:
    QMap<tagFileInfo*, QTreeWidgetItem*>     m_file_to_item;
    QMap<QTreeWidgetItem*, tagFileInfo*>     m_item_to_file;
    QMap<tagFileGroupInfo*, QTreeWidgetItem*>     m_group_to_item;
    QMap<QTreeWidgetItem*, tagFileGroupInfo*>     m_item_to_group;
    QIcon                                   m_icon;
    bool                                    m_can_remove;
    QString                                 m_select_uuid;
    QTreeWidgetItem                         *m_current;
    bool                                    m_show_picture;
    bool                                    m_double_clicked;
};

#endif // QFILELISTVIEW_H
