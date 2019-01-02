
#include "VariableManagerWin.h"
#include "ui_VariableManagerWin.h"
#include "CommunicationDeviceWin.h"
#include "ui_CommunicationDeviceWin.h"
#include "NewComDeviceDialog.h"
#include "DeviceBase.h"
#include "VariableEditDialog.h"
#include "CommentsDialog.h"
#include "stringdata.h"
#include "writer.h"
#include "reader.h"
#include "variantdata.h"
#include "Helper.h"

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPainter>
#include <QModelIndex>
#include <QComboBox>
#include <QString>
#include <QCloseEvent>
#include <QStringList>
#include <QHeaderView>
#include <QTableView>
#include <QModelIndex>
#include <QVBoxLayout>
#include <QItemSelectionModel>
#include <QModelIndexList>

#include <QDebug>


////////////////////////////////////////////////////////////////////

const int MaxTagSystemTableColumns = 6;


Qt::ItemFlags TagSystemTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
        theFlags |= Qt::ItemIsSelectable|/*Qt::ItemIsEditable|*/
                    Qt::ItemIsEnabled;
    return theFlags;
}

QVariant TagSystemTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
        index.row() < 0 || index.row() >= m_tagSysItems.count() ||
        index.column() < 0 || index.column() >= MaxTagSystemTableColumns)
        return QVariant();
    const TagSysItem &item = m_tagSysItems.at(index.row());
    if (role == Qt::SizeHintRole)
    {
        QStyleOptionComboBox option;
        switch (index.column())
        {
            case TagID: option.currentText = item.m_TagID; break;
            case Name: option.currentText = item.m_sName; break;
            case Description: option.currentText = item.m_sDescription; break;
            case Unit: option.currentText = item.m_sUnit; break;
            case ProjectConverter: option.currentText = item.m_sProjectConverter; break;
            case Comments: option.currentText = item.m_sComments; break;
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
            case TagID: return item.m_TagID;
            case Name: return item.m_sName;
            case Description: return item.m_sDescription;
            case Unit: return item.m_sUnit;
            case ProjectConverter: return item.m_sProjectConverter;
            case Comments: return item.m_sComments;
            default: Q_ASSERT(false);
        }
    }
    return QVariant();
}


QVariant TagSystemTableModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case TagID: return tr("ID");
            case Name: return tr("名称");
            case Description: return tr("描述");
            case Unit: return tr("单位");
            case ProjectConverter: return tr("工程转换");
            case Comments: return tr("备注");
            default: Q_ASSERT(false);
        }
    }
    return section + 1;
}


int TagSystemTableModel::rowCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : m_tagSysItems.count();
}


int TagSystemTableModel::columnCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : MaxTagSystemTableColumns;
}


bool TagSystemTableModel::setData(const QModelIndex &index,
                         const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole ||
        index.row() < 0 || index.row() >= m_tagSysItems.count() ||
        index.column() < 0 || index.column() >= MaxTagSystemTableColumns)
        return false;

    TagSysItem &item = m_tagSysItems[index.row()];
    switch (index.column())
    {
        case TagID: item.m_TagID = value.toInt(); break;;
        case Name: item.m_sName = value.toString(); break;
        case Description: item.m_sDescription = value.toString(); break;
        case Unit: item.m_sUnit = value.toString(); break;
        case ProjectConverter: item.m_sProjectConverter = value.toString(); break;
        case Comments: item.m_sComments = value.toString(); break;

        default: Q_ASSERT(false);
    }
    emit dataChanged(index, index);
    return true;
}


bool TagSystemTableModel::insertRows(int row, int count, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_tagSysItems.insert(row, TagSysItem());
    endInsertRows();
    return true;
}

bool TagSystemTableModel::removeRows(int row, int count, const QModelIndex&)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_tagSysItems.removeAt(row);
    endRemoveRows();
    return true;
}


void TagSystemTableModel::load(const QJsonObject &json)
{
    m_tagSysItems.clear();
    QJsonArray tagSysArray = json["SysVarArray"].toArray();
    for (int i = 0; i < tagSysArray.size(); ++i) {
        QJsonObject jsonObj = tagSysArray[i].toObject();
        TagSysItem item;
        item.m_TagID = jsonObj["iID"].toInt();
        item.m_sName = jsonObj["sName"].toString();
        item.m_sDescription = jsonObj["sDescription"].toString();
        item.m_sUnit = jsonObj["sUnit"].toString();
        item.m_sProjectConverter = jsonObj["sProjectConverter"].toString();
        item.m_sComments = jsonObj["sComments"].toString();
        m_tagSysItems.append(item);
    }
}


void TagSystemTableModel::save(QJsonObject &json)
{
    QJsonArray tagSysArray;
    for(int i = 0; i < m_tagSysItems.size(); i++)
    {
        TagSysItem item =  m_tagSysItems.at(i);
        QJsonObject jsonObj;
        jsonObj["iID"] =  item.m_TagID;
        jsonObj["sName"] = item.m_sName;
        jsonObj["sDescription"] = item.m_sDescription;
        jsonObj["sUnit"] = item.m_sUnit;
        jsonObj["sProjectConverter"] = item.m_sProjectConverter;
        jsonObj["sComments"]= item.m_sComments;
        tagSysArray.append(jsonObj);
    }
    json["SysVarArray"] = tagSysArray;
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

const int MaxTagTmpTableColumns = 11;


Qt::ItemFlags TagTmpTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
        theFlags |= Qt::ItemIsSelectable|/*Qt::ItemIsEditable|*/
                    Qt::ItemIsEnabled;
    if(index.column() == DataType ||
       index.column() == Name ||
       index.column() == Description ||
       index.column() == ActionScope)
        theFlags |= Qt::ItemIsEditable;
    return theFlags;
}


QVariant TagTmpTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
        index.row() < 0 || index.row() >= m_tagTmpItems.count() ||
        index.column() < 0 || index.column() >= MaxTagTmpTableColumns)
        return QVariant();
    const TagTmpItem &item = m_tagTmpItems.at(index.row());
    if (role == Qt::SizeHintRole)
    {
        QStyleOptionComboBox option;
        switch (index.column())
        {
            case TagID: option.currentText = item.m_TagID;break;
            case DataType: option.currentText = item.m_sDataType;break;
            case Name: option.currentText = item.m_sName; break;
            case Description: option.currentText = item.m_sDescription; break;
            case Unit: option.currentText = item.m_sUnit; break;
            case ActionScope: option.currentText = item.m_sActionScope; break;
            case DataAttribute: option.currentText = item.m_sDataAttribute;break;
            case Alarm: option.currentText = item.m_sAlarm;break;
            case ArchiveFile: option.currentText = item.m_sArchiveFile; break;
            case ProjectConverter: option.currentText = item.m_sProjectConverter; break;
            case Comments: option.currentText = item.m_sComments; break;
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
            case TagID: return item.m_TagID;
            case DataType: return item.m_sDataType;
            case Name: return item.m_sName;
            case Description: return item.m_sDescription;
            case Unit: return item.m_sUnit;
            case ActionScope: return item.m_sActionScope;
            case DataAttribute: return item.m_sDataAttribute;
            case Alarm: return item.m_sAlarm;
            case ArchiveFile: return item.m_sArchiveFile;
            case ProjectConverter: return item.m_sProjectConverter;
            case Comments: return item.m_sComments;
            default: Q_ASSERT(false);
        }
    }
    return QVariant();
}


