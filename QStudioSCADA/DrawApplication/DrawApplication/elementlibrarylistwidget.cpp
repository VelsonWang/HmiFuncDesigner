#include "elementlibrarylistwidget.h"
#include "elementlibrarylistwidgetitem.h"
#include <QFileDialog>
#include <QApplication>
#include <QtDebug>
#include <QMimeData>
#include <QDrag>

ElementLibraryListWidget::ElementLibraryListWidget(QListWidget *parent)
    : QListWidget(parent)
{
    setSpacing(5);
}

void ElementLibraryListWidget::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        startPos = event->pos();
    }

    QListWidget::mousePressEvent(event);
}

void ElementLibraryListWidget::mouseMoveEvent(QMouseEvent *event) {

    if (event->buttons() & Qt::LeftButton) {
         int distance = (event->pos() - startPos).manhattanLength();
         if (distance >= QApplication::startDragDistance())
             startDrag();
     }

     QListWidget::mouseMoveEvent(event);
}

void ElementLibraryListWidget::startDrag() {

    ElementLibraryListWidgetItem *item = dynamic_cast<ElementLibraryListWidgetItem*>(currentItem());
    if (item) {
        QMimeData *mimeData = new QMimeData;
        QByteArray data;
        QDataStream out(&data,QIODevice::WriteOnly);
        out << item->getFilePath();
        mimeData->setData("rti/lib",data);
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(item->icon().pixmap(50,50)));
        drag->start(Qt::MoveAction);
    }
}

void ElementLibraryListWidget::loadLibrary() {

    const QString &filename = QFileDialog::getOpenFileName(this,trUtf8("Open"),".",
                                                           trUtf8("Mnemo designer library (*.mdlib)"));
    QFileInfo fi(filename);


    if (filename.isEmpty()) {
        return;
    }

    if (!filename.toLower().endsWith(".mdlib")) {
        return;
    }

    ElementLibraryListWidgetItem *item = new ElementLibraryListWidgetItem;
    item->setText(fi.baseName());
    item->setIcon(QIcon(":/images/library.png"));
    item->setFilePath(filename);
    addItem(item);
}
