#ifndef QHOSTPARENTCHANGEDUNDOCOMMAND_H
#define QHOSTPARENTCHANGEDUNDOCOMMAND_H

#include "qbaseundocommand.h"

class QAbstractHost;

#define HOST_PARENT_CHANGED_UNDO_COMMAND    1005

class CORELIB_EXPORT QHostParentChangedUndoCommand : public QBaseUndoCommand
{
public:
    QHostParentChangedUndoCommand(QAbstractHost* host,
                                  QAbstractHost* old_parent,
                                  int           old_index,
                                  QAbstractHost* new_parent,
                                  int           new_index,
                                  QUndoCommand   *parent=0);
    void    redo();
    void    undo();

    int     id()const;
protected:
    QAbstractHost*      m_host;
    QAbstractHost*      m_old_parent;
    int                 m_old_parent_index;
    QAbstractHost*      m_new_parent;
    int                 m_new_parent_index;
};

#endif // QHOSTPARENTCHANGEDUNDOCOMMAND_H