QVariant TagTmpTableModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case TagID: return tr("ID");
            case DataType: return tr("数据类型");
            case Name: return tr("名称");
            case Description: return tr("描述");
            case Unit: return tr("单位");
            case ActionScope: return tr("作用域");
            case DataAttribute: return tr("数据属性");
            case Alarm: return tr("报警");
            case ArchiveFile: return tr("存盘");
            case ProjectConverter: return tr("工程转换");
            case Comments: return tr("备注");

            default: Q_ASSERT(false);
        }
    }
    return section + 1;
}


int TagTmpTableModel::rowCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : m_tagTmpItems.count();
}


int TagTmpTableModel::columnCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : MaxTagTmpTableColumns;
}


bool TagTmpTableModel::setData(const QModelIndex &index,
                         const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole ||
        index.row() < 0 || index.row() >= m_tagTmpItems.count() ||
        index.column() < 0 || index.column() >= MaxTagTmpTableColumns)
        return false;

    TagTmpItem &item = m_tagTmpItems[index.row()];
    switch (index.column())
    {
        case TagID: item.m_TagID = value.toInt(); break;
        case DataType: item.m_sDataType = value.toString(); break;
        case Name: item.m_sName = value.toString(); break;
        case Description: item.m_sDescription = value.toString(); break;
        case Unit: item.m_sUnit = value.toString(); break;
        case ActionScope: item.m_sActionScope = value.toString(); break;
        case DataAttribute: item.m_sDataAttribute = value.toString(); break;
        case Alarm: item.m_sAlarm = value.toString(); break;
        case ArchiveFile: item.m_sArchiveFile = value.toString(); break;
        case ProjectConverter: item.m_sProjectConverter = value.toString(); break;
        case Comments: item.m_sComments = value.toString(); break;
        default: Q_ASSERT(false);
    }
    emit dataChanged(index, index);
    return true;
}


bool TagTmpTableModel::insertRows(int row, int count, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_tagTmpItems.insert(row, TagTmpItem());
    endInsertRows();
    return true;
}


bool TagTmpTableModel::removeRows(int row, int count, const QModelIndex&)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_tagTmpItems.removeAt(row);
    endRemoveRows();
    return true;
}


void TagTmpTableModel::load(const QJsonObject &json)
{
    m_tagTmpItems.clear();
    QJsonArray TmpVarArray = json["TmpVarArray"].toArray();
    for (int i = 0; i < TmpVarArray.size(); ++i) {
        QJsonObject jsonObj = TmpVarArray[i].toObject();
        TagTmpItem item;
        item.m_TagID = jsonObj["iID"].toInt();
        item.m_sName = jsonObj["sName"].toString();
        item.m_sDescription = jsonObj["sDescription"].toString();
        item.m_sUnit = jsonObj["sUnit"].toString();
        item.m_sProjectConverter = jsonObj["sProjectConverter"].toString();
        item.m_sComments = jsonObj["sComments"].toString();
        item.m_sDataType = jsonObj["sDataType"].toString();
        item.m_sActionScope = jsonObj["sActionScope"].toString();
        item.m_sDataAttribute = jsonObj["sDataAttribute"].toString();
        item.m_sAlarm = jsonObj["sAlarm"].toString();
        item.m_sArchiveFile = jsonObj["sArchiveFile"].toString();
        m_tagTmpItems.append(item);
    }

}


void TagTmpTableModel::save(QJsonObject &json)
{
    QJsonArray TmpVarArray;
    for(int i = 0; i < m_tagTmpItems.size(); i++)
    {
        TagTmpItem item =  m_tagTmpItems.at(i);
        QJsonObject jsonObj;
        jsonObj["iID"] = item.m_TagID;
        jsonObj["sDataType"] = item.m_sDataType;
        jsonObj["sName"] = item.m_sName;
        jsonObj["sDescription"] = item.m_sDescription;
        jsonObj["sUnit"] = item.m_sUnit;
        jsonObj["sActionScope"] = item.m_sActionScope;
        jsonObj["sDataAttribute"] = item.m_sDataAttribute;
        jsonObj["sAlarm"] = item.m_sAlarm;
        jsonObj["sArchiveFile"] = item.m_sArchiveFile;
        jsonObj["sProjectConverter"] = item.m_sProjectConverter;
        jsonObj["sComments"] = item.m_sComments;
        TmpVarArray.append(jsonObj);
    }
    json["TmpVarArray"] = TmpVarArray;
}


void TagTmpTableModel::exportToCsv(QString path, QString /*group*/)
{
    QtCSV::StringData varData;
    for(int i = 0; i < m_tagTmpItems.size(); i++)
    {
        TagTmpItem item =  m_tagTmpItems.at(i);
        QStringList varRow;
        varRow << QString("%1").arg(item.m_TagID) << item.m_sDataType << item.m_sName
               << item.m_sDescription << item.m_sUnit << item.m_sActionScope
               << item.m_sDataAttribute << item.m_sAlarm << item.m_sArchiveFile
               << item.m_sProjectConverter << item.m_sComments;
        varData.addRow(varRow);
    }
    QString filepath = path + "/中间变量.csv";
    QStringList header;
    header << "ID" << "数据类型" << "名称" << "描述" << "单位" << "作用域" << "数据属性" << "报警" << "存盘" << "工程转换" << "备注";
    QtCSV::Writer::write(filepath, varData, QString(","), QString("\""),
                            QtCSV::Writer::REWRITE, header,  QStringList(), QTextCodec::codecForName("GB18030"));
}


void TagTmpTableModel::importFromCsv(QString path)
{
    QString filepath = path;

    QList<QStringList> data = QtCSV::Reader::readToList(filepath, QString(","),  QString("\""), QTextCodec::codecForName("GB18030"));
    int lastID = m_tagTmpItems.last().m_TagID;
    int lastNextRow = m_tagTmpItems.size();
    //qDebug()<<"lastID: "<<lastID;
    beginInsertRows(QModelIndex(), lastNextRow, lastNextRow + data.size() - 2);
    for(int i=0; i<data.size(); i++)
    {
        QStringList row = data.at(i);
        if(row.at(0) == "ID")
            continue;
        TagTmpItem item;
        item.m_TagID = lastID + row.at(0).toInt();
        item.m_sDataType = row.at(1);
        item.m_sName = row.at(2);
        item.m_sDescription = row.at(3);
        item.m_sUnit = row.at(4);
        item.m_sActionScope = row.at(5);
        item.m_sDataAttribute = row.at(6);
        item.m_sAlarm = row.at(7);
        item.m_sArchiveFile = row.at(8);
        item.m_sProjectConverter = row.at(9);
        item.m_sComments = row.at(10);
        m_tagTmpItems.append(item);
    }
    endInsertRows();
}


void TagTmpTableModel::AppendRow(TagTmpItem item)
{
    int lastNextRow = m_tagTmpItems.size();
    beginInsertRows(QModelIndex(), lastNextRow, lastNextRow);
    TagTmpItem it = item;
    m_tagTmpItems.append(it);
    endInsertRows();
}


void TagTmpTableModel::InsertRow(int i, TagTmpItem item)
{
    beginInsertRows(QModelIndex(), i, i);
    TagTmpItem it = item;
    m_tagTmpItems.insert(i, it);
    endInsertRows();
}


