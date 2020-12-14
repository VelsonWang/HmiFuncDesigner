#ifndef QLANGUAGEADDUNDOCOMMAND_H
#define QLANGUAGEADDUNDOCOMMAND_H

#include "qbaseundocommand.h"

#define LANGUAGE_ADD_UNDO_COMMAND 1003

class QLanguage;

enum enLanguageAddType
{
    LAT_ADD,
    LAT_REMOVE,
    LAT_EMPTY
};

class CORELIB_EXPORT QLanguageAddUndoCommand : public QBaseUndoCommand
{
public:
    QLanguageAddUndoCommand(QLanguage* l,int index,enLanguageAddType type,QUndoCommand *parent=0);
    ~QLanguageAddUndoCommand();

    void    redo();
    void    undo();
    int     id() const;
protected:
    void    add();
    void    remove();
protected:
    QLanguage       *m_language;
    enLanguageAddType   m_type;
    int             m_index;
};

#endif // QLANGUAGEADDUNDOCOMMAND_H
