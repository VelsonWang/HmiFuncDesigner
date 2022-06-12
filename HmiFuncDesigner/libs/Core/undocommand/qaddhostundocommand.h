#ifndef QADDHOSTUNDOCOMMAND_H
#define QADDHOSTUNDOCOMMAND_H

#include "qbaseundocommand.h"
#include "../../shared/xmlobject.h"

#include <QMap>

class QAbstractHost;

#define HOST_ADD_UNDO_COMMAND  1007

enum enAddHostType
{
    AHT_ADD,
    AHT_REMOVE
};

class CORELIB_EXPORT QAddHostUndoCommand : public QBaseUndoCommand
{
public:
    QAddHostUndoCommand(QAbstractHost *parent_host,
                        const QList<QAbstractHost*> &hosts,
                        const QList<int> &indexs,
                        enAddHostType type,
                        QUndoCommand* parent = 0);
    ~QAddHostUndoCommand();

    void redo();
    void undo();

    int id()const;

protected:
    void add();
    void remove();

protected:
    QAbstractHost *m_parent_host;
    QList<QAbstractHost*> m_hosts;
    QList<int> m_indexs;
    enAddHostType m_type;
};


#endif // QADDHOSTUNDOCOMMAND_H