TagTmpItem TagTmpTableModel::GetRow(int i)
{
    TagTmpItem it;
    if(i < m_tagTmpItems.size())
        return m_tagTmpItems.at(i);
    return it;
}


void TagTmpTableModel::UpdateRow(int i, TagTmpItem item)
{
    if(i<m_tagTmpItems.size())
    {
        m_tagTmpItems.replace(i, item);
        for(int c=0;c<MaxTagTmpTableColumns;c++)
            emit dataChanged(this->index(i, c), this->index(i, c));
    }
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


const int MaxTagIOTableColumns = 10;


Qt::ItemFlags TagIOTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
        theFlags |= Qt::ItemIsSelectable|/*Qt::ItemIsEditable|*/
                    Qt::ItemIsEnabled;
    if(index.column() == DataType ||
       index.column() == Name ||
       index.column() == Description)
        theFlags |= Qt::ItemIsEditable;
    return theFlags;
}


QVariant TagIOTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
        index.row() < 0 || index.row() >= m_tagIOItems.count() ||
        index.column() < 0 || index.column() >= MaxTagIOTableColumns)
        return QVariant();
    const TagIOItem &item = m_tagIOItems.at(index.row());
    if (role == Qt::SizeHintRole)
    {
        QStyleOptionComboBox option;
        switch (index.column())
        {
            case TagID: option.currentText = item.m_TagID;break;
            case DataType: option.currentText = item.m_sDataType;break;
            case Name: option.currentText = item.m_sName; break;
            case Description: option.currentText = item.m_sDescription; break;
            case Unit: option.currentText = item.m_sUnit; break;
            case IOConnect: option.currentText = item.m_sIOConnect; break;
            case Alarm: option.currentText = item.m_sAlarm;break;
            case ArchiveFile: option.currentText = item.m_sArchiveFile; break;
            case ProjectConverter: option.currentText = item.m_sProjectConverter; break;
            case Comments: option.currentText = item.m_sComments; break;
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
            case TagID: return item.m_TagID;
            case DataType: return item.m_sDataType;
            case Name: return item.m_sName;
            case Description: return item.m_sDescription;
            case Unit: return item.m_sUnit;
            case IOConnect: return item.m_sIOConnect;
            case Alarm: return item.m_sAlarm;
            case ArchiveFile: return item.m_sArchiveFile;
            case ProjectConverter: return item.m_sProjectConverter;
            case Comments: return item.m_sComments;
            default: Q_ASSERT(false);
        }
    }
    return QVariant();
}


QVariant TagIOTableModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case TagID: return tr("ID");
            case DataType: return tr("数据类型");
            case Name: return tr("名称");
            case Description: return tr("描述");
            case Unit: return tr("单位");
            case IOConnect: return tr("IO连接");
            case Alarm: return tr("报警");
            case ArchiveFile: return tr("存盘");
            case ProjectConverter: return tr("工程转换");
            case Comments: return tr("备注");

            default: Q_ASSERT(false);
        }
    }
    return section + 1;
}


int TagIOTableModel::rowCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : m_tagIOItems.count();
}


int TagIOTableModel::columnCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : MaxTagIOTableColumns;
}


bool TagIOTableModel::setData(const QModelIndex &index,
                         const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole ||
        index.row() < 0 || index.row() >= m_tagIOItems.count() ||
        index.column() < 0 || index.column() >= MaxTagIOTableColumns)
        return false;

    TagIOItem &item = m_tagIOItems[index.row()];
    switch (index.column())
    {
        case TagID: item.m_TagID = value.toInt(); break;
        case DataType: item.m_sDataType = value.toString(); break;
        case Name: item.m_sName = value.toString(); break;
        case Description: item.m_sDescription = value.toString(); break;
        case Unit: item.m_sUnit = value.toString(); break;
        case IOConnect: item.m_sIOConnect = value.toString(); break;
        case Alarm: item.m_sAlarm = value.toString(); break;
        case ArchiveFile: item.m_sArchiveFile = value.toString(); break;
        case ProjectConverter: item.m_sProjectConverter = value.toString(); break;
        case Comments: item.m_sComments = value.toString(); break;
        default: Q_ASSERT(false);
    }
    emit dataChanged(index, index);
    return true;
}


bool TagIOTableModel::insertRows(int row, int count, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_tagIOItems.insert(row, TagIOItem());
    endInsertRows();
    return true;
}


bool TagIOTableModel::removeRows(int row, int count, const QModelIndex&)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_tagIOItems.removeAt(row);
    endRemoveRows();
    return true;
}


void TagIOTableModel::load(const QJsonObject &json)
{
    m_tagIOItems.clear();
    QJsonArray IOVarArray = json["IOVarArray"].toArray();
    for (int i = 0; i < IOVarArray.size(); ++i) {
        QJsonObject jsonObj = IOVarArray[i].toObject();
        TagIOItem item;
        item.m_TagID = jsonObj["iID"].toInt();
        item.m_sName = jsonObj["sName"].toString();
        item.m_sDescription = jsonObj["sDescription"].toString();
        item.m_sUnit = jsonObj["sUnit"].toString();
        item.m_sProjectConverter = jsonObj["sProjectConverter"].toString();
        item.m_sComments = jsonObj["sComments"].toString();
        item.m_sDataType = jsonObj["sDataType"].toString();
        item.m_sIOConnect = jsonObj["sIOConnect"].toString();
        item.m_sAlarm = jsonObj["sAlarm"].toString();
        item.m_sArchiveFile = jsonObj["sArchiveFile"].toString();
        m_tagIOItems.append(item);
    }
}


void TagIOTableModel::save(QJsonObject &json)
{
    QJsonArray IOVarArray;
    for(int i = 0; i < m_tagIOItems.size(); i++)
    {
        TagIOItem item =  m_tagIOItems.at(i);
        QJsonObject jsonObj;
        jsonObj["iID"] = item.m_TagID;
        jsonObj["sDataType"] = item.m_sDataType;
        jsonObj["sName"] = item.m_sName;
        jsonObj["sDescription"] = item.m_sDescription;
        jsonObj["sUnit"] = item.m_sUnit;
        jsonObj["sIOConnect"] = item.m_sIOConnect;
        jsonObj["sAlarm"] = item.m_sAlarm;
        jsonObj["sArchiveFile"] = item.m_sArchiveFile;
        jsonObj["sProjectConverter"] = item.m_sProjectConverter;
        jsonObj["sComments"] = item.m_sComments;
        IOVarArray.append(jsonObj);
    }
    json["IOVarArray"] = IOVarArray;
}

void TagIOTableModel::exportToCsv(QString path, QString group)
{
    QtCSV::StringData varData;
    for(int i = 0; i < m_tagIOItems.size(); i++)
    {
        TagIOItem item =  m_tagIOItems.at(i);
        QStringList varRow;
        varRow << QString("%1").arg(item.m_TagID) << item.m_sDataType << item.m_sName
               << item.m_sDescription << item.m_sUnit << item.m_sIOConnect
               << item.m_sAlarm << item.m_sArchiveFile << item.m_sProjectConverter
               << item.m_sComments;
        varData.addRow(varRow);
    }
    QString filepath = path + "/" + group + ".csv";
    QStringList header;
    header << "ID" << "数据类型" << "名称" << "描述" << "单位" << "IO连接" << "报警" << "存盘" << "工程转换" << "备注";

    QtCSV::Writer::write(filepath, varData, QString(","), QString("\""),
                            QtCSV::Writer::REWRITE, header,  QStringList(), QTextCodec::codecForName("GB18030"));
}


