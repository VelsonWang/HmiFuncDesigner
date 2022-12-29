#include "qitemlistwidget.h"
#include "ui_qitemlistwidget.h"
#include "../../qtoolbarbutton.h"

QItemListWidget::QItemListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QItemListWidget)
{
    ui->setupUi(this);

    QAction *ac;

    ac = new QAction(QIcon(":/images/changed.png"), "", this);
    ac->setToolTip("Remove");
    m_changedButton = new QToolBarButton(ac, this);
    ui->horizontalLayout->addWidget(m_changedButton);
    connect(ac, SIGNAL(triggered()), this, SIGNAL(changed()));

    ac = new QAction(QIcon(":/images/removesubmitfield.png"), "", this);
    ac->setToolTip("Remove");
    m_removeButton = new QToolBarButton(ac, this);
    ui->horizontalLayout->addWidget(m_removeButton);
    connect(ac, SIGNAL(triggered()), this, SIGNAL(remove()));

    m_removeButton->setVisible(false);
    m_changedButton->setVisible(false);
}

QItemListWidget::~QItemListWidget()
{
    delete ui;
}

void QItemListWidget::setText(const QString &text)
{
    if(text == "Normal") {
        m_removeButton->setVisible(false);
        m_changedButton->setVisible(false);
    }
    ui->text->setText(text);
    this->setToolTip(text);
}

void QItemListWidget::enterEvent(QEvent *)
{
    if(ui->text->text() != "Normal") {
        m_removeButton->setVisible(true);
        m_changedButton->setVisible(true);
    }
}

void QItemListWidget::leaveEvent(QEvent *)
{
    m_removeButton->setVisible(false);
    m_changedButton->setVisible(false);
}

void QItemListWidget::setIcon(const QString &icon)
{
    ui->icon->setPixmap(QPixmap(icon));
}
