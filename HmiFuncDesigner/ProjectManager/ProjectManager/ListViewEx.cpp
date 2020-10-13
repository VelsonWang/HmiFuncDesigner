#include "ListViewEx.h"

ListViewEx::ListViewEx(QWidget *parent) : QListView(parent)
{

}

ListViewEx::~ListViewEx()
{

}

void ListViewEx::setViewportMargin(int left, int top, int right, int bottom)
{
    this->setViewportMargins(left, top, right, bottom);
};
