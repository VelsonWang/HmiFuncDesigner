#include "VariableManagerWin.h"
#include "ui_VariableManagerWin.h"
#include "CommunicationDeviceWin.h"
#include "ui_CommunicationDeviceWin.h"
#include "NewComDeviceDialog.h"
#include "VariableEditDialog.h"
#include "CommentsDialog.h"
#include "stringdata.h"
#include "writer.h"
#include "reader.h"
#include "variantdata.h"
#include "Helper.h"
#include "ProjectMgrUtils.h"
#include "TagManager.h"
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
            case TagID: option.currentText = item.m_sTagID; break;
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
            case TagID: return item.m_sTagID;
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
        case TagID: item.m_sTagID = value.toString(); break;;
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
        item.load(jsonObj);
        m_tagSysItems.append(item);
    }
}


void TagSystemTableModel::save(QJsonObject &json)
{
    QJsonArray tagSysArray;
    for(int i = 0; i < m_tagSysItems.size(); i++) {
        TagSysItem item =  m_tagSysItems.at(i);
        QJsonObject jsonObj;
        item.save(jsonObj);
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
            case TagID: option.currentText = item.m_sTagID;break;
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
            case TagID: return item.m_sTagID;
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
        case TagID: item.m_sTagID = value.toString(); break;
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
        item.load(jsonObj);
        m_tagTmpItems.append(item);
    }

}