void TagIOTableModel::importFromCsv(QString path)
{
    QString filepath = path;

    QList<QStringList> data = QtCSV::Reader::readToList(filepath, QString(","),  QString("\""), QTextCodec::codecForName("GB18030"));
    int lastNextRow = m_tagIOItems.size();
    int lastID = 0;
    if(lastNextRow != 0)
        lastID = m_tagIOItems.last().m_TagID;

    //qDebug()<<"lastID: "<<lastID;
    beginInsertRows(QModelIndex(), lastNextRow, lastNextRow + data.size() - 2);
    for(int i=0; i<data.size(); i++)
    {
        QStringList row = data.at(i);
        if(row.at(0) == "ID")
            continue;
        TagIOItem item;
        item.m_TagID = lastID + row.at(0).toInt();
        item.m_sDataType = row.at(1);
        item.m_sName = row.at(2);
        item.m_sDescription = row.at(3);
        item.m_sUnit = row.at(4);
        item.m_sIOConnect = row.at(5);
        item.m_sAlarm = row.at(6);
        item.m_sArchiveFile = row.at(7);
        item.m_sProjectConverter = row.at(8);
        item.m_sComments = row.at(9);
        m_tagIOItems.append(item);
    }
    endInsertRows();

}



void TagIOTableModel::AppendRow(TagIOItem item)
{
    int lastNextRow = m_tagIOItems.size();
    beginInsertRows(QModelIndex(), lastNextRow, lastNextRow);
    TagIOItem it = item;
    m_tagIOItems.append(it);
    endInsertRows();
}


void TagIOTableModel::InsertRow(int i, TagIOItem item)
{
    beginInsertRows(QModelIndex(), i, i);
    TagIOItem it = item;
    m_tagIOItems.insert(i, it);
    endInsertRows();
}


TagIOItem TagIOTableModel::GetRow(int i)
{
    TagIOItem it;
    if(i < m_tagIOItems.size())
        return m_tagIOItems.at(i);
    return it;
}


void TagIOTableModel::UpdateRow(int i, TagIOItem item)
{
    if(i<m_tagIOItems.size())
    {
        m_tagIOItems.replace(i, item);
        for(int c=0;c<MaxTagIOTableColumns;c++)
            emit dataChanged(this->index(i, c), this->index(i, c));
    }
}



////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


ComboBoxDelegate::ComboBoxDelegate( QStringList list, QObject *parent)
    : QItemDelegate( parent )
{
    m_strItems = list;
}

QWidget *ComboBoxDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/ ) const
{
    QComboBox *cbo = new QComboBox( parent );
    cbo->addItems(this->m_strItems);
    cbo->installEventFilter(const_cast<ComboBoxDelegate*>(this));
    return cbo;
}

void ComboBoxDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/ ) const
{
    editor->setGeometry( option.rect );
}

void ComboBoxDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox =  static_cast <QComboBox*>(editor);
    int tindex = comboBox->findText(text);
    comboBox->setCurrentIndex(tindex);
}

void ComboBoxDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    QComboBox *comboBox =  static_cast <QComboBox*>(editor);
    QString text = comboBox->currentText();
    model->setData(index, text, Qt::EditRole);
}


//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////


VariableTableView::VariableTableView(QWidget *parent) :
    QTableView(parent)
{

}

VariableTableView::~VariableTableView()
{

}

void VariableTableView::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton || event->button()==Qt::RightButton)
    {
        QModelIndex idx = indexAt(event->pos());
        if (!idx.isValid())
        {
            QItemSelectionModel *pModel = selectionModel();
            if(pModel != nullptr)
                pModel->clearSelection();
        }
    }
    QTableView::mousePressEvent(event);
}



///////////////////////////////////////////////////////////////////


VariableManagerWin::VariableManagerWin(QWidget *parent,
                                       const QString &itemName,
                                       const QString &projName) :
    ChildBase(parent, itemName, projName),
    ui(new Ui::VariableManagerWin),
    m_strCurTagType("")
{
    ui->setupUi(this);

    ui->verticalLayout->setSpacing(1);
    ui->verticalLayout->setContentsMargins(1, 1, 1, 1);
    m_variableTableView = new VariableTableView(this);
    m_variableTableView->setObjectName(QStringLiteral("variableTableView"));
    ui->verticalLayout->addWidget(m_variableTableView);
    connect(m_variableTableView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(tableViewVariableDoubleClicked(QModelIndex)));

    setContextMenuPolicy(Qt::DefaultContextMenu);

    pTagIOTableModel = new TagIOTableModel(this);
    pTagTmpTableModel = new TagTmpTableModel(this);
    pTagSystemTableModel = new TagSystemTableModel(this);

}

VariableManagerWin::~VariableManagerWin()
{
    delete m_variableTableView;
    delete ui;   
    if(pTagSystemTableModel)
    {
        delete pTagSystemTableModel;
        pTagSystemTableModel = 0;
    }
    if(pTagTmpTableModel)
    {
        delete pTagTmpTableModel;
        pTagTmpTableModel = 0;
    }
    if(pTagIOTableModel)
    {
        delete pTagIOTableModel;
        pTagIOTableModel = 0;
    }
}

void VariableManagerWin::init(const QString &itemName)
{   
    if(m_strCurTagType != itemName) {
        if(!m_strCurTagType.isEmpty()) {
            SetTitle(m_strCurTagType);
            save();
        }
        SetTitle(itemName);
        loadFromFile(DATA_SAVE_FORMAT, m_strItemName);
        m_strCurTagType = itemName;
    }

    if(m_strItemName == tr("设备变量")) {
        m_variableTableView->setModel(pTagIOTableModel);
        QStringList stringListDataType;
        stringListDataType << tr("模拟量")
                           << tr("数字量");
        ComboBoxDelegate *pDdataTypeDelegate = new ComboBoxDelegate(stringListDataType, this);
        m_variableTableView->setItemDelegateForColumn(1, pDdataTypeDelegate);
    } else if(m_strItemName == tr("中间变量")) {
        m_variableTableView->setModel(pTagTmpTableModel);
        QStringList stringListDataType;
        stringListDataType << tr("模拟量")
                           << tr("数字量");
        ComboBoxDelegate *pDdataTypeDelegate = new ComboBoxDelegate(stringListDataType, this);
        m_variableTableView->setItemDelegateForColumn(1, pDdataTypeDelegate);

        QStringList stringListActionScope;
        stringListActionScope << tr("全局");// << tr("数字量");  后期加入作用画面名称
        ComboBoxDelegate *pActionScopeDelegate = new ComboBoxDelegate(stringListActionScope, this);
        m_variableTableView->setItemDelegateForColumn(5, pActionScopeDelegate);
    } else if(m_strItemName == tr("系统变量")) {
        m_variableTableView->setModel(pTagSystemTableModel);
    }
    m_variableTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 单元格显示不换行
    m_variableTableView->setWordWrap(false);
    //QHeaderView *pVHeaderView = m_variableTableView->verticalHeader();
    //pVHeaderView->setSectionResizeMode(QHeaderView::ResizeToContents);
    //QHeaderView *pHHeaderView = m_variableTableView->horizontalHeader();
    //pHHeaderView->setSectionResizeMode(QHeaderView::ResizeToContents);

    m_variableTableView->horizontalHeader()->setStretchLastSection(true);
    m_variableTableView->horizontalHeader()->setHighlightSections(false);  // 当表格只有一行的时候，则表头会出现塌陷问题
}

