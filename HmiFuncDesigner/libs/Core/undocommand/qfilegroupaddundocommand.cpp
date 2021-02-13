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
    m_data.m_uuid = data.m_uuid;
    foreach(tagFileInfo* f, data.m_files) {
        tagFileInfo *file = new tagFileInfo;
        file->m_file_data = f->m_file_data;
        file->m_exp = f->m_exp;
        file->m_file_name = f->m_file_name;
        file->m_group_uuid = f->m_group_uuid;
        file->m_type = f->m_type;
        file->m_uuid = f->m_uuid;
        m_data.m_files.append(file);
        m_data.m_uuid_to_file.insert(file->m_uuid, file);
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
    QSoftCore::getCore()->getFileManager()->insert_group(&m_data, m_index);
}

void QFileGroupAddUndoCommand::remove()
{
    QSoftCore::getCore()->getFileManager()->remove_group(m_data.m_uuid);
}
