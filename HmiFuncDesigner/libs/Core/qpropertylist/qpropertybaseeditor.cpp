#include "qpropertybaseeditor.h"
#include "../../shared/property/qabstractproperty.h"
#include "../../shared/qpropertyfactory.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QEvent>
#include <QApplication>
#include <QLabel>

QPropertyBaseEditor::QPropertyBaseEditor(QAbstractProperty *pro, QUndoStack* stack, QWidget *parent)
    : QWidget(parent),
      property(pro)
{
    this->setFocusPolicy(Qt::StrongFocus);
    resetButton = new QToolButton(this);
    resetButton->setVisible(property->getAttribute(ATTR_RESET_ABLEABLE).toBool());
    resetButton->setIcon(QIcon(":/images/reset.png"));
    resetButton->setIconSize(QSize(8, 8));
    resetButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    resetButton->setFocusPolicy(Qt::NoFocus);
    resetButton->setFocusProxy(this);
    if(property) {
        resetButton->setEnabled(property->modified());
    }
    connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
    connect(property, SIGNAL(refresh()), this, SLOT(onPropertyRefresh()));

    this->setProperty("no-ManhattanStyle", true);

    widget = QPropertyFactory::createEditor(pro->getObjectProperty("type").toString(), pro, stack);
    if(widget == NULL) {
        QLabel *lablel = new QLabel;
        lablel->setFocusPolicy(Qt::StrongFocus);
        lablel->setText(pro->getValueText());
        lablel->setMargin(3);
        widget = lablel;
    }
    this->setFocusProxy(widget);
    widget->installEventFilter(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(widget);
    layout->addWidget(resetButton);

    this->setLayout(layout);
}

void QPropertyBaseEditor::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(this->rect(), Qt::white);
}

void QPropertyBaseEditor::reset()
{
    resetButton->setEnabled(false);
    if(property) {
        property->reset();
    }
}

bool QPropertyBaseEditor::eventFilter(QObject *o, QEvent *e)
{
    if(o == widget && e->type() == QEvent::FocusOut) {
        qApp->sendEvent(this, e);
    }
    return QWidget::eventFilter(o, e);
}

void QPropertyBaseEditor::onPropertyRefresh()
{
    if(property) {
        resetButton->setEnabled(property->modified());
    }
}
