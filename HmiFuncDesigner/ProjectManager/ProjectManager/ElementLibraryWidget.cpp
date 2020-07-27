#include "ElementLibraryWidget.h"
#include "ConfigUtils.h"
#include "Helper.h"
#include <QToolBox>

ElementLibraryWidget::ElementLibraryWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    QTabWidget *pTab = this->tabWidget;
    QString iniFileName = Helper::AppDir() + "/Config/DrawApplication.ini";
    QStringList sEleTypeList;
    ConfigUtils::getCfgList(iniFileName, "ElementTypeList", "list", sEleTypeList);
    QToolBox *toolBox = new QToolBox(this);
    for (int i=0; i< sEleTypeList.count(); i++) {
        QString nameTmp = sEleTypeList.at(i);
        QString name = QString(nameTmp.toLatin1());
        QWidget *page = new QWidget();
        QVBoxLayout *vLayout = new QVBoxLayout(page);
        vLayout->setSpacing(2);
        vLayout->setContentsMargins(1, 1, 1, 1);
        vLayout->setObjectName(QStringLiteral("vLayout"));

        ElementSimpleListWidget *eleWidget = new ElementSimpleListWidget(name);
        vLayout->addWidget(eleWidget);

        toolBox->addItem(page, name);
        elementListWidget.append(eleWidget);
    }
    toolBox->setCurrentIndex(0);
    this->layoutElements->addWidget(toolBox);
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
