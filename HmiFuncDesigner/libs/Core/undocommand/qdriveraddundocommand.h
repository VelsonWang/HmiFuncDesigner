#ifndef QDRIVERADDUNDOCOMMAND_H
#define QDRIVERADDUNDOCOMMAND_H

#include "qbaseundocommand.h"
#include "../../shared/driver/qabstractdriver.h"

#define DRIVER_ADD_UNDO_COMMAND_ID  1015

enum enDriverAddType
{
    DRAT_ADD,
    DRAT_REMOVE,
    DRAT_EMPTY
};

class CORELIB_EXPORT QDriverAddUndoCommand: public QBaseUndoCommand
{
public:
    QDriverAddUndoCommand(QAbstractDriver* driver,
                          int index,
                          enDriverAddType type,
                          QUndoCommand* parent=0);

    ~QDriverAddUndoCommand();

    void    redo();
    void    undo();

    int     id()const;
protected:
    void    add();
    void    remove();
protected:
    QAbstractDriver*        m_driver;
    enDriverAddType         m_type;
    int                     m_index;
};

#endif // QDRIVERADDUNDOCOMMAND_H
