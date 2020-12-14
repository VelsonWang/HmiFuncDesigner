#ifndef QDRIVERDATACHANGEDUNDOCOMMAND_H
#define QDRIVERDATACHANGEDUNDOCOMMAND_H

#include "qbaseundocommand.h"
#include "../../shared/driver/qabstractdriver.h"

#define DRIVER_DATA_CHANGED_UNDO_COMMAND_ID  1014

class CORELIB_EXPORT QDriverDataChangedUndoCommand: public QBaseUndoCommand
{
public:
    QDriverDataChangedUndoCommand(const QString &driver_uuid,
                                  const QString &data_uuid,
                                  const QString &key,
                                  const QVariant &old_value,
                                  const QVariant &new_value,
                                  QUndoCommand* parent=0);

    void    redo();
    void    undo();

    int     id()const;

    bool    mergeWith(const QUndoCommand *other);
protected:
    QString                 m_driver_uuid;
    QString                 m_data_uuid;
    QString                 m_key;
    QVariant                m_old_value;
    QVariant                m_new_value;
};

#endif // QDRIVERDATACHANGEDUNDOCOMMAND_H
