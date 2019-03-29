#include "UndoCommand.h"
#include <QtDebug>
#include <exception>


AddCommand::AddCommand(const QList<QGraphicsItem *> &addItems,
                       GraphPage *graphPage,
                       QUndoCommand *parent)
    : QUndoCommand(parent)
{
    items = addItems;
    this->graphPage = graphPage;
}

void AddCommand::undo() {

    foreach (QGraphicsItem *item, items) {
        graphPage->removeItem(item);
    }
    graphPage->removeElementEvent();
}

void AddCommand::redo() {

    foreach (QGraphicsItem *item, items) {
        graphPage->addItem(item);
    }
    graphPage->addElementEvent();
}


RemoveCommand::RemoveCommand(const QList<QGraphicsItem *> &delItems,
                             GraphPage *graphPage,
                             QUndoCommand *parent)
    : QUndoCommand(parent)
{

    items = delItems;
    this->graphPage  = graphPage;
}

void RemoveCommand::undo() {

    foreach (QGraphicsItem *item, items) {
        graphPage->addItem(item);
    }
    graphPage->addElementEvent();
}

void RemoveCommand::redo() {

    foreach (QGraphicsItem *item, items) {
        graphPage->removeItem(item);
    }
    graphPage->removeElementEvent();
}



ChangePositionCommand::ChangePositionCommand(Element *element,
                                             QPointF oldPos,
                                             QUndoCommand *parent)
    : QUndoCommand(parent)
{
    element_ = element;
    this->oldPos = oldPos;
    newPos = element->pos();
}

void ChangePositionCommand::undo() {

    element_->setPos(oldPos);
    element_->scene()->update();
}

void ChangePositionCommand::redo() {

    element_->setPos(newPos);
    element_->scene()->update();
}


ChangeSizeCommand::ChangeSizeCommand(Element *element,
                                     int width,
                                     int height,
                                     QPointF pos,
                                     QUndoCommand *parent)
    : QUndoCommand(parent)
{
    element_ = element;
    oldWidth = width;
    oldHeight = height;
    oldPos = pos;

    newWidth = element_->getElementWidth();
    newHeight = element_->getElementHeight();
    newPos = element_->pos();
}

void ChangeSizeCommand::undo() {

    element_->setElementWidth(oldWidth);
    element_->setElementHeight(oldHeight);
    element_->setElementXPos(oldPos.x());
    element_->setElementYPos(oldPos.y());
    element_->updateBoundingElement();
    element_->scene()->update();
}

void ChangeSizeCommand::redo() {

    element_->setElementWidth(newWidth);
    element_->setElementHeight(newHeight);
    element_->setElementXPos(newPos.x());
    element_->setElementYPos(newPos.y());
    element_->scene()->update();
}
