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
    theLabel_ = new QLabel(this);
    theLabel_->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    QToolButton *button = new QToolButton(this);
    button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    button->setText(QLatin1String("..."));
    layout->addWidget(theLabel_);
    layout->addWidget(button);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_InputMethodEnabled);
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
        theLabel_->setText(szFuncs);
        emit functionsChanged(szFuncs);
    }
}

QStringList FunctionEdit::getFunctions() const
{
    return funcs_;
}


void FunctionEdit::setFunctions(const QStringList &funcs)
{
    funcs_ = funcs;
    QString szFuncs = funcs.join("|");
    theLabel_->setText(szFuncs);
}

/**
 * @brief setSupportEvents
 * @details 设置支持的事件列表
 * @param events 事件列表
 */
void FunctionEdit::setSupportEvents(QStringList events)
{
    supportEvents_ = events;
}
