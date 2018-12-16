
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStyleOptionComboBox>

#include "Tag.h"

const int MaxTagTableColumns = 6;


Qt::ItemFlags TagTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
        theFlags |= Qt::ItemIsSelectable|/*Qt::ItemIsEditable|*/
                    Qt::ItemIsEnabled;
    return theFlags;
}


QVariant TagTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
        index.row() < 0 || index.row() >= m_tagItems.count() ||
        index.column() < 0 || index.column() >= MaxTagTableColumns)
        return QVariant();
    const TagItem *item = m_tagItems.at(index.row());
    if (role == Qt::SizeHintRole)
    {
        QStyleOptionComboBox option;
        switch (index.column())
        {
            case TagID: option.currentText = item->mId ;break;
            case Name: option.currentText = item->mName; break;
            case Description: option.currentText = item->mDescription; break;
            case DeviceInfo: option.currentText = item->mDeviceInfo; break;
            case LogicValue: option.currentText = item->mLogicValue; break;
            case Status: option.currentText = item->mStatus;break;
            default: Q_ASSERT(false);
        }

        QFontMetrics fontMetrics(data(index, Qt::FontRole)
                                 .value<QFont>());
        option.fontMetrics = fontMetrics;
        QSize size(fontMetrics.width(option.currentText),
                   fontMetrics.height());
        return qApp->style()->sizeFromContents(QStyle::CT_ComboBox,
                                               &option, size);
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
            case TagID: return item->mId;
            case Name: return item->mName;
            case Description: return item->mDescription;
            case DeviceInfo: return item->mDeviceInfo;
            case LogicValue: return item->mLogicValue;
            case Status: return item->mStatus;
            default: Q_ASSERT(false);
        }
    }
    return QVariant();
}


QVariant TagTableModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case TagID: return tr("内存编号");
            case Name: return tr("名称");
            case Description: return tr("描述");
            case DeviceInfo: return tr("设备信息");
            case LogicValue: return tr("逻辑值");
            case Status: return tr("状态 报警");
            default: Q_ASSERT(false);
        }
    }
    return section + 1;
}


int TagTableModel::rowCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : m_tagItems.count();
}


int TagTableModel::columnCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : MaxTagTableColumns;
}


bool TagTableModel::setData(const QModelIndex &index,
                         const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole ||
        index.row() < 0 || index.row() >= m_tagItems.count() ||
        index.column() < 0 || index.column() >= MaxTagTableColumns)
        return false;

    TagItem *item = m_tagItems[index.row()];
    switch (index.column())
    {
        case TagID: item->mId = value.toInt(); break;
        case Name: item->mName = value.toString(); break;
        case Description: item->mDescription = value.toString(); break;
        case DeviceInfo: item->mDeviceInfo = value.toString(); break;
        case LogicValue: item->mLogicValue = value.toString(); break;
        case Status: item->mStatus = value.toString(); break;
        default: Q_ASSERT(false);
    }
    emit dataChanged(index, index);
    return true;
}


bool TagTableModel::insertRows(int row, int count, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_tagItems.insert(row, new TagItem());
    endInsertRows();
    return true;
}


bool TagTableModel::removeRows(int row, int count, const QModelIndex&)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_tagItems.removeAt(row);
    endRemoveRows();
    return true;
}


void TagTableModel::load(QList<TagItem *> items, int start, int end)
{
    m_tagItems.clear();
    for (int i = start; i < end+1; i++)
    {
        m_tagItems.append(items.at(i));
    }
}

void TagTableModel::ClearItems()
{
    m_tagItems.clear();
}

void TagTableModel::AppendRow(TagItem * item)
{
    int lastNextRow = m_tagItems.size();
    beginInsertRows(QModelIndex(), lastNextRow, lastNextRow);
    m_tagItems.append(item);
    endInsertRows();
}


void TagTableModel::InsertRow(int i, TagItem * item)
{
    beginInsertRows(QModelIndex(), i, i);
    m_tagItems.insert(i, item);
    endInsertRows();
}


TagItem *TagTableModel::GetRow(int i)
{
    if(i < m_tagItems.size())
        return m_tagItems.at(i);
    return NULL;
}


void TagTableModel::UpdateRow(int i, TagItem * item)
{
    if(i<m_tagItems.size())
    {
        m_tagItems.replace(i, item);
        for(int c=0; c<MaxTagTableColumns; c++)
            emit dataChanged(this->index(i, c), this->index(i, c));
    }
}


void TagTableModel::UpdateLogicValueAndStatus(int id, QString logicValue, QString status)
{
    for(int i=0; i<m_tagItems.size(); i++)
    {
        if(id == m_tagItems.at(i)->mId)
        {
            m_tagItems.at(i)->mLogicValue = logicValue;
            m_tagItems.at(i)->mStatus = status;
            emit dataChanged(this->index(i, LogicValue), this->index(i, LogicValue));
            emit dataChanged(this->index(i, Status), this->index(i, Status));
            break;
        }
    }
}




