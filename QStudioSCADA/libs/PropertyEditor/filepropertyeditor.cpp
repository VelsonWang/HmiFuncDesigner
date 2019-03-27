#include "filepropertyeditor.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QFileDialog>
#include <QtDebug>

FilePropertyEditor::FilePropertyEditor(QWidget *parent) :
    QWidget(parent),
    file_(QString())
{
    // Create the tool button
    toolButton_ = new QToolButton(this);
    toolButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    toolButton_->setText(tr("..."));
    toolButton_->setFixedWidth(20);
    toolButton_->installEventFilter(this);
    setFocusProxy(toolButton_);	// Make the ToolButton the focus proxy
    setFocusPolicy(toolButton_->focusPolicy());
    connect(toolButton_, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    // Create the text label
    textLabel_ = new QLabel(this);
    textLabel_->setText(file_);

    // Spacer (this is needed for proper display of the label and button)
    spacer_ = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Ignored);

    // The layout (a horizontal layout)
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(textLabel_);
    layout->addItem(spacer_);
    layout->addWidget(toolButton_);
}

void FilePropertyEditor::setFile(const QString& file)
{
    if (file != file_) {
        file_ = file;
        textLabel_->setText(file_);
    }
}

void FilePropertyEditor::onToolButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR,
                                                    tr("选择图片"),
                                                    QApplication::applicationDirPath(),
                                                    tr("Images (*.png *.xpm *.jpg *.bmp)"));

    if (fileName != file_) {
        setFile(fileName);
        emit dataChangedByUser(file_, this);
    }
}

bool FilePropertyEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == toolButton_ && (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyPress))
    {
        ev->ignore();
        return true;
    }

    return QWidget::eventFilter(obj, ev);
}

QString FilePropertyEditor::getFile() const
{
    return file_;
}

