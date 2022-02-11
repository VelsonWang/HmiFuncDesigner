#include "qscripteditdialog.h"
#include "qscripteditview.h"
#include "qwidgetview.h"
#include "qfunctionview.h"
#include "qwidgetviewtoolbar.h"
#include "qfindwidget.h"
#include "../../styledbar.h"
#include "../../minisplitter.h"
#include "../../../shared/host/qabstracthost.h"
#include "../../../shared/property/qabstractproperty.h"
#include "../../undocommand/qpropertychangedundocommand.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QUndoStack>

QScriptEditDialog::QScriptEditDialog(QAbstractProperty *property,
                                     QUndoStack *stack,
                                     QWidget *parent) :
    QDialog(parent),
    m_editView(new QScriptEditView(property, property->getHost())),
    m_widgetView(new QWidgetView(property->getHost())),
    m_functionView(new QFunctionView),
    m_widgetStyledBar(new StyledBar(this)),
    m_widgetToolBar(new QWidgetViewToolBar),
    m_functionStyledBar(new StyledBar(this)),
    m_functionToolBar(new QWidgetViewToolBar),
    m_editStyledBar(new StyledBar(this)),
    m_property(property),
    m_findWidget(new QFindWidget),
    m_undoStack(stack)
{
    MiniSplitter *splitter;

    splitter = new MiniSplitter;

    QVBoxLayout *v = new QVBoxLayout;
    v->setMargin(0);
    v->setSpacing(0);
    v->addWidget(m_widgetToolBar);
    m_widgetStyledBar->setLayout(v);
    m_widgetToolBar->setLabelText(tr("Object"));

    v = new QVBoxLayout;
    v->setMargin(0);
    v->setSpacing(0);
    v->addWidget(m_functionToolBar);
    m_functionStyledBar->setLayout(v);

    m_functionToolBar->setLabelText(tr("Function"));

    MiniSplitter *s = new MiniSplitter(Qt::Vertical);

    QWidget* wid = new QWidget;
    v = new QVBoxLayout;
    v->setMargin(0);
    v->setSpacing(0);
    v->addWidget(m_widgetStyledBar);
    v->addWidget(m_widgetView);
    wid->setLayout(v);
    s->addWidget(wid);

    wid = new QWidget;
    v = new QVBoxLayout;
    v->setMargin(0);
    v->setSpacing(0);
    v->addWidget(m_functionStyledBar);
    v->addWidget(m_functionView);
    wid->setLayout(v);
    s->addWidget(wid);

    s->resize(200, 200);

    splitter->addWidget(s);

    wid = new QWidget;
    v = new QVBoxLayout;
    v->setMargin(0);
    v->setSpacing(0);
    v->addWidget(m_editView->toolBar());
    m_editStyledBar->setLayout(v);

    wid = new QWidget;
    v = new QVBoxLayout;
    v->setMargin(0);
    v->setSpacing(0);
    v->addWidget(m_editStyledBar);
    v->addWidget(m_editView);
    v->addWidget(m_findWidget);
    wid->setLayout(v);
    splitter->addWidget(wid);

    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    v = new QVBoxLayout;
    v->setMargin(0);
    v->setSpacing(0);
    v->addWidget(splitter);
    this->setLayout(v);
    this->resize(800, 600);

    connect(m_functionToolBar, SIGNAL(findNext(QString)), m_functionView, SLOT(findNext(QString)));
    connect(m_functionToolBar, SIGNAL(findPrev(QString)), m_functionView, SLOT(findPrev(QString)));
    connect(m_widgetView, SIGNAL(select(QAbstractHost*)), m_functionView, SLOT(setHost(QAbstractHost*)));
    connect(m_editView, SIGNAL(find()), this, SLOT(script_find()));
    connect(m_findWidget, SIGNAL(find(QString)), m_editView, SLOT(find(QString)));
    connect(m_findWidget, SIGNAL(findNext()), m_editView, SLOT(findNext()));
    connect(m_findWidget, SIGNAL(findPrev()), m_editView, SLOT(findPrev()));
    connect(m_findWidget, SIGNAL(replace(QString)), m_editView, SLOT(replace(QString)));
    connect(m_findWidget, SIGNAL(replaceAll(QString)), m_editView, SLOT(replaceAll(QString)));
    connect(m_findWidget, SIGNAL(replaceAndNext(QString)), m_editView, SLOT(replaceNext(QString)));

    QAbstractHost *host = m_property->getHost();
    m_widgetView->setSelect(host);
    connect(m_functionView, SIGNAL(select(QMetaMethod)), this, SLOT(selectFunction(QMetaMethod)));
    connect(m_editView, SIGNAL(save()), this, SLOT(save()));

    m_editView->setPlainText(m_property->getValue().toString());
    QTextCursor c = m_editView->textCursor();
    c.movePosition(QTextCursor::End);
    m_editView->setTextCursor(c);
    m_editView->setFocus();

    QSize sz = qApp->desktop()->size();

    this->setGeometry(sz.width() * 0.1, sz.height() * 0.1, sz.width() * 0.8, sz.height() * 0.8);
}

void QScriptEditDialog::save()
{
    QPropertyChangedUndoCommand *cmd = new QPropertyChangedUndoCommand(m_property->getHost()->getUuid(),
            m_property->getObjectProperty("name").toString(),
            m_property->getValue(),
            m_editView->toPlainText());
    m_undoStack->push(cmd);
}

void QScriptEditDialog::selectFunction(const QMetaMethod &method)
{
    QAbstractHost *host = m_widgetView->currentHost();
    if(host != NULL) {
        m_editView->insertMethod(method, host);
    }
}

void QScriptEditDialog::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape) {
        return;
    }
    QDialog::keyReleaseEvent(e);
}

void QScriptEditDialog::closeEvent(QCloseEvent *e)
{
    QString str = m_editView->toPlainText();
    if(str != m_property->getValue().toString()) {
        int ret = QMessageBox::warning(this,
                                       tr("Save"),
                                       tr("Do you want save?"),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(ret == QMessageBox::Yes) {
            save();
        } else if(ret == QMessageBox::Cancel) {
            e->ignore();
            return;
        }
    }
    QDialog::closeEvent(e);
}

void QScriptEditDialog::script_find()
{
    QTextCursor cursor = m_editView->textCursor();
    QString str = cursor.selectedText();
    if(!m_findWidget->isVisible()) {
        m_findWidget->show();
    }
    m_findWidget->setFindText(str);
}
