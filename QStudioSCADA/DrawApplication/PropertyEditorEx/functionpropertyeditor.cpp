#include "functionpropertyeditor.h"
#include "FunctionEditorDialog.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QtDebug>

FunctionPropertyEditor::FunctionPropertyEditor(QWidget *parent) :
    QWidget(parent),
    funcs_(QStringList())
{
    toolButton_ = new QToolButton(this);
    toolButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    toolButton_->setText(tr("..."));
    toolButton_->setFixedWidth(20);
    toolButton_->installEventFilter(this);
    setFocusProxy(toolButton_);
    setFocusPolicy(toolButton_->focusPolicy());
    connect(toolButton_, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    textLabel_ = new QLabel(this);
    textLabel_->setText(tr("功能操作..."));

    spacer_ = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Ignored);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(textLabel_);
    layout->addItem(spacer_);
    layout->addWidget(toolButton_);
}

void FunctionPropertyEditor::setFunctions(const QStringList &funcs)
{
    if (funcs_ != funcs_) {
        funcs_ = funcs;
        textLabel_->setText(tr("功能操作..."));
    }
}

void FunctionPropertyEditor::onToolButtonClicked()
{
    FunctionEditorDialog dlg;
    if(dlg.exec() == QDialog::Accepted) {
        QStringList funcs = dlg.getFunctions();
        if (funcs != funcs_) {
            setFunctions(funcs);
            emit dataChangedByUser(funcs, this);
        }
    }
}

bool FunctionPropertyEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == toolButton_ && (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyPress))
    {
        ev->ignore();
        return true;
    }

    return QWidget::eventFilter(obj, ev);
}

QStringList FunctionPropertyEditor::getFunctions() const
{
    return funcs_;
}

