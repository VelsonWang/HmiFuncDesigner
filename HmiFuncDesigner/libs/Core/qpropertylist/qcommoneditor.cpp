#include "qcommoneditor.h"

#include "../../shared/property/qabstractproperty.h"

#include <QHBoxLayout>

QCommonEditor::QCommonEditor(QAbstractProperty *property, QUndoStack *, QWidget *parent):
    QWidget(parent),
    m_icon(new QLabel(this)),
    m_text(new QLabel(this)),
    m_property(property)
{
    this->setFocusPolicy(Qt::StrongFocus);
    m_text->setContentsMargins(0,1,0,0);
    m_text->setText(m_property->get_value_text());
    QIcon icon=m_property->get_value_icon();
    if(icon.isNull())
    {
        m_icon->setVisible(false);
    }
    else
    {
        QPixmap pix=icon.pixmap(QSize(16,16));
        m_icon->setFixedSize(16,16);
        m_icon->setPixmap(pix);
    }

    QHBoxLayout *l=new QHBoxLayout;
    this->setLayout(l);
    l->setContentsMargins(3,2,3,2);
    l->setSpacing(3);
    l->addWidget(m_icon);
    l->addWidget(m_text);
    connect(m_property,SIGNAL(refresh()),this,SLOT(property_changed()));
}

void QCommonEditor::property_changed()
{
    m_text->setText(m_property->get_value_text());
    QIcon icon=m_property->get_value_icon();
    if(icon.isNull())
    {
        m_icon->setVisible(false);
    }
    else
    {
        QPixmap pix=icon.pixmap(QSize(16,16));
        m_icon->setFixedSize(16,16);
        m_icon->setPixmap(pix);
    }
}
