#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QListWidget>

class GraphPageTreeView : public QListWidget
{
    Q_OBJECT
public:
    GraphPageTreeView(QWidget *parent = 0);
    void graphPageChangeName(int index,const QString &);
    void updateView();

public slots:
    void addNewElement(const QString &);

private:
    QIcon itemIcon;
};

#endif // PROJECTTREEVIEW_H
