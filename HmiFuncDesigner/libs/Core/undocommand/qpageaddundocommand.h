#ifndef QPAGEADDUNDOCOMMAND_H
#define QPAGEADDUNDOCOMMAND_H

#include "qbaseundocommand.h"

#define PAGE_ADD_UNDO_COMMAND_ID  1016

enum enPageAddType
{
    PAT_ADD,
    PAT_REMOVE,
    PAT_EMPTY
};

class QAbstractHost;

class CORELIB_EXPORT QPageAddUndoCommand : public QBaseUndoCommand
{
public:
    QPageAddUndoCommand(QAbstractHost* page,
                        int index,
                        enPageAddType type,
                        QUndoCommand* parent=0);

    ~QPageAddUndoCommand();

    void redo();
    void undo();

    int id()const;

protected:
    void add();
    void remove();

protected:
    QAbstractHost* m_page;
    enPageAddType m_type;
    int m_index;
};

#endif // QPAGEADDUNDOCOMMAND_H
