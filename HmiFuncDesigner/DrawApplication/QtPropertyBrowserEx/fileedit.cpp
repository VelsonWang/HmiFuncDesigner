#include "fileedit.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QFileDialog>

FileEdit::FileEdit(QWidget *parent)
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

void FileEdit::buttonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("选择文件"),
                                                    theLabel_->text(),
                                                    theFilter);
    if (filePath.isNull())
        return;
    theLabel_->setText(filePath);
    emit filePathChanged(filePath);
}


void FileEdit::setFilePath(const QString &filePath)
{
    if (theLabel_->text() != filePath)
        theLabel_->setText(filePath);
}

QString FileEdit::filePath() const
{
    return theLabel_->text();
}

void FileEdit::setFilter(const QString &filter)
{
    theFilter = filter;
}

QString FileEdit::filter() const
{
    return theFilter;
}
