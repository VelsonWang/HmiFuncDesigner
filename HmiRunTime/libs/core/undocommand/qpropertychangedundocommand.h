#ifndef QPROPERTYCHANGEDUNDOCOMMAND_H
#define QPROPERTYCHANGEDUNDOCOMMAND_H

#include "qbaseundocommand.h"

#define PROPERTY_UNDO_COMMAND   1004

class CORELIB_EXPORT QPropertyChangedUndoCommand : public QBaseUndoCommand
{
public:
    QPropertyChangedUndoCommand(const QString &host_uuid,
                                const QString &property_name,
                                const QVariant &old_value,
                                const QVariant& new_value,
                                QUndoCommand* parent = 0);
    int id() const;
    bool mergeWith(const QUndoCommand *other);

    void redo();
    void undo();

protected:
    QString m_host_uuid;
    QString m_property_name;
    QVariant m_old_value;
    QVariant m_new_value;
};

#endif // QPROPERTYCHANGEDUNDOCOMMAND_H
