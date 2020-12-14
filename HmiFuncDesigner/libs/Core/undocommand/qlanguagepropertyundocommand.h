#ifndef QLANGUAGEPROPERTYUNDOCOMMAND_H
#define QLANGUAGEPROPERTYUNDOCOMMAND_H

#include "qbaseundocommand.h"

#define LANGUAGE_PROPERTY_UNDO_COMMAND_ID  1002

class CORELIB_EXPORT QLanguagePropertyUndoCommand: public QBaseUndoCommand
{
public:
    QLanguagePropertyUndoCommand(QString property,const QString &uuid,const QVariant &old,
                             const QVariant &now,QUndoCommand* parent=0);

    void    redo();
    void    undo();

    int     id()const;
protected:
    QString     m_property_name;
    QString     m_language_uuid;
    QVariant    m_old_value;
    QVariant    m_new_value;
};

#endif // QLANGUAGEPROPERTYUNDOCOMMAND_H
