#include "ElementLibraryWidget.h"

ElementLibraryWidget::ElementLibraryWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    simpleGridLayout->addWidget(&simpleListWidget);
    libraryGridLayout->addWidget(&libraryListWidget);
    setWindowFlags(Qt::Tool);

    connect(pbLoadLibrary,SIGNAL(clicked()),&libraryListWidget,SLOT(loadLibrary()));
}

