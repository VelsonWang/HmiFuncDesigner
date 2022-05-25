#include "qfilegroupaddundocommand.h"

#include "../qsoftcore.h"

QFileGroupAddUndoCommand::QFileGroupAddUndoCommand(const tagFileGroupInfo &data,
        int index,
        enFileGroupAddType type,
        QUndoCommand *parent) :
    QBaseUndoCommand(parent),
    m_type(type),
    m_index(index)
{
    m_data.m_group_name = data.m_group_name;
    m_data.m_id = data.m_id;
    foreach(tagFileInfo* f, data.m_files) {
        tagFileInfo *file = new tagFileInfo;
        file->m_file_data = f->m_file_data;
        file->m_exp = f->m_exp;
        file->m_file_name = f->m_file_name;
        file->m_group_id = f->m_group_id;
        file->m_type = f->m_type;
        file->m_id = f->m_id;
        m_data.m_files.append(file);
        m_data.m_id_to_file.insert(file->m_id, file);
    }
}

void QFileGroupAddUndoCommand::redo()
{
    QBaseUndoCommand::redo();
    if(m_type == FGAT_ADD) {
        add();
    } else if(m_type == FGAT_REMOVE) {
        remove();
    }
}

void QFileGroupAddUndoCommand::undo()
{
    QBaseUndoCommand::undo();
    if(m_type == FGAT_ADD) {
        remove();
    } else if(m_type == FGAT_REMOVE) {
        add();
    }
}

int QFileGroupAddUndoCommand::id() const
{
    return FILE_GROUP_ADD_UNDO_COMMAND_ID;
}

void QFileGroupAddUndoCommand::add()
{
    QSoftCore::getCore()->getFileManager()->insertGroup(&m_data, m_index);
}

void QFileGroupAddUndoCommand::remove()
{
    QSoftCore::getCore()->getFileManager()->removeGroup(m_data.m_id);
}
