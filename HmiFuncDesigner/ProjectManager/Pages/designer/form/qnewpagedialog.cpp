#include "qnewpagedialog.h"
#include "ui_qnewpagedialog.h"
#include "qnewpagetable.h"
#include "qpagewidget.h"
#include "../../../libs/core/qsoftcore.h"
#include "../../../libs/shared/qprojectcore.h"
#include "../../../libs/shared/qpagemanager.h"
#include "../../../libs/shared/host/qabstracthost.h"
#include "../../../libs/core/undocommand/qpageaddundocommand.h"
#include "../../../libs/core/undocommand/qpropertychangedundocommand.h"
#include "../../../libs/shared/property/qabstractproperty.h"
#include <QVBoxLayout>
#include <QUuid>

QNewPageDialog::QNewPageDialog(QUndoStack *stack, QWidget *parent):
    QDialog(parent),
    ui(new Ui::QNewPageDialog),
    m_tab_bar(new QNewPageTable),
    m_stacked_widget(new QStackedWidget),
    m_undo_stack(stack)
{
    ui->setupUi(this);

    QVBoxLayout *v = new QVBoxLayout;
    v->setMargin(0);
    v->setSpacing(0);
    v->addWidget(m_tab_bar);
    v->addWidget(m_stacked_widget);
    setLayout(v);
    connect(m_tab_bar, SIGNAL(current_index_changed(int)),
            m_stacked_widget, SLOT(setCurrentIndex(int)));

    QPageWidget* wid = new QPageWidget("form");
    connect(wid, SIGNAL(cancel()), this, SLOT(close()));
    connect(wid, SIGNAL(ok(QAbstractHost*)), this, SLOT(ok(QAbstractHost*)));
    m_stacked_widget->insertWidget(-1, wid);
    wid = new QPageWidget("dialog");
    connect(wid, SIGNAL(cancel()), this, SLOT(close()));
    connect(wid, SIGNAL(ok(QAbstractHost*)), this, SLOT(ok(QAbstractHost*)));
    m_stacked_widget->insertWidget(-1, wid);
    wid = new QPageWidget("keyboard");
    connect(wid, SIGNAL(cancel()), this, SLOT(close()));
    connect(wid, SIGNAL(ok(QAbstractHost*)), this, SLOT(ok(QAbstractHost*)));
    m_stacked_widget->insertWidget(-1, wid);

    m_tab_bar->insert_data(tr("Form"));
    m_tab_bar->insert_data(tr("Dialog"));
    m_tab_bar->insert_data(tr("Keyboard"));

    m_tab_bar->set_current_index(0);

}

QNewPageDialog::~QNewPageDialog()
{
    delete ui;
}


void QNewPageDialog::ok(QAbstractHost *host)
{
    QProjectCore *core = QSoftCore::getCore()->getProjectCore();
    host->setPageManager(core->getPageManager());
    QList<QAbstractHost*> list;
    list.append(host);
    while(list.size() > 0) {
        QAbstractHost* h = list.takeFirst();
        h->setID(QUuid::createUuid().toString());
        list += h->getChildren();
    }
    host->setDefault();
    int index = core->getPageManager()->getPages().size();
    QUndoCommand *cmd = new QUndoCommand;
    new QPageAddUndoCommand(host, index, PAT_ADD, cmd);
    QAbstractProperty* pro = core->getProjectHost()->getProperty("start_page");
    if(pro != NULL) {
        if(pro->getValue().toString() == "") {
            new QPropertyChangedUndoCommand(core->getProjectHost()->getID(),
                                            "start_page", pro->getValue(), host->getID(), cmd);
        }
    }
    m_undo_stack->push(cmd);
    close();
}