void VariableManagerWin::SetTitle(const QString &t)
{
    if(t == tr("中间变量") || t == tr("系统变量"))
    {
        m_strItemName = t;        
    }
    else
    {
        m_strItemName = tr("设备变量");
        m_IOVariableListWhat = t;
    }
}


bool VariableManagerWin::loadFromFile(SaveFormat saveFormat, const QString &it)
{
    QString file = "";

    if(it == tr("设备变量"))
    {
        file = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/DevVarList-" + m_IOVariableListWhat + ".odb";
    }
    else if(it == tr("中间变量"))
    {
        file = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/TmpVarList.odb";
    }
    else if(it == tr("系统变量"))
    {
        file = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/SysVarList.odb";
        QString srcfile = QCoreApplication::applicationDirPath() + "/SysVarList.odb";
        QFile testFile(file);
        if(testFile.exists())
        {
            testFile.remove();
        }
        QFile::copy(srcfile ,file);
    }

    QFile loadFile(file);
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        if(it.left(it.indexOf("-")) == tr("设备变量"))
        {
            qWarning("Couldn't open load file: DevVarList.odb.");
        }
        else if(it == tr("中间变量"))
        {
            qWarning("Couldn't open load file: TmpVarList.odb.");
        }
        else if(it == tr("系统变量"))
        {
            qWarning("Couldn't open load file: SysVarList.odb.");
        }
        return false;
    }
    QByteArray loadData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(loadData) : QJsonDocument::fromBinaryData(loadData));
    load(loadDoc.object(), it);
    loadFile.close();
    return true;
}


bool VariableManagerWin::saveToFile(SaveFormat saveFormat, const QString &it)
{
    QString file = "";
    if(it == tr("设备变量"))
    {
        file = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/DevVarList-" + m_IOVariableListWhat + ".odb";
    }
    else if(it == tr("中间变量"))
    {
        file = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/TmpVarList.odb";
    }
    else if(it == tr("系统变量"))
    {
        return true;
    }
    QFile saveFile(file);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        if(it.left(it.indexOf("-")) == tr("设备变量"))
        {
            qWarning("Couldn't open save file: DevVarList.odb.");
        }
        else if(it == tr("中间变量"))
        {
            qWarning("Couldn't open save file: TmpVarList.odb.");
        }
        else if(it == tr("系统变量"))
        {
            // do nothing here!
        }
        return false;
    }
    QJsonObject obj;
    save(obj, it);
    QJsonDocument saveDoc(obj);
    saveFile.write(saveFormat == Json ? saveDoc.toJson() : saveDoc.toBinaryData());
    saveFile.close();
    return true;
}


void VariableManagerWin::load(const QJsonObject &json, const QString &it)
{
    if(it == "设备变量")
    {
        pTagIOTableModel->load(json);
    }
    else if(it == "中间变量")
    {
        pTagTmpTableModel->load(json);
    }
    else if(it == "系统变量")
    {
        pTagSystemTableModel->load(json);
    }
}

void VariableManagerWin::save(QJsonObject &json, const QString &it)
{
    if(it == "设备变量")
    {
        pTagIOTableModel->save(json);
    }
    else if(it == "中间变量")
    {
        pTagTmpTableModel->save(json);
    }
    else if(it == "系统变量")
    {
        // do nothing here!
    }
}

/*
* 导出变量表
*/
void VariableManagerWin::exportToCsv(const QString &path, const QString &group)
{
    //qDebug()<< m_strItemName << " path: " << path;
    if(m_strItemName == "设备变量")
    {
        pTagIOTableModel->exportToCsv(path, group);
    }
    else if(m_strItemName == "中间变量")
    {
        pTagTmpTableModel->exportToCsv(path, group);
    }
}

/*
* 导入变量表
*/
void VariableManagerWin::importFromCsv(const QString &path)
{
    if(path.contains("设备变量"))
    {
        pTagIOTableModel->importFromCsv(path);
    }
    else if(path.contains("中间变量"))
    {
        pTagTmpTableModel->importFromCsv(path);
    }
}


/*
* 右键菜单
*/
void VariableManagerWin::contextMenuEvent(QContextMenuEvent * /*event*/)
{
    if(m_strItemName == "变量管理" || m_strItemName == "系统变量")
        return;

    QMenu *pMenu = new QMenu(this);

    QAction *pAddAct = new QAction(QIcon(":/images/data_add.png"), tr("增加"), this);
    pAddAct->setStatusTip(tr("新增变量"));
    connect(pAddAct, SIGNAL(triggered()), this, SLOT(VariableAdd()));
    pMenu->addAction(pAddAct);

    QAction *pAppendAct = new QAction(QIcon(":/images/data_supadd.png"), tr("追加"), this);
    pAppendAct->setStatusTip(tr("追加"));
    connect(pAppendAct, SIGNAL(triggered()), this, SLOT(VariableAppend()));
    pMenu->addAction(pAppendAct);

    QAction *pRowCopyAct = new QAction(QIcon(":/images/data_rowcopy.png"), tr("行拷"), this);
    pRowCopyAct->setStatusTip(tr("行拷"));
    connect(pRowCopyAct, SIGNAL(triggered()), this, SLOT(VariableRowCopy()));
    pMenu->addAction(pRowCopyAct);

    QAction *pColCopyAct = new QAction(QIcon(":/images/data_colcopy.png"), tr("列拷"), this);
    pColCopyAct->setStatusTip(tr("列拷"));
    connect(pColCopyAct, SIGNAL(triggered()), this, SLOT(VariableColCopy()));
    pMenu->addAction(pColCopyAct);

    QAction *pModifyAct = new QAction(QIcon(":/images/icon_modify.png"), tr("修改"), this);
    pModifyAct->setStatusTip(tr("修改"));
    connect(pModifyAct, SIGNAL(triggered()), this, SLOT(VariableModify()));
    pMenu->addAction(pModifyAct);

    QAction *pDeleteAct = new QAction(QIcon(":/images/data_delete.png"), tr("删除"), this);
    pDeleteAct->setStatusTip(tr("删除"));
    connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(VariableDelete()));
    pMenu->addAction(pDeleteAct);

    pMenu->move(cursor().pos());
    pMenu->show();
}


