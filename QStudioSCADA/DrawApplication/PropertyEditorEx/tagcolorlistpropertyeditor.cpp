#include "tagcolorlistpropertyeditor.h"
#include "tagcolorlisteditordialog.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QDebug>

TagColorListPropertyEditor::TagColorListPropertyEditor(QWidget *parent) :
    QWidget(parent),
    valueColorList_(QStringList())
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

void TagColorListPropertyEditor::setValueColorList(const QStringList &list) {
    if (valueColorList_ != list) {
        valueColorList_ = list;
    }
}

void TagColorListPropertyEditor::onToolButtonClicked() {
    TagColorListEditorDialog dlg(0);
    dlg.setValueColorList(valueColorList_);
    if(dlg.exec() == QDialog::Accepted) {
        QStringList list = dlg.getValueColorList();
        if (list != valueColorList_) {
            setValueColorList(list);
            emit dataChangedByUser(list, this);
        }
    }
}

bool TagColorListPropertyEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == toolButton_ && (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyPress))
    {
        ev->ignore();
        return true;
    }

    return QWidget::eventFilter(obj, ev);
}

QStringList TagColorListPropertyEditor::getValueColorList() const {
    return valueColorList_;
}


