#ifndef LISTVIEWEX_H
#define LISTVIEWEX_H

#include <QListView>

class ListViewEx : public QListView
{
public:
    ListViewEx(QWidget *parent = nullptr);
    ~ListViewEx();
    void setViewportMargin(int left, int top, int right, int bottom);

};

#endif // LISTVIEWEX_H