/*
* 插槽：变量表视图双击处理
*/
void VariableManagerWin::tableViewVariableDoubleClicked(const QModelIndex &index)
{
    QStringList sl;
    QModelIndex idx;
    QVariant val;
    int rowIndex = index.row();
    int columnIndex = index.column();

    if(m_strItemName == "设备变量")
    {
        TagIOItem item = pTagIOTableModel->GetRow(rowIndex);
        if(columnIndex == pTagIOTableModel->Column::IOConnect ||
           columnIndex == pTagIOTableModel->Column::Alarm ||
           columnIndex == pTagIOTableModel->Column::ArchiveFile)
        {
            VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
            pDlg->setWindowTitle(tr("编辑设备变量"));
            sl << item.m_sDataType << item.m_sName << item.m_sDescription << item.m_sUnit;

            // 单元格数据有可能已经改变
            if(item.m_sDataType == "模拟量")
                pDlg->SetVariableType(VariableEditDialog::AI);
            else if(item.m_sDataType == "数字量")
                pDlg->SetVariableType(VariableEditDialog::DI);
            else
                pDlg->SetVariableType(VariableEditDialog::NONE);
            pDlg->SetBasicSetting(sl);

            // Removes the tab at position index from this stack of widgets.
            // The page widget itself is not deleted.
            pDlg->RemoveTab(0); // 隐藏基本设置页
            pDlg->RemoveTab(0); // 隐藏数据属性页
            if(columnIndex == pTagIOTableModel->Column::IOConnect)
                pDlg->SetCurrentTabIndex(4-2); // IO连接页
            else if(columnIndex == pTagIOTableModel->Column::Alarm)
                pDlg->SetCurrentTabIndex(3-2); // 报警页
            else if(columnIndex == pTagIOTableModel->Column::ArchiveFile)
                pDlg->SetCurrentTabIndex(2-2); // 存盘页

            pDlg->SetIOConnectString(item.m_sIOConnect);
            pDlg->SetAlarmString(item.m_sAlarm);
            pDlg->SetSaveDiskString(item.m_sArchiveFile);

            if(pDlg->exec() == QDialog::Accepted)
            { 
                QStringList sl = pDlg->GetBasicSetting();
                item.m_sDataType = sl.at(0);
                item.m_sName = sl.at(1);
                item.m_sDescription = sl.at(2);
                item.m_sUnit = sl.at(3);
                item.m_sIOConnect = pDlg->GetIOConnectString();
                item.m_sAlarm = pDlg->GetAlarmString();
                item.m_sArchiveFile = pDlg->GetSaveDiskString();
                pTagIOTableModel->UpdateRow(rowIndex, item);
            }
            delete pDlg;
        }
        else if(columnIndex == pTagIOTableModel->Column::ProjectConverter)
        {
            TagFuncEditDialog *pDlg = new TagFuncEditDialog(m_strProjectName.left(m_strProjectName.lastIndexOf("/")), this);
            pDlg->SetData(item.m_sProjectConverter);
            if(pDlg->exec() == QDialog::Accepted)
            {
                item.m_sProjectConverter = pDlg->GetData();
                pTagIOTableModel->UpdateRow(rowIndex, item);
            }
            delete pDlg;
        }
        else if(columnIndex == pTagIOTableModel->Column::Comments)
        {
            CommentsDialog *pDlg = new CommentsDialog(this);
            pDlg->SetCommentsText(item.m_sComments);
            if(pDlg->exec() == QDialog::Accepted)
            {
                item.m_sComments = pDlg->GetCommentsText();
                pTagIOTableModel->UpdateRow(rowIndex, item);
            }
            delete pDlg;
        }
    }
    else if(m_strItemName == "中间变量")
    {
        TagTmpItem item = pTagTmpTableModel->GetRow(rowIndex);
        if(columnIndex == pTagTmpTableModel->Column::DataAttribute ||
           columnIndex == pTagTmpTableModel->Column::Alarm ||
           columnIndex == pTagTmpTableModel->Column::ArchiveFile)
        {
            VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
            pDlg->setWindowTitle(tr("编辑中间变量"));
            sl << item.m_sDataType << item.m_sName << item.m_sDescription << item.m_sUnit;

            // 单元格数据有可能已经改变
            if(item.m_sDataType == "模拟量")
                pDlg->SetVariableType(VariableEditDialog::AI);
            else if(item.m_sDataType == "数字量")
                pDlg->SetVariableType(VariableEditDialog::DI);
            else
                pDlg->SetVariableType(VariableEditDialog::NONE);
            pDlg->SetBasicSetting(sl);

            // Removes the tab at position index from this stack of widgets.
            // The page widget itself is not deleted.
            pDlg->RemoveTab(0); // 隐藏基本设置页
            if(columnIndex == pTagTmpTableModel->Column::DataAttribute)
                pDlg->SetCurrentTabIndex(1-1); // 数据属性页 移除"基本设置页"后"数据属性页"CurrentTabIndex为0
            else if(columnIndex == pTagTmpTableModel->Column::Alarm)
                pDlg->SetCurrentTabIndex(3-1); // 报警页
            else if(columnIndex == pTagTmpTableModel->Column::ArchiveFile)
                pDlg->SetCurrentTabIndex(2-1); // 存盘页
            pDlg->RemoveTab(4-1); // 隐藏IO连接页

            pDlg->SetDataAttribuyeString(item.m_sDataAttribute);
            pDlg->SetAlarmString(item.m_sAlarm);
            pDlg->SetSaveDiskString(item.m_sArchiveFile);

            if(pDlg->exec() == QDialog::Accepted)
            {
                QStringList sl = pDlg->GetBasicSetting();
                item.m_sDataType = sl.at(0);
                item.m_sName = sl.at(1);
                item.m_sDescription = sl.at(2);
                item.m_sUnit = sl.at(3);
                item.m_sDataAttribute = pDlg->GetDataAttribuyeString();
                item.m_sAlarm = pDlg->GetAlarmString();
                item.m_sArchiveFile = pDlg->GetSaveDiskString();
                pTagTmpTableModel->UpdateRow(rowIndex, item);
            }
            delete pDlg;
        }
        else if(columnIndex == pTagTmpTableModel->Column::ProjectConverter)
        {
            TagFuncEditDialog *pDlg = new TagFuncEditDialog(m_strProjectName.left(m_strProjectName.lastIndexOf("/")), this);
            pDlg->SetData(item.m_sProjectConverter);
            if(pDlg->exec() == QDialog::Accepted)
            {
                item.m_sProjectConverter = pDlg->GetData();
                pTagTmpTableModel->UpdateRow(rowIndex, item);
            }
            delete pDlg;
        }
        else if(columnIndex == pTagTmpTableModel->Column::Comments)
        {
            CommentsDialog *pDlg = new CommentsDialog(this);
            pDlg->SetCommentsText(item.m_sComments);
            if(pDlg->exec() == QDialog::Accepted)
            {
                item.m_sComments = pDlg->GetCommentsText();
                pTagTmpTableModel->UpdateRow(rowIndex, item);
            }
            delete pDlg;
        }
    }
}

