#include "ElementSimpleListWidget.h"
#include "PluginManager.h"
#include <QMimeData>
#include <QDrag>
#include <QApplication>
#include <QDebug>

ElementSimpleListWidget::ElementSimpleListWidget(QListWidget *parent) :
    QListWidget(parent)
{
    setSpacing(5);
    addElements();
}

void ElementSimpleListWidget::addElements() {

    QMap<QString, IDrawApplicationPlugin*> pluginMap = PluginManager::getInstance()->getPluginByType(trUtf8("基本图元"));
    QMapIterator<QString, IDrawApplicationPlugin*> iter(pluginMap);
    while(iter.hasNext())
    {
        iter.next();
        //qDebug() << __FUNCTION__ << __LINE__ << iter.key();
        IDrawApplicationPlugin* plugin = iter.value();
        if(plugin != nullptr) {
            QIcon ico = plugin->getElementIcon();
            QString eleName = plugin->getElementName();
            QListWidgetItem *lineItem = new QListWidgetItem(ico, eleName);
            addItem(lineItem);
        }
    }
}

void ElementSimpleListWidget::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        startPos = event->pos();
    }

    QListWidget::mousePressEvent(event);
}

void ElementSimpleListWidget::mouseMoveEvent(QMouseEvent *event) {

    if (event->buttons() & Qt::LeftButton) {
         int distance = (event->pos() - startPos).manhattanLength();
         if (distance >= QApplication::startDragDistance())
             startDrag();
     }

     QListWidget::mouseMoveEvent(event);
}

void ElementSimpleListWidget::startDrag() {

    QListWidgetItem *item = currentItem();
    if (item) {
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(item->text());
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(currentItem()->icon().pixmap(50,50)));
        drag->start(Qt::MoveAction);
    }
}
