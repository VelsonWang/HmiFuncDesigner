#include "colorpropertyeditor.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QColorDialog>
#include <QtDebug>

ColorPropertyEditor::ColorPropertyEditor(QWidget *parent) :
    QWidget(parent)
{
    // Create the tool button
    ToolButton = new QToolButton(this);
    ToolButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    ToolButton->setText(tr("..."));
    ToolButton->setFixedWidth(20);
    ToolButton->installEventFilter(this);
    setFocusProxy(ToolButton);	// Make the ToolButton the focus proxy
    setFocusPolicy(ToolButton->focusPolicy());
    connect(ToolButton, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    // Create the text label
    TextLabel = new QLabel(this);
    TextLabel->setText(getColorString(Color));

    // Create the label for the pixmap
    ColorLabel = new QLabel(this);
    ColorLabel->setPixmap(getColorPixmap(Color));

    // Spacer (this is needed for proper display of the label and button)
    Spacer = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Ignored);

    // The layout (a horizontal layout)
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addItem(Spacer);
    layout->addWidget(ToolButton);
    TextLabel->hide();
    ColorLabel->hide();	// for now, we just use the standard display and only add the button
}

void ColorPropertyEditor::setColor(const QColor& color_)
{
    if (Color != color_)
    {
        Color = color_;
        ColorLabel->setPixmap(getColorPixmap(Color));
        TextLabel->setText(getColorString(Color));
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
    QRgb oldRgba = Color.rgba();
    QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
    if (ok && newRgba != oldRgba) {
        qDebug() << "OKCOLOR";
        setColor(QColor::fromRgba(newRgba));
        emit dataChangedByUser(Color, this);
    }
}

bool ColorPropertyEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == ToolButton && (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyPress))
    {
        ev->ignore();
        return true;
    }

    return QWidget::eventFilter(obj, ev);
}

QColor ColorPropertyEditor::getColor() const
{
    return Color;
}

