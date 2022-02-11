#include "qcommoneditor.h"
#include "../../shared/property/qabstractproperty.h"
#include <QHBoxLayout>

QCommonEditor::QCommonEditor(QAbstractProperty *property, QUndoStack *, QWidget *parent) :
    QWidget(parent),
    m_icon(new QLabel(this)),
    m_text(new QLabel(this)),
    m_property(property)
{
    this->setFocusPolicy(Qt::StrongFocus);
    m_text->setContentsMargins(0, 1, 0, 0);
    m_text->setText(property->getValueText());
    if(property->getValueIcon().isNull()) {
        m_icon->setVisible(false);
    } else {
        QPixmap pix = property->getValueIcon().pixmap(QSize(16, 16));
        m_icon->setFixedSize(16, 16);
        m_icon->setPixmap(pix);
    }

    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);
    layout->setContentsMargins(3, 2, 3, 2);
    layout->setSpacing(3);
    layout->addWidget(m_icon);
    layout->addWidget(m_text);
    connect(property, SIGNAL(refresh()), this, SLOT(onPropertyChanged()));
}

void QCommonEditor::onPropertyChanged()
{
    if(m_property) {
        m_text->setText(m_property->getValueText());
        if(m_property->getValueIcon().isNull()) {
            m_icon->setVisible(false);
        } else {
            QPixmap pix = m_property->getValueIcon().pixmap(QSize(16, 16));
            m_icon->setFixedSize(16, 16);
            m_icon->setPixmap(pix);
        }
    }
}
