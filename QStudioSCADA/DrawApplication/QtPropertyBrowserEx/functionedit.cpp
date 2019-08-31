#include "functionedit.h"
#include "functioneditordialog.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QFocusEvent>

FunctionEdit::FunctionEdit(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    theLineEdit = new QLineEdit(this);
    theLineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    theLineEdit->setReadOnly(true);
    theLineEdit->setStyleSheet("background:rgb(194, 194, 194)");
    QToolButton *button = new QToolButton(this);
    button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    button->setText(QLatin1String("..."));
    layout->addWidget(theLineEdit);
    layout->addWidget(button);
    setFocusProxy(theLineEdit);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_InputMethodEnabled);
    connect(theLineEdit, SIGNAL(textEdited(const QString &)),
            this, SIGNAL(filePathChanged(const QString &)));
    connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
}

void FunctionEdit::buttonClicked()
{
    FunctionEditorDialog dlg(Q_NULLPTR, supportEvents_);
    dlg.setFunctions(funcs_);
    if(dlg.exec() == QDialog::Accepted) {
        QStringList funcs = dlg.getFunctions();
        setFunctions(funcs);
        QString szFuncs = funcs.join("|");
        theLineEdit->setText(szFuncs);
        emit filePathChanged(szFuncs);
    }
}

void FunctionEdit::focusInEvent(QFocusEvent *e)
{
    theLineEdit->event(e);
    if (e->reason() == Qt::TabFocusReason || e->reason() == Qt::BacktabFocusReason) {
        theLineEdit->selectAll();
    }
    QWidget::focusInEvent(e);
}

void FunctionEdit::focusOutEvent(QFocusEvent *e)
{
    theLineEdit->event(e);
    QWidget::focusOutEvent(e);
}

void FunctionEdit::keyPressEvent(QKeyEvent *e)
{
    theLineEdit->event(e);
}

void FunctionEdit::keyReleaseEvent(QKeyEvent *e)
{
    theLineEdit->event(e);
}
