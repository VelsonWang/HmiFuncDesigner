#include "coloreditor.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QFocusEvent>
#include <QColorDialog>

ColorEditor::ColorEditor(QWidget *parent) :
    QWidget(parent),
    color(Qt::white)
{
    toolButton = new QToolButton(this);
    toolButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    toolButton->setText(tr("..."));
    toolButton->setFixedWidth(20);
    toolButton->installEventFilter(this);
    setFocusProxy(toolButton);
    setFocusPolicy(toolButton->focusPolicy());
    connect(toolButton, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    colorLabel = new QLabel(this);
    QString styleSheet = QString("background:%1;").arg(color.name());
    colorLabel->setStyleSheet(styleSheet);
    colorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(colorLabel);
    layout->addWidget(toolButton);
}

ColorEditor::~ColorEditor()
{

}

void ColorEditor::setColor(const QColor &c)
{
    if (color != c) {
        color = c;
        QString styleSheet = QString("background:%1;").arg(color.name());
        colorLabel->setStyleSheet(styleSheet);
    }
}

void ColorEditor::onToolButtonClicked()
{
    QColor newColor = QColorDialog::getColor(color, this, tr("选择颜色"));
    if (newColor.isValid() && newColor != color) {
        setColor(newColor);
    }
}

bool ColorEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == toolButton && (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyRelease)) {
        ev->ignore();
        return true;
    }
    return QWidget::eventFilter(obj, ev);
}

QColor ColorEditor::getColor() const
{
    return color;
}

