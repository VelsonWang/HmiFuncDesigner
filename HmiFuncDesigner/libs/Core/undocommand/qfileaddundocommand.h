#ifndef QFILEADDUNDOCOMMAND_H
#define QFILEADDUNDOCOMMAND_H

#include "qbaseundocommand.h"
#include "../qfilemanager.h"

#define FILE_ADD_UNDO_COMMAND_ID  1012

enum enFileAddType
{
    FAT_ADD,
    FAT_REMOVE,
    FAT_EMPTY
};

class CORELIB_EXPORT QFileAddUndoCommand : public QBaseUndoCommand
{
public:
    QFileAddUndoCommand(const tagFileInfo &data, int index, enFileAddType type, QUndoCommand* parent=0);

    void redo();
    void undo();

    int id()const;

protected:
    void add();
    void remove();

protected:
    enFileAddType m_type;
    int m_index;
    tagFileInfo m_data;
};

#endif // QFILEADDUNDOCOMMAND_H
