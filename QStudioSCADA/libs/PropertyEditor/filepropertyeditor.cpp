#include "filepropertyeditor.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QFileDialog>

FilePropertyEditor::FilePropertyEditor(QWidget *parent) :
    QWidget(parent),
    szFileName_(QString(""))
{
    toolButton_ = new QToolButton(this);
    toolButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    toolButton_->setText(tr("..."));
    toolButton_->setFixedWidth(20);
    toolButton_->installEventFilter(this);
    connect(toolButton_, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    textLabel_ = new QLabel(this);
    textLabel_->setText(szFileName_);

    spacer_ = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Ignored);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(textLabel_);
    layout->addItem(spacer_);
    layout->addWidget(toolButton_);
}

void FilePropertyEditor::setFile(const QString& file)
{
    this->szFileName_ = file;
    textLabel_->setText(szFileName_);
}

void FilePropertyEditor::onToolButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR,
                                                    tr("选择图片"),
                                                    QApplication::applicationDirPath(),
                                                    tr("Images (*.png *.xpm *.jpg *.bmp)"));
    setFile(fileName);
}

bool FilePropertyEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == toolButton_ && (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyPress)) {
        ev->ignore();
        return true;
    }

    return QWidget::eventFilter(obj, ev);
}

QString FilePropertyEditor::getFile() const
{
    return szFileName_;
}