/*
* 插槽：增加变量
*/
void VariableManagerWin::VariableAdd()
{
    QModelIndex index;
    QVariant val;
    bool ok = false;
    int i;
    int num;

    if(m_strItemName == "设备变量")
    {
        VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
        pDlg->RemoveTab(1); // 隐藏数据属性页
        if(pDlg->exec() == QDialog::Accepted)
        {
            num = pDlg->GetBatchNum().toInt(&ok, 10);
            if(ok)
                for(i=0; i<num; i++)
                {
                    TagIOItem prevItem;
                    TagIOItem newItem;
                    if(pTagIOTableModel->rowCount()>0)
                    {
                        prevItem = pTagIOTableModel->GetRow(pTagIOTableModel->rowCount()-1);
                        newItem.m_TagID = prevItem.m_TagID + 1;
                    }
                    else
                    {
                        newItem.m_TagID = IOVARIABLE_BASE + 1;
                    }
                    newItem.m_sDataType = pDlg->GetDataType();
                    newItem.m_sName = pDlg->GetName() + QString("%1").arg(i+1);
                    newItem.m_sDescription = pDlg->GetDescription();
                    newItem.m_sUnit = pDlg->GetUnit();

                    int offset = pDlg->GetDataRegisterSpace().toInt() * i;
                    pDlg->SetBatchOffset(offset);
                    newItem.m_sIOConnect = pDlg->GetIOConnectString();

                    newItem.m_sAlarm = pDlg->GetAlarmString();
                    newItem.m_sArchiveFile = pDlg->GetSaveDiskString();
                    pTagIOTableModel->AppendRow(newItem);
                }
        }
        delete pDlg;
    }
    else if(m_strItemName == "中间变量")
    {
        VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
        pDlg->RemoveTab(4); // 隐藏IO连接页
        if(pDlg->exec() == QDialog::Accepted)
        {
            num = pDlg->GetBatchNum().toInt(&ok, 10);
            if(ok)
                for(i=0; i<num; i++)
                {
                    TagTmpItem  prevItem;
                    TagTmpItem  newItem;
                    if(pTagTmpTableModel->rowCount()>0)
                    {
                        prevItem = pTagTmpTableModel->GetRow(pTagTmpTableModel->rowCount()-1);
                        newItem.m_TagID = prevItem.m_TagID + 1;
                    }
                    else
                    {
                        newItem.m_TagID = TMPVARIABLE_BASE + 1;
                    }
                    newItem.m_sDataType = pDlg->GetDataType();
                    newItem.m_sName = pDlg->GetName() + QString("%1").arg(i+1);
                    newItem.m_sDescription = pDlg->GetDescription();
                    newItem.m_sUnit = pDlg->GetUnit();
                    newItem.m_sActionScope = "全局";
                    newItem.m_sDataAttribute = pDlg->GetDataAttribuyeString();
                    newItem.m_sAlarm = pDlg->GetAlarmString();
                    newItem.m_sArchiveFile = pDlg->GetSaveDiskString();
                    pTagTmpTableModel->AppendRow(newItem);
                }
        }
        delete pDlg;
    }
}


/*
* 插槽：追加变量
*/
void VariableManagerWin::VariableAppend()
{
    QModelIndex index;
    QVariant val;

    if(m_strItemName == "设备变量")
    {
        if(pTagIOTableModel->rowCount() < 1)
            return;

        TagIOItem prevItem = pTagIOTableModel->GetRow(pTagIOTableModel->rowCount()-1);
        TagIOItem newItem;
        if(pTagIOTableModel->rowCount()>0)
            newItem.m_TagID = prevItem.m_TagID + 1;
        else
            newItem.m_TagID = IOVARIABLE_BASE + 1;
        newItem.m_sDataType = tr("模拟量");
        // 获取前一行的Name
        QString lastVarName = prevItem.m_sName;
        QString str = "var";
        if(lastVarName.indexOf("var")>-1)
        {
            int len = lastVarName.size();
            QString subStr = lastVarName.right(len - 3);
            int idx = subStr.toInt();
            str = QString("var%1").arg(idx+1);
        }
        newItem.m_sName = str;
        newItem.m_sDescription = tr("描述");
        newItem.m_sUnit = "mA";
        pTagIOTableModel->AppendRow(newItem);
    }
    else if(m_strItemName == "中间变量")
    {
        if(pTagTmpTableModel->rowCount() < 1)
            return;

        TagTmpItem  prevItem = pTagTmpTableModel->GetRow(pTagTmpTableModel->rowCount()-1);
        TagTmpItem  newItem;
        if(pTagTmpTableModel->rowCount()>0)
            newItem.m_TagID = prevItem.m_TagID + 1;
        else
            newItem.m_TagID = TMPVARIABLE_BASE + 1;
        newItem.m_sDataType = tr("模拟量");
        // 获取前一行的Name
        QString lastVarName = prevItem.m_sName;
        QString str = "var";
        if(lastVarName.indexOf("var")>-1)
        {
            int len = lastVarName.size();
            QString subStr = lastVarName.right(len - 3);
            int idx = subStr.toInt();
            str = QString("var%1").arg(idx+1);
        }
        newItem.m_sName = str;
        newItem.m_sDescription = tr("描述");
        newItem.m_sUnit = "mA";
        newItem.m_sActionScope = tr("全局");
        pTagTmpTableModel->AppendRow(newItem);
    }
}


/*
* 插槽：行拷贝变量
*/
void VariableManagerWin::VariableRowCopy()
{
    QModelIndex ModelIndex = m_variableTableView->selectionModel()->currentIndex();
    QModelIndex index;
    QVariant val;
    int row = ModelIndex.row();
    int column = ModelIndex.column();

    if(row < 0 || column < 0)
        return;

    if(m_strItemName == "设备变量")
    {
        TagIOItem curitem = pTagIOTableModel->GetRow(row);
        TagIOItem lastitem = pTagIOTableModel->GetRow(pTagIOTableModel->rowCount()-1);
        curitem.m_TagID = lastitem.m_TagID + 1;
        pTagIOTableModel->AppendRow(curitem);
    }
    else if(m_strItemName == "中间变量")
    {
        TagTmpItem curitem = pTagTmpTableModel->GetRow(row);
        TagTmpItem lastitem = pTagTmpTableModel->GetRow(pTagTmpTableModel->rowCount()-1);
        curitem.m_TagID = lastitem.m_TagID + 1;
        pTagTmpTableModel->AppendRow(curitem);
    }

}

/*
* 插槽：列拷贝变量
*/
void VariableManagerWin::VariableColCopy()
{
    if(m_strItemName == "设备变量")
    {

    }
    else if(m_strItemName == "中间变量")
    {

    }
}

