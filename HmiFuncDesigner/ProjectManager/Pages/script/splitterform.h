#ifndef SPLITTERFORM_H
#define SPLITTERFORM_H
#include <QSplitter>
#include <QWidget>

class SplitterForm : public QSplitter
{
public:
    SplitterForm(Qt::Orientation orientation, QWidget * parent = 0);

};

#endif // SPLITTERFORM_H
