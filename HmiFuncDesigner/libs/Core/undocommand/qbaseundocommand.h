#ifndef QBASEUNDOCOMMAND_H
#define QBASEUNDOCOMMAND_H

#include "../corelibglobal.h"

#include <QUndoCommand>
#include <QVariant>

class CORELIB_EXPORT QBaseUndoCommand : public QUndoCommand
{
public:
    QBaseUndoCommand(QUndoCommand* parent = 0);
};

#endif // QBASEUNDOCOMMAND_H
