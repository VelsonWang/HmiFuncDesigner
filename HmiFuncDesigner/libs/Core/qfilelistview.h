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
    explicit QFileListView(bool canRemove = true, bool show_picture = false, bool doubleClicked = true, QWidget *parent = 0);

    void init();

    tagFileInfo* getFile(QTreeWidgetItem* item);
    tagFileGroupInfo *getGroup(QTreeWidgetItem* item);

    bool canRemove();

protected:
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
    void mousePressEvent(QMouseEvent *event);
    void selectFirst(tagFileGroupInfo *g, tagFileInfo *f);
    void select(tagFileInfo* f);

signals:
    void sigRemoveGroup(tagFileGroupInfo *g);
    void sigRemoveFile(tagFileInfo *f);
    void sigSelectFile(tagFileInfo *f);

protected slots:
    void insertGroup(tagFileGroupInfo *g, int index);
    void removeGroup(tagFileGroupInfo *g);

    void insertFile(tagFileGroupInfo *g, tagFileInfo *f, int index);
    void removeFile(tagFileGroupInfo *g, tagFileInfo *f);

    void buttonClicked();
    void doubleClicked(QTreeWidgetItem *item);

protected:
    friend class QFileDelegate;

protected:
    QMap<tagFileInfo*, QTreeWidgetItem*> m_fileToItem;
    QMap<QTreeWidgetItem*, tagFileInfo*> m_itemToFile;
    QMap<tagFileGroupInfo*, QTreeWidgetItem*> m_groupToItem;
    QMap<QTreeWidgetItem*, tagFileGroupInfo*> m_itemToGroup;
    QIcon m_icon;
    bool m_canRemove;
    QString m_selectUuid;
    QTreeWidgetItem *m_current;
    bool m_showPicture;
    bool m_doubleClicked;
};

#endif // QFILELISTVIEW_H
