#ifndef LISTVIEWEX_H
#define LISTVIEWEX_H

#include "corelibglobal.h"
#include <QListView>

class CORELIB_EXPORT ListViewEx : public QListView
{
public:
    ListViewEx(QWidget *parent = nullptr);
    ~ListViewEx();
    void setViewportMargin(int left, int top, int right, int bottom);

};

#endif // LISTVIEWEX_H
