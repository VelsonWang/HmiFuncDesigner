#ifndef QUSERADDUNDOCOMMAND_H
#define QUSERADDUNDOCOMMAND_H

#include "qbaseundocommand.h"
#include "../../shared/qusermanager.h"

#define USER_ADD_UNDO_COMMAND_ID 1001

enum enUserAddType
{
    UAT_ADD,
    UAT_REMOVE,
    UAT_EMPTY
};

class CORELIB_EXPORT QUserAddUndoCommand : public QBaseUndoCommand
{
public:
    QUserAddUndoCommand(tagUserInfo* user,enUserAddType type,int index=-1,QUndoCommand* parent=0);

    int id() const;

    void    redo();
    void    undo();

protected:
    void    add();
    void    remove();
protected:
    tagUserInfo     m_user_info;
    enUserAddType   m_type;
    int             m_index;
};

#endif // QUSERADDUNDOCOMMAND_H
