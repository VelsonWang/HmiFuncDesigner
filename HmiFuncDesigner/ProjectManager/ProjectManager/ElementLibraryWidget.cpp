#include "ElementLibraryWidget.h"
#include "ConfigUtils.h"
#include "Helper.h"


ElementLibraryWidget::ElementLibraryWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    QTabWidget *pTab = this->tabWidget;
    QString iniFileName = Helper::AppDir() + "/Config/DrawApplication.ini";
    QStringList sEleTypeList;
    ConfigUtils::getCfgList(iniFileName, "ElementTypeList", "list", sEleTypeList);

    for (int i=0; i< sEleTypeList.count(); i++) {
        QString nameTmp = sEleTypeList.at(i);
        QString szName = QString(nameTmp.toLatin1());
        m_elementTreeWidgetObj.addElements(szName);
    }

    this->layoutElements->addWidget(&m_elementTreeWidgetObj);
    pTab->setCurrentIndex(0);

    libraryGridLayout->addWidget(&libraryListWidget);
    setWindowFlags(Qt::Tool);

    connect(pbLoadLibrary, SIGNAL(clicked()), &libraryListWidget, SLOT(loadLibrary()));
}

ElementLibraryWidget::~ElementLibraryWidget() {

}