void TagTmpTableModel::save(QJsonObject &json)
{
    QJsonArray TmpVarArray;
    for(int i = 0; i < m_tagTmpItems.size(); i++) {
        TagTmpItem item =  m_tagTmpItems.at(i);
        QJsonObject jsonObj;
        item.save(jsonObj);
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
        varRow << item.m_sTagID << item.m_sDataType << item.m_sName
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
    int lastID = 1;
    QString szVarTmp = m_tagTmpItems.last().m_sTagID;
    QString szStartText = "tmp.";
    QString szTmp = "0";
    if(szVarTmp.startsWith(szStartText)) {
        szTmp = szVarTmp.remove(0, szStartText.length());
        lastID = szTmp.toInt();
    }

    int lastNextRow = m_tagTmpItems.size();
    beginInsertRows(QModelIndex(), lastNextRow, lastNextRow + data.size() - 2);
    for(int i=0; i<data.size(); i++)
    {
        QStringList row = data.at(i);
        if(row.at(0) == "ID")
            continue;
        TagTmpItem item;

        int id = 1;
        szVarTmp = row.at(0);
        szTmp = "0";
        if(szVarTmp.startsWith(szStartText)) {
            szTmp = szVarTmp.remove(0, szStartText.length());
            id = szTmp.toInt();
        }

        item.m_sTagID = QString("tmp.%1").arg(QString::number(lastID + id));
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
            case TagID: option.currentText = item.m_sTagID;break;
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
            case TagID: return item.m_sTagID;
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
        case TagID: item.m_sTagID = value.toString(); break;
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
        item.load(jsonObj);
        m_tagIOItems.append(item);
    }
}


void TagIOTableModel::save(QJsonObject &json)
{
    QJsonArray IOVarArray;
    for(int i = 0; i < m_tagIOItems.size(); i++) {
        TagIOItem item =  m_tagIOItems.at(i);
        QJsonObject jsonObj;
        item.save(jsonObj);
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
        varRow << item.m_sTagID << item.m_sDataType << item.m_sName
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
    int lastID = 1;
    QString szVarTmp = "";
    QString szStartText = "io.";
    QString szTmp = "0";
    if(lastNextRow != 0) {
        szVarTmp = m_tagIOItems.last().m_sTagID;
        if(szVarTmp.startsWith(szStartText)) {
            int iPos = szVarTmp.lastIndexOf(".");
            szTmp = szVarTmp.right(szVarTmp.length() - iPos - 1);
            lastID = szTmp.toInt();
        }
    }

    beginInsertRows(QModelIndex(), lastNextRow, lastNextRow + data.size() - 2);
    for(int i=0; i<data.size(); i++)
    {
        QStringList row = data.at(i);
        if(row.at(0) == "ID")
            continue;
        TagIOItem item;

        int id = 1;
        int iPos = -1;
        szVarTmp = row.at(0);
        if(szVarTmp.startsWith(szStartText)) {
            iPos = szVarTmp.lastIndexOf(".");
            szTmp = szVarTmp.right(szVarTmp.length() - iPos - 1);
            id = szTmp.toInt();
        }

        item.m_sTagID = QString("%1%2")
                .arg(szVarTmp.left(iPos + 1))
                .arg(QString::number(lastID + id));

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
    if(pTagSystemTableModel) {
        delete pTagSystemTableModel;
        pTagSystemTableModel = nullptr;
    }
    if(pTagTmpTableModel) {
        delete pTagTmpTableModel;
        pTagTmpTableModel = nullptr;
    }
    if(pTagIOTableModel) {
        delete pTagIOTableModel;
        pTagIOTableModel = nullptr;
    }
}

void VariableManagerWin::init(const QString &itemName)
{   
    if(m_strCurTagType != itemName) {
        if(!m_strCurTagType.isEmpty()) {
            SetTitle(m_strCurTagType);
            save();
        }

        /////////////////////////////////////////////////////

        if(m_strItemName == tr("设备变量")) {
            if(pTagIOTableModel) {
                delete pTagIOTableModel;
                pTagIOTableModel = new TagIOTableModel(this);
            }
        } else if(m_strItemName == tr("中间变量")) {
            if(pTagTmpTableModel) {
                delete pTagTmpTableModel;
                pTagTmpTableModel = new TagTmpTableModel(this);
            }
        } else if(m_strItemName == tr("系统变量")) {
            if(pTagSystemTableModel) {
                delete pTagSystemTableModel;
                pTagSystemTableModel = new TagSystemTableModel(this);
            }
        }

        ////////////////////////////////////////////////////

        SetTitle(itemName);
        loadFromFile(DATA_SAVE_FORMAT, m_strItemName);
    }

    if(itemName.indexOf(tr("设备变量-")) > -1)
        SetTitle(itemName);

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
    m_variableTableView->horizontalHeader()->setStretchLastSection(true);
    m_variableTableView->horizontalHeader()->setHighlightSections(false);  // 当表格只有一行的时候，则表头会出现塌陷问题
}

void VariableManagerWin::SetTitle(const QString &t)
{
    if(t == tr("中间变量") || t == tr("系统变量")) {
        m_strItemName = t;        
    } else {
        m_strItemName = tr("设备变量");
        QString str = t;
        m_IOVariableListWhat = str.replace(QString(tr("设备变量-")), "");
    }
    m_strCurTagType = t;
}


bool VariableManagerWin::loadFromFile(SaveFormat saveFormat, const QString &it)
{
    QString file = "";

    if(it == tr("设备变量")) {
        file = ProjectMgrUtils::getProjectPath(m_strProjectName) + "/DevVarList-" + m_IOVariableListWhat + ".odb";
    } else if(it == tr("中间变量")) {
        file = ProjectMgrUtils::getProjectPath(m_strProjectName) + "/TmpVarList.odb";
    } else if(it == tr("系统变量")) {
        file = ProjectMgrUtils::getProjectPath(m_strProjectName) + "/SysVarList.odb";
        QString srcfile = QCoreApplication::applicationDirPath() + "/SysVarList.odb";
        QFile testFile(file);
        if(testFile.exists()) {
            testFile.remove();
        }
        QFile::copy(srcfile ,file);
    }

    QFile loadFile(file);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << QString("Couldn't open load file: %1.").arg(file);
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
    if(it == tr("设备变量")) {
        file = ProjectMgrUtils::getProjectPath(m_strProjectName) + "/DevVarList-" + m_IOVariableListWhat + ".odb";
    } else if(it == tr("中间变量")) {
        file = ProjectMgrUtils::getProjectPath(m_strProjectName) + "/TmpVarList.odb";
    } else if(it == tr("系统变量")) {
        return true;
    }
    QFile saveFile(file);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << QString("Couldn't open save file: %1.").arg(file);
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
    if(it == "设备变量") {
        pTagIOTableModel->load(json);
    } else if(it == "中间变量") {
        pTagTmpTableModel->load(json);
    } else if(it == "系统变量") {
        pTagSystemTableModel->load(json);
    }
}

void VariableManagerWin::save(QJsonObject &json, const QString &it)
{
    if(it == "设备变量") {
        pTagIOTableModel->save(json);
    } else if(it == "中间变量") {
        pTagTmpTableModel->save(json);
    } else if(it == "系统变量") {
        // do nothing here!
    }
}

/*
* 导出变量表
*/
void VariableManagerWin::exportToCsv(const QString &path)
{
    if(m_strItemName == tr("设备变量")) {
        pTagIOTableModel->exportToCsv(path, QString("%1-%2").arg(m_strItemName).arg(m_IOVariableListWhat));
    } else if(m_strItemName == tr("中间变量")) {
        pTagTmpTableModel->exportToCsv(path, m_strItemName);
    }
}

/*
* 导入变量表
*/
void VariableManagerWin::importFromCsv(const QString &path)
{
    if(path.contains(tr("设备变量"))) {
        pTagIOTableModel->importFromCsv(path);
    } else if(path.contains(tr("中间变量"))) {
        pTagTmpTableModel->importFromCsv(path);
    }
}


/*
* 右键菜单
*/
void VariableManagerWin::contextMenuEvent(QContextMenuEvent * /*event*/)
{
    if(m_strItemName == tr("变量管理") || m_strItemName == tr("系统变量"))
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
    int rowIndex = index.row();
    int columnIndex = index.column();

    if(m_strItemName == tr("设备变量")) {
        TagIOItem item = pTagIOTableModel->GetRow(rowIndex);
        if(columnIndex == pTagIOTableModel->Column::IOConnect ||
           columnIndex == pTagIOTableModel->Column::Alarm ||
           columnIndex == pTagIOTableModel->Column::ArchiveFile) {
            VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
            pDlg->setWindowTitle(tr("编辑设备变量"));
            sl << item.m_sDataType << item.m_sName << item.m_sDescription << item.m_sUnit;

            // 单元格数据有可能已经改变
            if(item.m_sDataType == tr("模拟量"))
                pDlg->SetVariableType(VariableEditDialog::AI);
            else if(item.m_sDataType == tr("数字量"))
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

            if(pDlg->exec() == QDialog::Accepted) {
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
        } else if(columnIndex == pTagIOTableModel->Column::ProjectConverter) {
            TagFuncEditDialog *pDlg = new TagFuncEditDialog(ProjectMgrUtils::getProjectPath(m_strProjectName), this);
            pDlg->SetData(item.m_sProjectConverter);
            if(pDlg->exec() == QDialog::Accepted) {
                item.m_sProjectConverter = pDlg->GetData();
                pTagIOTableModel->UpdateRow(rowIndex, item);
            }
            delete pDlg;
        } else if(columnIndex == pTagIOTableModel->Column::Comments) {
            CommentsDialog *pDlg = new CommentsDialog(this);
            pDlg->setCommentsText(item.m_sComments);
            if(pDlg->exec() == QDialog::Accepted) {
                item.m_sComments = pDlg->getCommentsText();
                pTagIOTableModel->UpdateRow(rowIndex, item);
            }
            delete pDlg;
        }
    } else if(m_strItemName == tr("中间变量")) {
        TagTmpItem item = pTagTmpTableModel->GetRow(rowIndex);
        if(columnIndex == pTagTmpTableModel->Column::DataAttribute ||
           columnIndex == pTagTmpTableModel->Column::Alarm ||
           columnIndex == pTagTmpTableModel->Column::ArchiveFile) {
            VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
            pDlg->setWindowTitle(tr("编辑中间变量"));
            sl << item.m_sDataType << item.m_sName << item.m_sDescription << item.m_sUnit;

            // 单元格数据有可能已经改变
            if(item.m_sDataType == tr("模拟量"))
                pDlg->SetVariableType(VariableEditDialog::AI);
            else if(item.m_sDataType == tr("数字量"))
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

            if(pDlg->exec() == QDialog::Accepted) {
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
        } else if(columnIndex == pTagTmpTableModel->Column::ProjectConverter) {
            TagFuncEditDialog *pDlg = new TagFuncEditDialog(ProjectMgrUtils::getProjectPath(m_strProjectName), this);
            pDlg->SetData(item.m_sProjectConverter);
            if(pDlg->exec() == QDialog::Accepted) {
                item.m_sProjectConverter = pDlg->GetData();
                pTagTmpTableModel->UpdateRow(rowIndex, item);
            }
            delete pDlg;
        } else if(columnIndex == pTagTmpTableModel->Column::Comments) {
            CommentsDialog *pDlg = new CommentsDialog(this);
            pDlg->setCommentsText(item.m_sComments);
            if(pDlg->exec() == QDialog::Accepted) {
                item.m_sComments = pDlg->getCommentsText();
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
    bool ok = false;
    int i;
    int num;

    if(m_strItemName == tr("设备变量")) {
        VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
        pDlg->RemoveTab(1); // 隐藏数据属性页
        if(pDlg->exec() == QDialog::Accepted) {
            num = pDlg->GetBatchNum().toInt(&ok, 10);
            if(ok) {
                int iPageID = 0;
                QString szProjectPath = ProjectMgrUtils::getProjectPath(m_strProjectName);
                TagManager::clearData();
                TagManager::loadProjectTags(szProjectPath);
                foreach(DBVarGroup *var, TagManager::ioDBVarGroups_.varBlockGroupList_) {
                    if(m_IOVariableListWhat == var->m_name) {
                        iPageID = static_cast<int>(var->m_iPageID);
                        break;
                    }
                }

                for(i=0; i<num; i++) {
                    TagIOItem prevItem;
                    TagIOItem newItem;

                    int id = 1;
                    int iPos = -1;
                    QString szVarTmp = "";
                    QString szTmp = "0";
                    if(pTagIOTableModel->rowCount() > 0) {
                        prevItem = pTagIOTableModel->GetRow(pTagIOTableModel->rowCount()-1);
                        szVarTmp = prevItem.m_sTagID;
                        if(szVarTmp.startsWith("io.")) {
                            iPos = szVarTmp.lastIndexOf(".");
                            szTmp = szVarTmp.right(szVarTmp.length() - iPos - 1);
                            id = szTmp.toInt() + 1;
                        }
                    }

                    if(iPos == -1) {
                        newItem.m_sTagID =  QString("io.group%1.%2")
                                .arg(QString::number(iPageID))
                                .arg(QString::number(id));
                    } else {
                        newItem.m_sTagID =  QString("%1%2")
                                .arg(szVarTmp.left(iPos + 1))
                                .arg(QString::number(id));
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
        }
        delete pDlg;
    } else if(m_strItemName == tr("中间变量")) {
        VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
        pDlg->RemoveTab(4); // 隐藏IO连接页
        if(pDlg->exec() == QDialog::Accepted) {
            num = pDlg->GetBatchNum().toInt(&ok, 10);
            if(ok)
                for(i=0; i<num; i++) {
                    TagTmpItem prevItem;
                    TagTmpItem newItem;
                    int id = 1;
                    if(pTagTmpTableModel->rowCount() > 0) {
                        prevItem = pTagTmpTableModel->GetRow(pTagTmpTableModel->rowCount()-1);
                        QString szVarTmp = prevItem.m_sTagID;
                        QString szStartText = "tmp.";
                        QString szTmp = "0";
                        if(szVarTmp.startsWith(szStartText)) {
                            szTmp = szVarTmp.remove(0, szStartText.length());
                            id = szTmp.toInt() + 1;
                        }
                    }
                    newItem.m_sTagID = QString("tmp.%1").arg(QString::number(id));

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
    if(m_strItemName == tr("设备变量")) {
        if(pTagIOTableModel->rowCount() < 1)
            return;

        TagIOItem prevItem = pTagIOTableModel->GetRow(pTagIOTableModel->rowCount()-1);
        TagIOItem newItem;

        int id = 1;
        int iPos = 0;
        QString szVarTmp = "";
        QString szTmp = "0";

        prevItem = pTagIOTableModel->GetRow(pTagIOTableModel->rowCount()-1);
        szVarTmp = prevItem.m_sTagID;
        if(szVarTmp.startsWith("io.")) {
            iPos = szVarTmp.lastIndexOf(".");
            szTmp = szVarTmp.right(szVarTmp.length() - iPos - 1);
            id = szTmp.toInt() + 1;
        }

        newItem.m_sTagID =  QString("%1%2")
                .arg(szVarTmp.left(iPos + 1))
                .arg(QString::number(id));

        newItem.m_sDataType = tr("模拟量");
        // 获取前一行的Name
        QString lastVarName = prevItem.m_sName;
        QString str = "var";
        if(lastVarName.indexOf("var") > -1) {
            int len = lastVarName.size();
            QString subStr = lastVarName.right(len - 3);
            int idx = subStr.toInt();
            str = QString("var%1").arg(idx+1);
        }
        newItem.m_sName = str;
        newItem.m_sDescription = tr("描述");
        newItem.m_sUnit = "mA";
        pTagIOTableModel->AppendRow(newItem);
    } else if(m_strItemName == "中间变量") {
        if(pTagTmpTableModel->rowCount() < 1)
            return;

        TagTmpItem prevItem = pTagTmpTableModel->GetRow(pTagTmpTableModel->rowCount()-1);
        TagTmpItem newItem;

        QString szVarTmp = prevItem.m_sTagID;
        QString szStartText = "tmp.";
        QString szTmp = "0";
        int id = 1;
        if(szVarTmp.startsWith(szStartText)) {
            szTmp = szVarTmp.remove(0, szStartText.length());
            id = szTmp.toInt() + 1;
        }
        newItem.m_sTagID = QString("tmp.%1").arg(QString::number(id));

        newItem.m_sDataType = tr("模拟量");
        // 获取前一行的Name
        QString lastVarName = prevItem.m_sName;
        QString str = "var";
        if(lastVarName.indexOf("var") > -1) {
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
    int row = ModelIndex.row();
    int column = ModelIndex.column();

    if(row < 0 || column < 0)
        return;

    if(m_strItemName == tr("设备变量")) {
        TagIOItem curitem = pTagIOTableModel->GetRow(row);
        TagIOItem lastitem = pTagIOTableModel->GetRow(pTagIOTableModel->rowCount()-1);

        int id = 1;
        int iPos = 0;
        QString szVarTmp = "";
        QString szTmp = "0";

        szVarTmp = lastitem.m_sTagID;
        if(szVarTmp.startsWith("io.")) {
            iPos = szVarTmp.lastIndexOf(".");
            szTmp = szVarTmp.right(szVarTmp.length() - iPos - 1);
            id = szTmp.toInt() + 1;
        }

        curitem.m_sTagID =  QString("%1%2").arg(szVarTmp.left(iPos + 1)).arg(QString::number(id));

        pTagIOTableModel->AppendRow(curitem);
    } else if(m_strItemName == tr("中间变量")) {
        TagTmpItem curitem = pTagTmpTableModel->GetRow(row);
        TagTmpItem lastitem = pTagTmpTableModel->GetRow(pTagTmpTableModel->rowCount()-1);

        QString szVarTmp = lastitem.m_sTagID;
        QString szStartText = "tmp.";
        QString szTmp = "0";
        int id = 1;
        if(szVarTmp.startsWith(szStartText)) {
            szTmp = szVarTmp.remove(0, szStartText.length());
            id = szTmp.toInt() + 1;
        }
        curitem.m_sTagID = QString("tmp.%1").arg(QString::number(id));

        pTagTmpTableModel->AppendRow(curitem);
    }

}

/*
* 插槽：列拷贝变量
*/
void VariableManagerWin::VariableColCopy()
{
    if(m_strItemName == tr("设备变量")) {

    } else if(m_strItemName == tr("中间变量")) {

    }
}

/*
* 插槽：修改变量
*/
void VariableManagerWin::VariableModify()
{
    QStringList sl;
    int rowIndex = m_variableTableView->currentIndex().row();

    if(m_strItemName == tr("设备变量")) {
        VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
        pDlg->setWindowTitle(tr("编辑设备变量"));
        TagIOItem item = pTagIOTableModel->GetRow(rowIndex);

        if(item.m_sDataType == tr("模拟量"))
            pDlg->SetVariableType(VariableEditDialog::AI);
        else if(item.m_sDataType == tr("数字量"))
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

        if(pDlg->exec() == QDialog::Accepted) {
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
    } else if(m_strItemName == tr("中间变量")) {
        VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
        pDlg->setWindowTitle(tr("编辑中间变量"));
        TagTmpItem item = pTagTmpTableModel->GetRow(rowIndex);

        // 单元格数据有可能已经改变
        if(item.m_sDataType == tr("模拟量"))
            pDlg->SetVariableType(VariableEditDialog::AI);
        else if(item.m_sDataType == tr("数字量"))
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

        if(pDlg->exec() == QDialog::Accepted) {
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
    foreach (QModelIndex index, modelIndexList) {
        rowMap.insert(index.row(), 0);
    }
    int rowToDel;
    QMapIterator<int, int> rowMapIterator(rowMap);
    rowMapIterator.toBack();
    while (rowMapIterator.hasPrevious()) {
        rowMapIterator.previous();
        rowToDel = rowMapIterator.key();
        if(m_strItemName == tr("设备变量")) {
            pTagIOTableModel->removeRow(rowToDel);
        } else if(m_strItemName == tr("中间变量")) {
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
    if(!m_strItemName.isEmpty())
        saveToFile(DATA_SAVE_FORMAT, m_strItemName);
}

/*
* 显示大图标
*/
void VariableManagerWin::showLargeIcon()
{

}

/*
* 显示小图标
*/
void VariableManagerWin::showSmallIcon()
{

}