/*
* 插槽：修改变量
*/
void VariableManagerWin::VariableModify()
{
    QModelIndex index;
    QVariant val;
    QStringList sl;
    QModelIndex idx;
    QString sTmp;
    int rowIndex = m_variableTableView->currentIndex().row();

    if(m_strItemName == "设备变量")
    {
        VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
        pDlg->setWindowTitle(tr("编辑设备变量"));
        TagIOItem item = pTagIOTableModel->GetRow(rowIndex);

        if(item.m_sDataType == "模拟量")
            pDlg->SetVariableType(VariableEditDialog::AI);
        else if(item.m_sDataType == "数字量")
            pDlg->SetVariableType(VariableEditDialog::DI);
        else
            pDlg->SetVariableType(VariableEditDialog::NONE);

        sl << item.m_sDataType << item.m_sName << item.m_sDescription << item.m_sUnit;
        pDlg->SetBasicSetting(sl);

        // Removes the tab at position index from this stack of widgets.
        // The page widget itself is not deleted.
        pDlg->RemoveTab(1); // 隐藏数据属性页
        pDlg->SetDataAttribuyeString(item.m_sIOConnect);
        pDlg->SetAlarmString(item.m_sAlarm);
        pDlg->SetSaveDiskString(item.m_sArchiveFile);

        if(pDlg->exec() == QDialog::Accepted)
        { 
            QStringList sl = pDlg->GetBasicSetting();
            item.m_sDataType = sl.at(0);
            item.m_sName = sl.at(1);
            item.m_sDescription = sl.at(2);
            item.m_sUnit = sl.at(3);
            item.m_sIOConnect = pDlg->GetIOConnectString();
            item.m_sAlarm = pDlg->GetAlarmString();
            item.m_sArchiveFile = pDlg->GetSaveDiskString();
            pTagIOTableModel->UpdateRow(rowIndex, item);
        }
        delete pDlg;
    }
    else if(m_strItemName == "中间变量")
    {
        VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
        pDlg->setWindowTitle(tr("编辑中间变量"));
        TagTmpItem item = pTagTmpTableModel->GetRow(rowIndex);

        // 单元格数据有可能已经改变
        if(item.m_sDataType == "模拟量")
            pDlg->SetVariableType(VariableEditDialog::AI);
        else if(item.m_sDataType == "数字量")
            pDlg->SetVariableType(VariableEditDialog::DI);
        else
            pDlg->SetVariableType(VariableEditDialog::NONE);

        sl << item.m_sDataType << item.m_sName << item.m_sDescription << item.m_sUnit;
        pDlg->SetBasicSetting(sl);

        pDlg->SetCurrentTabIndex(0);
        pDlg->RemoveTab(4); // 隐藏IO连接页
        pDlg->SetDataAttribuyeString(item.m_sDataAttribute);
        pDlg->SetAlarmString(item.m_sAlarm);
        pDlg->SetSaveDiskString(item.m_sArchiveFile);

        if(pDlg->exec() == QDialog::Accepted)
        {      
            QStringList sl = pDlg->GetBasicSetting();
            item.m_sDataType = sl.at(0);
            item.m_sName = sl.at(1);
            item.m_sDescription = sl.at(2);
            item.m_sUnit = sl.at(3);
            item.m_sDataAttribute = pDlg->GetDataAttribuyeString();
            item.m_sAlarm = pDlg->GetAlarmString();
            item.m_sArchiveFile = pDlg->GetSaveDiskString();
            pTagTmpTableModel->UpdateRow(rowIndex, item);
        }
        delete pDlg;
    }
}

/*
* 插槽：删除变量
*/
void VariableManagerWin::VariableDelete()
{
    QItemSelectionModel *pItemSelectionModel = m_variableTableView->selectionModel();
    QModelIndexList modelIndexList = pItemSelectionModel->selectedIndexes();
    QMap<int, int> rowMap;
    foreach (QModelIndex index, modelIndexList)
    {
        rowMap.insert(index.row(), 0);
    }
    int rowToDel;
    QMapIterator<int, int> rowMapIterator(rowMap);
    rowMapIterator.toBack();
    while (rowMapIterator.hasPrevious())
    {
        rowMapIterator.previous();
        rowToDel = rowMapIterator.key();
        if(m_strItemName == tr("设备变量"))
        {
            pTagIOTableModel->removeRow(rowToDel);
        }
        else if(m_strItemName == tr("中间变量"))
        {
            pTagTmpTableModel->removeRow(rowToDel);
        }
    }
}


/*
 * 关闭事件
 */
void VariableManagerWin::closeEvent(QCloseEvent *event)
{
    save();
    event->accept();
}

/*
 * 打开文件
 */
void VariableManagerWin::open()
{

}

/*
 * 保存文件
 */
void VariableManagerWin::save()
{
    saveToFile(DATA_SAVE_FORMAT, m_strItemName);
}

/*
* 显示大图标
*/
void VariableManagerWin::ShowLargeIcon()
{

}

/*
* 显示小图标
*/
void VariableManagerWin::ShowSmallIcon()
{

}



/*
* 获取工程所有变量的名称
* type: IO, TMP, SYS, ALL
*/
void VariableManagerWin::GetAllProjectVariableName(const QString &proj_path,
                                                   QStringList &varList,
                                                   const QString &type)
{
    varList.clear();

    TagTmpTableModel* pTagTmpModel = new TagTmpTableModel();
    TagSystemTableModel *pTagSystemModel = new TagSystemTableModel();
    TagIOTableModel *pTagIOModel = new TagIOTableModel();

    //-------------设备变量------------------//
    if(type == "ALL" || type == "IO")
    {
        QFile loadFileVarList(proj_path + "/DBVarList.odb");
        if (!loadFileVarList.open(QIODevice::ReadOnly))
            return;
        QByteArray loadData = loadFileVarList.readAll();
        QJsonDocument loadDoc(DATA_SAVE_FORMAT == Json ? QJsonDocument::fromJson(loadData) : QJsonDocument::fromBinaryData(loadData));
        loadFileVarList.close();

        QJsonObject jsonDBVarList = loadDoc.object();
        if(jsonDBVarList != QJsonObject())
        {
            QJsonArray DevVarTabArray = jsonDBVarList["DevVarTabArray"].toArray();
            for (int i = 0; i < DevVarTabArray.size(); ++i)
            {
                QJsonObject jsonObj = DevVarTabArray[i].toObject();
                QString varGroupName = jsonObj["name"].toString();
                QString fileDev = proj_path + "/DevVarList-" + varGroupName+ ".odb";
                QFile loadFileDev(fileDev);
                if (!loadFileDev.open(QIODevice::ReadOnly))
                {
                    qWarning("Couldn't open load file: DevVarList.odb.");
                    return ;
                }
                QByteArray loadDataDev = loadFileDev.readAll();
                QJsonDocument loadDocDev(DATA_SAVE_FORMAT == Json ? QJsonDocument::fromJson(loadDataDev) : QJsonDocument::fromBinaryData(loadDataDev));
                pTagIOModel->load(loadDocDev.object());
                foreach(TagIOItem item, pTagIOModel->m_tagIOItems)
                {
                    varList << ("设备变量." + varGroupName + "." + item.m_sName);
                }
                loadFileDev.close();
            }
        }
    }

    //-------------中间变量------------------//
    if(type == "ALL" || type == "TMP")
    {
        QString fileTmp = proj_path + "/TmpVarList.odb";
        QFile loadFileTmp(fileTmp);
        if (!loadFileTmp.open(QIODevice::ReadOnly))
        {
            qWarning("Couldn't open load file: TmpVarList.odb.");
            return;
        }
        QByteArray loadDataTmp = loadFileTmp.readAll();
        QJsonDocument loadDocTmp(DATA_SAVE_FORMAT == Json ? QJsonDocument::fromJson(loadDataTmp) : QJsonDocument::fromBinaryData(loadDataTmp));
        pTagTmpModel->load(loadDocTmp.object());
        foreach(TagTmpItem item, pTagTmpModel->m_tagTmpItems)
        {
            varList << ("中间变量." + item.m_sName);
        }
        loadFileTmp.close();
    }

    //-------------系统变量------------------//
    if(type == "ALL" || type == "SYS")
    {
        QString fileSys = proj_path + "/SysVarList.odb";
        QFile loadFileSys(fileSys);
        if (!loadFileSys.open(QIODevice::ReadOnly))
        {
            qWarning("Couldn't open load file: SysVarList.odb.");
            return;
        }
        QByteArray loadDataSys = loadFileSys.readAll();
        QJsonDocument loadDocSys(DATA_SAVE_FORMAT == Json ? QJsonDocument::fromJson(loadDataSys) : QJsonDocument::fromBinaryData(loadDataSys));
        pTagSystemModel->load(loadDocSys.object());
        foreach(TagSysItem item, pTagSystemModel->m_tagSysItems)
        {
            varList << ("系统变量." + item.m_sName);
        }
        loadFileSys.close();
    }

    delete pTagTmpModel;
    delete pTagSystemModel;
    delete pTagIOModel;

}



