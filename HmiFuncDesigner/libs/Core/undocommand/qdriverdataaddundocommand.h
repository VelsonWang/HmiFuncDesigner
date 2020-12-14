#ifndef QDRIVERDATAADDUNDOCOMMAND_H
#define QDRIVERDATAADDUNDOCOMMAND_H

#include "qbaseundocommand.h"
#include "../../shared/driver/qabstractdriver.h"

#define DRIVER_DATA_ADD_UNDO_COMMAND_ID  1013

enum enDriverDataAddType
{
    DDAT_ADD,
    DDAT_REMOVE,
    DDAT_EMPTY
};

class CORELIB_EXPORT QDriverDataAddUndoCommand: public QBaseUndoCommand
{
public:
    QDriverDataAddUndoCommand(const QString &uuid,
                              const tagDriverDataInfo &data,
                              int index,
                              enDriverDataAddType type,
                              QUndoCommand* parent=0);

    void    redo();
    void    undo();

    int     id()const;
protected:
    void    add();
    void    remove();
protected:
    tagDriverDataInfo       m_data;
    QString                 m_driver_uuid;
    enDriverDataAddType     m_type;
    int                     m_index;
};
#endif // QDRIVERDATAADDUNDOCOMMAND_H
