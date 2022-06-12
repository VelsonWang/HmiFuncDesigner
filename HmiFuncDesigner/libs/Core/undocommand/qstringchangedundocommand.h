#ifndef QSTRINGCHANGEDUNDOCOMMAND_H
#define QSTRINGCHANGEDUNDOCOMMAND_H

#include "qbaseundocommand.h"

#define STRING_CHANGED_UNDO_COMMAND   1006

class CORELIB_EXPORT QStringChangedUndoCommand : public QBaseUndoCommand
{
public:
    QStringChangedUndoCommand(const QString &host_id,
                              const QString &property_name,
                              const QString &id,
                              bool old_tr,
                              bool new_tr,
                              const QString &old_text = "",
                              const QString &new_text = "",
                              const QMap<QString, QString> &old_translate = QMap<QString, QString>(),
                              const QMap<QString, QString> &new_translate = QMap<QString, QString>(),
                              QUndoCommand* parent = 0);
    int id() const;
    void redo();
    void undo();

protected:
    QString m_host_id;
    QString m_property_name;
    QString m_id;
    bool m_old_tr;
    bool m_new_tr;
    QString m_old_text;
    QString m_new_text;
    QMap<QString, QString> m_old_translates;
    QMap<QString, QString> m_new_translates;
};
#endif // QSTRINGCHANGEDUNDOCOMMAND_H
