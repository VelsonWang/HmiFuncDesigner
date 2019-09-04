#include "tagcolorlistedit.h"
#include "tagcolorlisteditordialog.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QFocusEvent>

TagColorListEdit::TagColorListEdit(QWidget *parent)
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

QStringList TagColorListEdit::getValueColorList() const
{
    return valueColorList_;
}

void TagColorListEdit::setValueColorList(const QStringList &list)
{
    if (valueColorList_ != list) {
        valueColorList_ = list;
        QString szVal = list.join("|");
        theLabel_->setText(szVal);
    }
}

void TagColorListEdit::buttonClicked()
{
    TagColorListEditorDialog dlg;
    dlg.setValueColorList(valueColorList_);
    if(dlg.exec() == QDialog::Accepted) {
        QStringList list = dlg.getValueColorList();
        if (list != valueColorList_) {
            setValueColorList(list);
            QString szVal = list.join("|");
            theLabel_->setText(szVal);
            emit valueColorListChanged(szVal);
        }
    }
}
