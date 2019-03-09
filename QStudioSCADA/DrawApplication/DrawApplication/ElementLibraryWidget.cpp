#include "ElementLibraryWidget.h"
#include "configutils.h"
#include "Helper.h"
#include <QTabWidget>
#include <QDebug>

ElementLibraryWidget::ElementLibraryWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    QTabWidget *pTab = this->tabWidget;
    QString iniFileName = Helper::AppDir() + "/Config/DrawApplication.ini";
    QStringList sEleTypeList;
    ConfigUtils::getCfgList(iniFileName, "ElementTypeList", "list", sEleTypeList);
    for (int i=0; i< sEleTypeList.count(); i++)
    {
        QString nameTmp = sEleTypeList.at(i);
        QString name = QString(nameTmp.toLatin1());
        ElementSimpleListWidget *eleWidget = new ElementSimpleListWidget(name);
        pTab->insertTab(i, eleWidget, name);
        elementListWidget.append(eleWidget);
    }
    pTab->setCurrentIndex(0);

    libraryGridLayout->addWidget(&libraryListWidget);
    setWindowFlags(Qt::Tool);

    connect(pbLoadLibrary, SIGNAL(clicked()), &libraryListWidget, SLOT(loadLibrary()));
}

ElementLibraryWidget::~ElementLibraryWidget()
{
    qDeleteAll(elementListWidget);
    elementListWidget.clear();
}
