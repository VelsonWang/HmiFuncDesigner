#include "qcommoneditor.h"
#include "../../shared/property/qabstractproperty.h"
#include <QHBoxLayout>

QCommonEditor::QCommonEditor(QAbstractProperty *property, QUndoStack *, QWidget *parent):
    QWidget(parent),
    icon(new QLabel(this)),
    text(new QLabel(this)),
    property(property)
{
    this->setFocusPolicy(Qt::StrongFocus);
    text->setContentsMargins(0, 1, 0, 0);
    text->setText(property->get_value_text());
    if(property->get_value_icon().isNull()) {
        icon->setVisible(false);
    } else {
        QPixmap pix = property->get_value_icon().pixmap(QSize(16, 16));
        icon->setFixedSize(16, 16);
        icon->setPixmap(pix);
    }

    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);
    layout->setContentsMargins(3, 2, 3, 2);
    layout->setSpacing(3);
    layout->addWidget(icon);
    layout->addWidget(text);
    connect(property, SIGNAL(refresh()), this, SLOT(onPropertyChanged()));
}

void QCommonEditor::onPropertyChanged()
{
    if(property) {
        text->setText(property->get_value_text());
        if(property->get_value_icon().isNull()) {
            icon->setVisible(false);
        } else {
            QPixmap pix = property->get_value_icon().pixmap(QSize(16, 16));
            icon->setFixedSize(16, 16);
            icon->setPixmap(pix);
        }
    }
}
