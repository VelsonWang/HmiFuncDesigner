#include "functionpropertyeditor.h"
#include "FunctionEditorDialog.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>


FunctionPropertyEditor::FunctionPropertyEditor(QWidget *parent) :
    QWidget(parent),
    funcs_(QStringList())
{
    toolButton_ = new QToolButton(this);
    toolButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    toolButton_->setText(tr("..."));
    toolButton_->setFixedWidth(20);
    toolButton_->installEventFilter(this);
    connect(toolButton_, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    textLabel_ = new QLabel(this);
    textLabel_->setText(tr(""));

    spacer_ = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Ignored);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(textLabel_);
    layout->addItem(spacer_);
    layout->addWidget(toolButton_);
}

void FunctionPropertyEditor::setFunctions(const QStringList &funcs) {
    funcs_ = funcs;
}

void FunctionPropertyEditor::onToolButtonClicked() {
    FunctionEditorDialog dlg(0, supportEvents_);
    dlg.setFunctions(funcs_);
    if(dlg.exec() == QDialog::Accepted) {
        QStringList funcs = dlg.getFunctions();
        setFunctions(funcs);
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

/**
 * @brief FunctionPropertyEditor::setSupportEvents
 * @details 设置支持的事件列表
 * @param events 事件列表
 */
void FunctionPropertyEditor::setSupportEvents(QStringList events)
{
    supportEvents_ = events;
}
