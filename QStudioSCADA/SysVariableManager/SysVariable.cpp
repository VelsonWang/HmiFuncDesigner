#include "SysVariable.h"
#include "ui_SysVariable.h"
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QApplication>
#include <QDataStream>
#include <QFile>
#include <QFontMetrics>
#include <QStyleOptionComboBox>
#include <QDebug>

const int MaxColumns = 6;

TagSystemTableModel::TagSystemTableModel()
{

}

Qt::ItemFlags TagSystemTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
        theFlags |= Qt::ItemIsSelectable|Qt::ItemIsEditable|
                    Qt::ItemIsEnabled;
    return theFlags;
}

QVariant TagSystemTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
        index.row() < 0 || index.row() >= m_tagSysItems.count() ||
        index.column() < 0 || index.column() >= MaxColumns)
        return QVariant();
    const TagSysItem &item = m_tagSysItems.at(index.row());
    if (role == Qt::SizeHintRole) {
        QStyleOptionComboBox option;
        switch (index.column()) {
            case TagID: option.currentText = item.m_TagID;break;
            case Name: option.currentText = item.m_sName;break;
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
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
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
    if (orientation == Qt::Horizontal) {
        switch (section) {
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
    return index.isValid() ? 0 : MaxColumns;
}


bool TagSystemTableModel::setData(const QModelIndex &index,
                         const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole ||
        index.row() < 0 || index.row() >= m_tagSysItems.count() ||
        index.column() < 0 || index.column() >= MaxColumns)
        return false;

    TagSysItem &item = m_tagSysItems[index.row()];
    switch (index.column()) {
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


void TagSystemTableModel::load(const QString &filename, SaveFormat saveFormat)
{
    QFile loadFile(filename);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << QString("Couldn't open load file: %1.").arg(filename);
        return;
    }
    m_tagSysItems.clear();
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(saveData) : QJsonDocument::fromBinaryData(saveData));
    const QJsonObject json = loadDoc.object();
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
        //qSort(m_tagSysItems);
    }

    loadFile.close();
}


void TagSystemTableModel::save(const QString &filename, SaveFormat saveFormat)
{
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << QString("Couldn't open save file: %1.").arg(filename);
        return;
    }

    QJsonObject obj;
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

    obj["SysVarArray"] = tagSysArray;

    QJsonDocument saveDoc(obj);
    saveFile.write(saveFormat == Json ? saveDoc.toJson() : saveDoc.toBinaryData());
    saveFile.close();
}

///////////////////////////////////////////////////////////////////////////////


SysVariable::SysVariable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysVariable)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/pj_var.png"));
    setContextMenuPolicy(Qt::DefaultContextMenu);

    pTableViewVarManagerModel = new TagSystemTableModel(this);
    QString file = QCoreApplication::applicationDirPath() + "/SysVarList.odb";
    pTableViewVarManagerModel->load(file, DATA_SAVE_FORMAT);

    ui->SysVariableManagerTableView->setModel(pTableViewVarManagerModel);
    ui->SysVariableManagerTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->SysVariableManagerTableView->horizontalHeader()->setStretchLastSection(true);
    ui->SysVariableManagerTableView->horizontalHeader()->setHighlightSections(false);

    if(pTableViewVarManagerModel->rowCount() > 0)
        ui->SysVariableManagerTableView->selectRow(0);
}

SysVariable::~SysVariable()
{
    delete ui;
    if(pTableViewVarManagerModel != nullptr) {
        delete pTableViewVarManagerModel;
        pTableViewVarManagerModel = nullptr;
    }
}


/*
* 右键菜单
*/
void SysVariable::contextMenuEvent(QContextMenuEvent * event)
{
    Q_UNUSED(event)

    QMenu *pMenu = new QMenu(this);

    QAction *pAddAct = new QAction(QIcon(":/images/data_add.png"), tr("增加"), this);
    pAddAct->setStatusTip(tr("新增变量"));
    connect(pAddAct, SIGNAL(triggered()), this, SLOT(variableAdd()));
    pMenu->addAction(pAddAct);

    QAction *pDeleteAct = new QAction(QIcon(":/images/data_delete.png"), tr("删除"), this);
    pDeleteAct->setStatusTip(tr("删除"));
    connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(variableDelete()));
    pMenu->addAction(pDeleteAct);

    pMenu->move(cursor().pos());
    pMenu->show();
}


/*
* 插槽：增加变量
*/
void SysVariable::variableAdd()
{
    QModelIndex index;

    index = pTableViewVarManagerModel->index(pTableViewVarManagerModel->rowCount()-1, 0);
    QVariant val = pTableViewVarManagerModel->data(index);

    pTableViewVarManagerModel->insertRows(pTableViewVarManagerModel->rowCount(), 1);

    index = pTableViewVarManagerModel->index(pTableViewVarManagerModel->rowCount()-1, 0);

    int id;
    if(pTableViewVarManagerModel->rowCount() > 0)
        id = val.toInt() + 1;
    else
        id = SYSVARIABLE_BASE + 1;
    pTableViewVarManagerModel->setData(index, id);

    index = pTableViewVarManagerModel->index(pTableViewVarManagerModel->rowCount()-1, 1);
    pTableViewVarManagerModel->setData(index, QString("sys%1").arg(pTableViewVarManagerModel->rowCount()));
}


/*
* 插槽：删除变量
*/
void SysVariable::variableDelete()
{
    QModelIndex ModelIndex = ui->SysVariableManagerTableView->selectionModel()->currentIndex();
    pTableViewVarManagerModel->removeRow(ModelIndex.row(), ModelIndex.parent());
    for(int i = 0; i < pTableViewVarManagerModel->rowCount(); i++) {
        QModelIndex index = pTableViewVarManagerModel->index(i, 1);
        pTableViewVarManagerModel->setData(index, QString("sys%1").arg(i+1));
    }
}


void SysVariable::closeEvent(QCloseEvent *event)
{
    QString file = QCoreApplication::applicationDirPath() + "/SysVarList.odb";
    pTableViewVarManagerModel->save(file, DATA_SAVE_FORMAT);
    QWidget::closeEvent(event);
}
