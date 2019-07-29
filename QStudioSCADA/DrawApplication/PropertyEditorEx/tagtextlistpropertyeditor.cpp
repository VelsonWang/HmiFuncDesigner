#include "tagtextlistpropertyeditor.h"
#include "tagtextlisteditordialog.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QDebug>

TagTextListPropertyEditor::TagTextListPropertyEditor(QWidget *parent)
    : QWidget(parent),
      valueTextList_(QStringList())
{
    toolButton_ = new QToolButton(this);
    toolButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    toolButton_->setText(tr("..."));
    toolButton_->setFixedWidth(20);
    toolButton_->installEventFilter(this);
    setFocusProxy(toolButton_);
    setFocusPolicy(toolButton_->focusPolicy());
    connect(toolButton_, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    spacer_ = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Ignored);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addItem(spacer_);
    layout->addWidget(toolButton_);
}

void TagTextListPropertyEditor::setValueTextList(const QStringList &list)
{
    if (valueTextList_ != list) {
        valueTextList_ = list;
    }
}

void TagTextListPropertyEditor::onToolButtonClicked()
{
    TagTextListEditorDialog dlg;
    dlg.setValueTextList(valueTextList_);

    if(dlg.exec() == QDialog::Accepted) {
        QStringList list = dlg.getValueTextList();
        if (list != valueTextList_) {
            setValueTextList(list);
            emit dataChangedByUser(list, this);
        }
    }
}

bool TagTextListPropertyEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == toolButton_ && (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyPress))
    {
        ev->ignore();
        return true;
    }

    return QWidget::eventFilter(obj, ev);
}

QStringList TagTextListPropertyEditor::getValueTextList() const
{
    return valueTextList_;
}


