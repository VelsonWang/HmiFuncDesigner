#include "qfileaddundocommand.h"


#include "../qsoftcore.h"

QFileAddUndoCommand::QFileAddUndoCommand(const tagFileInfo &data,
        int index,
        enFileAddType type,
        QUndoCommand *parent) :
    QBaseUndoCommand(parent),
    m_type(type),
    m_index(index)
{
    m_data.m_file_data = data.m_file_data;
    m_data.m_file_name = data.m_file_name;
    m_data.m_exp = data.m_exp;
    m_data.m_group_id = data.m_group_id;
    m_data.m_type = data.m_type;
    m_data.m_id = data.m_id;
}

void QFileAddUndoCommand::redo()
{
    QBaseUndoCommand::redo();
    if(m_type == FAT_ADD) {
        add();
    } else if(m_type == FAT_REMOVE) {
        remove();
    }
}

void QFileAddUndoCommand::undo()
{
    QBaseUndoCommand::undo();
    if(m_type == FAT_ADD) {
        remove();
    } else if(m_type == FAT_REMOVE) {
        add();
    }
}

int QFileAddUndoCommand::id() const
{
    return FILE_ADD_UNDO_COMMAND_ID;
}

void QFileAddUndoCommand::add()
{
    QSoftCore::getCore()->getFileManager()->insertFile(m_data.m_group_id, &m_data, m_index);
}

void QFileAddUndoCommand::remove()
{
    QSoftCore::getCore()->getFileManager()->removeFile(m_data.m_group_id, m_data.m_id);
}
