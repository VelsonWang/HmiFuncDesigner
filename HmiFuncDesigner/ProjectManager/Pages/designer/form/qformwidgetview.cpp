#include "qformwidgetview.h"
#include "qformlistwidget.h"
#include "qnewpagedialog.h"
#include "../../../libs/core/styledbar.h"
#include "../../../libs/core/qsoftcore.h"
#include "../../../libs/core/qactiontoolbar.h"
#include "../../../libs/shared/host/qformhost.h"
#include "../../../libs/shared/qhostfactory.h"
#include "../../../libs/shared/qprojectcore.h"
#include "../../../libs/shared/qpagemanager.h"
#include <QVBoxLayout>
#include <QStringList>
#include <QPainter>

QFormWidgetView::QFormWidgetView(QWidget *parent) :
    QWidget(parent),
    m_formWidget(new QFormListWidget),
    m_styledBar(new StyledBar)
{
    this->setAutoFillBackground(false);
    QVBoxLayout *l = new QVBoxLayout;
    l->setMargin(0);
    l->setSpacing(0);

    l->addWidget(m_styledBar);
    l->addWidget(m_formWidget);

    this->setLayout(l);

    QSoftCore *core = QSoftCore::getCore();

    QActionToolBar *toolbar = new QActionToolBar;
    QList<QAction*> list;
    QAction *pActObj;

    pActObj = core->getAction("FormManager.Add");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), this, SLOT(add_page()));
        pActObj->setEnabled(false);
    }

    pActObj = new QAction(this);
    pActObj->setSeparator(true);
    list.append(pActObj);

    pActObj = core->getAction("FormManager.Same.Left");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(sameLeft()));
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.V-Center");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(sameVCenter()));
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.Right");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(sameRight()));
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.Top");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(sameTop()));
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.H-Center");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(sameHCenter()));
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.Bottom");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(sameBottom()));
        pActObj->setEnabled(false);
    }

    pActObj = new QAction(this);
    pActObj->setSeparator(true);
    list.append(pActObj);

    pActObj = core->getAction("FormManager.Same.Width");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(sameWidth()));
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.Height");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(sameHeight()));
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.Gemotry");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(sameGeometry()));
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Horizontal.UniformDistribution");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(horizontalUniformDistribution()));
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Vertical.UniformDistribution");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(verticalUniformDistribution()));
        pActObj->setEnabled(false);
    }

    pActObj = new QAction(this);
    pActObj->setSeparator(true);
    list.append(pActObj);

    pActObj = core->getAction("Undo.Redo");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(redo()));
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("Undo.Undo");
    if(pActObj != NULL) {
        list.append(pActObj);
        connect(pActObj, SIGNAL(triggered()), m_formWidget, SLOT(undo()));
        pActObj->setEnabled(false);
    }

    pActObj = new QAction(this);
    pActObj->setSeparator(true);
    list.append(pActObj);

    toolbar->addButtonActions(list);

    QHBoxLayout *h = new QHBoxLayout;
    h->setMargin(0);
    h->setSpacing(0);
    h->addWidget(toolbar, 1);
    m_styledBar->setLayout(h);

    connect(m_formWidget, SIGNAL(select(QAbstractHost*)),
            this, SIGNAL(select(QAbstractHost*)));
    connect(m_formWidget, SIGNAL(remove(QList<QAbstractHost*>)),
            this, SIGNAL(remove(QList<QAbstractHost*>)));
    connect(this, SIGNAL(notifyPropertyEdit(QAbstractProperty*, QVariant)),
            m_formWidget, SLOT(slotPropertyEdit(QAbstractProperty*, QVariant)));
    connect(QSoftCore::getCore()->getProjectCore(), SIGNAL(notifyOpened()),
            this, SLOT(project_opened()));
    connect(QSoftCore::getCore()->getProjectCore(), SIGNAL(notifyClosed()),
            this, SLOT(project_closed()));
}

QFormWidgetView::~QFormWidgetView()
{
}

void QFormWidgetView::project_opened()
{
    QPageManager *manager = QSoftCore::getCore()->getProjectCore()->getPageManager();
    QList<QAbstractHost*> pages = manager->getPages();

    foreach(QAbstractHost* h, pages) {
        h->getObject()->setProperty("no-ManhattanStyle", true);
        m_formWidget->insertForm(h);
    }
    if(pages.size() > 0) {
        m_formWidget->showForm(pages.first());
    }

    QSoftCore *core = QSoftCore::getCore();

    QAction *pActObj;

    pActObj = core->getAction("FormManager.Add");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("FormManager.Del");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("FormManager.Same.Left");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("FormManager.Same.V-Center");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("FormManager.Same.Right");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("FormManager.Same.Top");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("FormManager.Same.H-Center");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("FormManager.Same.Bottom");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("FormManager.Same.Width");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("FormManager.Same.Height");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("FormManager.Same.Gemotry");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("FormManager.Horizontal.UniformDistribution");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("FormManager.Vertical.UniformDistribution");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("Undo.Redo");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }

    pActObj = core->getAction("Undo.Undo");
    if(pActObj != NULL) {
        pActObj->setEnabled(true);
    }
}

void QFormWidgetView::project_closed()
{
    m_language_id.clear();
    m_formWidget->clear();
    m_undo_stack->clear();

    QSoftCore *core = QSoftCore::getCore();

    QAction *pActObj;

    pActObj = core->getAction("FormManager.Add");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Del");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.Left");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.V-Center");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.Right");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.Top");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.H-Center");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.Bottom");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.Width");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.Height");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Same.Gemotry");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Horizontal.UniformDistribution");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("FormManager.Vertical.UniformDistribution");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("Undo.Redo");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }

    pActObj = core->getAction("Undo.Undo");
    if(pActObj != NULL) {
        pActObj->setEnabled(false);
    }
}

void QFormWidgetView::add_page()
{
    QNewPageDialog dlg(m_undo_stack, this);
    dlg.exec();
}

void QFormWidgetView::setUndoStack(QUndoStack *stack)
{
    m_undo_stack = stack;
    m_formWidget->setUndoStack(stack);
}

void QFormWidgetView::set_select(QAbstractHost *host)
{
    m_formWidget->selectHost(host);
}

void QFormWidgetView::onZoom()
{
    m_formWidget->onZoom();
}
