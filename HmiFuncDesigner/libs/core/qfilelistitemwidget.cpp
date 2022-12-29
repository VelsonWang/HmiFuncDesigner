#include "qfilelistitemwidget.h"
#include "ui_qfilelistitemwidget.h"
#include "qfilelistview.h"
#include "qtoolbarbutton.h"
#include "qfilemanager.h"

QFileListItemWidget::QFileListItemWidget(QFileListView *view,
        QTreeWidgetItem *item,
        QWidget *parent):
    QWidget(parent),
    ui(new Ui::QFileListItemWidget),
    m_item(item),
    m_view(view)
{
    ui->setupUi(this);
    QAction *ac;
    ac = new QAction(QIcon(":/images/removesubmitfield.png"), "", this);
    ac->setToolTip("Remove");
    m_removeButton = new QToolBarButton(ac, this);
    ui->horizontalLayout->addWidget(m_removeButton);
    connect(ac, SIGNAL(triggered()), this, SIGNAL(remove()));

    m_removeButton->setVisible(false);
}

QFileListItemWidget::~QFileListItemWidget()
{
    delete ui;
}

void QFileListItemWidget::setText(const QString &text)
{
    ui->text->setText(text);
    this->setToolTip(text);
}

void QFileListItemWidget::enterEvent(QEvent *)
{
    if(m_view->canRemove()) {
        tagFileGroupInfo *g = m_view->getGroup(m_item);
        if(g != NULL) {
            foreach(tagFileInfo *info, g->m_files) {
                if(info->m_type == FILE_SYSTEM) {
                    return;
                }
            }
            m_removeButton->setVisible(true);
        } else {
            tagFileInfo *f = m_view->getFile(m_item);
            m_removeButton->setVisible(f->m_type != FILE_SYSTEM);
        }
    }
}

void QFileListItemWidget::leaveEvent(QEvent *)
{
    m_removeButton->setVisible(false);
}

void QFileListItemWidget::setIcon(const QString &icon)
{
    ui->icon->setPixmap(QPixmap(icon));
}
