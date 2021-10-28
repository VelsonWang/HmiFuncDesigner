#include "coloreditor.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QFocusEvent>
#include <QColorDialog>

ColorEditor::ColorEditor(QWidget *parent) :
    QWidget(parent),
    m_color(Qt::white)
{
    m_toolButton = new QToolButton(this);
    m_toolButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    m_toolButton->setText(tr("..."));
    m_toolButton->setFixedWidth(20);
    m_toolButton->installEventFilter(this);
    setFocusProxy(m_toolButton);
    setFocusPolicy(m_toolButton->focusPolicy());
    connect(m_toolButton, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    m_colorLabel = new QLabel(this);
    QString styleSheet = QString("background:%1;").arg(m_color.name());
    m_colorLabel->setStyleSheet(styleSheet);
    m_colorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(m_colorLabel);
    layout->addWidget(m_toolButton);
}

ColorEditor::~ColorEditor()
{

}

void ColorEditor::setColor(const QColor &c)
{
    if (m_color != c) {
        m_color = c;
        QString styleSheet = QString("background:%1;").arg(m_color.name());
        m_colorLabel->setStyleSheet(styleSheet);
    }
}

void ColorEditor::onToolButtonClicked()
{
    QColor newColor = QColorDialog::getColor(m_color, this, tr("选择颜色"));
    if (newColor.isValid() && newColor != m_color) {
        setColor(newColor);
    }
}

bool ColorEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == m_toolButton && (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyRelease)) {
        ev->ignore();
        return true;
    }
    return QWidget::eventFilter(obj, ev);
}

QColor ColorEditor::getColor() const
{
    return m_color;
}

