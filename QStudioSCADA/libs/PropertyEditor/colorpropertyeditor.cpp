#include "colorpropertyeditor.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QColorDialog>

ColorPropertyEditor::ColorPropertyEditor(QWidget *parent) :
    QWidget(parent)
{
    toolButton_ = new QToolButton(this);
    toolButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    toolButton_->setText(tr("..."));
    toolButton_->setFixedWidth(20);
    toolButton_->installEventFilter(this);
    connect(toolButton_, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    spacer_ = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Ignored);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addItem(spacer_);
    layout->addWidget(toolButton_);
}

void ColorPropertyEditor::setColor(const QColor& color)
{
    if (color_ != color) {
        color_ = color;
    }
}

QPixmap ColorPropertyEditor::getColorPixmap(const QColor& color, unsigned int size)
{
    QImage tmpImgage(size, size, QImage::Format_ARGB32_Premultiplied);
    tmpImgage.fill(static_cast<unsigned int>(color.rgb()));
    return QPixmap::fromImage(tmpImgage);
    // todo: support alpha channel
}

QString ColorPropertyEditor::getColorString(const QColor& color)
{
    return QApplication::translate("QtPropertyExplorer", "[%1, %2, %3] (%4)", "Colors as string")
                                 .arg(QString::number(color.red()))
                                 .arg(QString::number(color.green()))
                                 .arg(QString::number(color.blue()))
                                 .arg(QString::number(color.alpha()));

}

void ColorPropertyEditor::onToolButtonClicked()
{
    bool ok = false;
    QRgb oldRgba = color_.rgba();
    QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
    if (ok && newRgba != oldRgba) {
        setColor(QColor::fromRgba(newRgba));
        emit dataChangedByUser(color_, this);
    }
}

bool ColorPropertyEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == toolButton_ && (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyPress))
    {
        ev->ignore();
        return true;
    }

    return QWidget::eventFilter(obj, ev);
}

QColor ColorPropertyEditor::getColor() const
{
    return color_;
}

