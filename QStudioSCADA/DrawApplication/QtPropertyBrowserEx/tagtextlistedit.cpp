#include "tagtextlistedit.h"
#include "tagtextlisteditordialog.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QFocusEvent>

TagTextListEdit::TagTextListEdit(QWidget *parent)
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

void TagTextListEdit::buttonClicked()
{
    TagTextListEditorDialog dlg;
    dlg.setValueTextList(valueTextList_);

    if(dlg.exec() == QDialog::Accepted) {
        QStringList list = dlg.getValueTextList();
        if (list != valueTextList_) {
            setValueTextList(list);
            QString szVal = list.join("|");
            theLabel_->setText(szVal);
            emit valueTextListChanged(szVal);
        }
    }
}

void TagTextListEdit::setValueTextList(const QStringList &list)
{
    if (valueTextList_ != list) {
        valueTextList_ = list;
    }
}


QStringList TagTextListEdit::getValueTextList() const
{
    return valueTextList_;
}


