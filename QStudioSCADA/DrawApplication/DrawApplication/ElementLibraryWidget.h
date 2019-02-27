#ifndef ELEMENTLIBRARYWIDGET_H
#define ELEMENTLIBRARYWIDGET_H

#include <QWidget>
#include "ElementSimpleListWidget.h"
#include "ElementLibraryListWidget.h"
#include "ui_ElementLibraryWidget.h"

class ElementLibraryWidget : public QWidget, public Ui::ElementLibraryWidget
{
    Q_OBJECT
    
public:
    explicit ElementLibraryWidget(QWidget *parent = 0);

private:
    ElementSimpleListWidget simpleListWidget;
    ElementLibraryListWidget libraryListWidget;
};

#endif // ELEMENTLIBRARYWIDGET_H
