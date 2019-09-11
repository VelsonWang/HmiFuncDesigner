#include "coloreditor.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QFocusEvent>
#include <QColorDialog>

ColorEditor::ColorEditor(QWidget *parent) :
    QWidget(parent),
    color_(Qt::white)
{
    toolButton_ = new QToolButton(this);
    toolButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    toolButton_->setText(tr("..."));
    toolButton_->setFixedWidth(20);
    toolButton_->installEventFilter(this);
    setFocusProxy(toolButton_);
    setFocusPolicy(toolButton_->focusPolicy());
    connect(toolButton_, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    colorLabel_ = new QLabel(this);
    QString styleSheet = QString("background:%1;").arg(color_.name());
    colorLabel_->setStyleSheet(styleSheet);
    colorLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(colorLabel_);
    layout->addWidget(toolButton_);
}

ColorEditor::~ColorEditor()
{

}

void ColorEditor::setColor(const QColor &color)
{
    if (color_ != color) {
        color_ = color;
        QString styleSheet = QString("background:%1;").arg(color_.name());
        colorLabel_->setStyleSheet(styleSheet);
    }
}

void ColorEditor::onToolButtonClicked()
{
    bool ok = false;
    QRgb oldRgba = color_.rgba();
    QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
    if (ok && newRgba != oldRgba) {
        setColor(QColor::fromRgba(newRgba));
    }
}

bool ColorEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == toolButton_ &&
            (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyPress)) {
        ev->ignore();
        return true;
    }
    return QWidget::eventFilter(obj, ev);
}

QColor ColorEditor::getColor() const
{
    return color_;
}

