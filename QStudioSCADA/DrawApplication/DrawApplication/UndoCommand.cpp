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



ChangePositionCommand::ChangePositionCommand(Figure *fig,
                                             QPointF oldPos,
                                             QUndoCommand *parent)
    : QUndoCommand(parent)
{
    figure = fig;
    this->oldPos = oldPos;
    newPos = fig->pos();
}

void ChangePositionCommand::undo() {

    figure->setPos(oldPos);
    figure->scene()->update();
}

void ChangePositionCommand::redo() {

    figure->setPos(newPos);
    figure->scene()->update();
}


ChangeSizeCommand::ChangeSizeCommand(Figure *fig,
                                     int width,
                                     int height,
                                     QPointF pos,
                                     QUndoCommand *parent)
    : QUndoCommand(parent)
{
    figure = fig;
    oldWidth = width;
    oldHeight = height;
    oldPos = pos;

    newWidth = figure->getElementWidth();
    newHeight = figure->getElementHeight();
    newPos = figure->pos();
}

void ChangeSizeCommand::undo() {

    figure->setElementWidth(oldWidth);
    figure->setElementHeight(oldHeight);
    figure->setElementXPos(oldPos.x());
    figure->setElementYPos(oldPos.y());
    figure->updateBoundingElement();
    figure->scene()->update();
}

void ChangeSizeCommand::redo() {

    figure->setElementWidth(newWidth);
    figure->setElementHeight(newHeight);
    figure->setElementXPos(newPos.x());
    figure->setElementYPos(newPos.y());
    figure->scene()->update();
}
