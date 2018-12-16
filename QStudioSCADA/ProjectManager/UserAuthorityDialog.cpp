#include "UserAuthorityDialog.h"
#include "ui_UserAuthorityDialog.h"
#include <QStandardItem>
#include <QModelIndex>
#include <QMessageBox>
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

const int UserItemMaxColumns = 9;

Qt::ItemFlags TagUserTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
        theFlags |= Qt::ItemIsSelectable|Qt::ItemIsEditable|
                    Qt::ItemIsEnabled;
    return theFlags;
}


QVariant TagUserTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
        index.row() < 0 || index.row() >= m_tagUserItems.count() ||
        index.column() < 0 || index.column() >= UserItemMaxColumns)
        return QVariant();
    const TagUserItem &item = m_tagUserItems.at(index.row());
    if (role == Qt::SizeHintRole)
    {
        QStyleOptionComboBox option;
        switch (index.column())
        {
            case Index: option.currentText = item.m_sIndex;break;
            case Name: option.currentText = item.m_sName;break;
            case PassWord: option.currentText = item.m_sPassWord; break;
            case Authority: option.currentText = item.m_sAuthority; break;
            case Comments: option.currentText = item.m_sComments; break;
            case Name2: option.currentText = item.m_sName2; break;
            case Name3: option.currentText = item.m_sName3; break;
            case Name4: option.currentText = item.m_sName4; break;
            case Name5: option.currentText = item.m_sName5; break;
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
            case Index: return item.m_sIndex;
            case Name: return item.m_sName;
            case PassWord: return item.m_sPassWord;
            case Authority: return item.m_sAuthority;
            case Comments: return item.m_sComments;
            case Name2: return item.m_sName2;
            case Name3: return item.m_sName3;
            case Name4: return item.m_sName4;
            case Name5: return item.m_sName5;

            default: Q_ASSERT(false);
        }
    }
    return QVariant();
}

//enum Column {Index, Name, PassWord, Authority, Comments, Name2, Name3, Name4, Name5};

QVariant TagUserTableModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case Index: return tr("序号");
            case Name: return tr("姓名");
            case PassWord: return tr("密码");
            case Authority: return tr("权限");
            case Comments: return tr("备注");
            case Name2: return tr("姓名2");
            case Name3: return tr("姓名3");
            case Name4: return tr("姓名4");
            case Name5: return tr("姓名5");

            default: Q_ASSERT(false);
        }
    }
    return section + 1;
}


int TagUserTableModel::rowCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : m_tagUserItems.count();
}


int TagUserTableModel::columnCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : UserItemMaxColumns;
}

////enum Column {Index, Name, PassWord, Authority, Comments, Name2, Name3, Name4, Name5};
bool TagUserTableModel::setData(const QModelIndex &index,
                         const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole ||
        index.row() < 0 || index.row() >= m_tagUserItems.count() ||
        index.column() < 0 || index.column() >= UserItemMaxColumns)
        return false;

    TagUserItem &item = m_tagUserItems[index.row()];
    switch (index.column())
    {
        case Index: item.m_sIndex = value.toString(); break;;
        case Name: item.m_sName = value.toString(); break;;
        case PassWord: item.m_sPassWord = value.toString(); break;;
        case Authority: item.m_sAuthority = value.toString(); break;;
        case Comments: item.m_sComments = value.toString(); break;;
        case Name2: item.m_sName2 = value.toString(); break;;
        case Name3: item.m_sName3 = value.toString(); break;;
        case Name4: item.m_sName4 = value.toString(); break;;
        case Name5: item.m_sName5 = value.toString(); break;;

        default: Q_ASSERT(false);
    }
    emit dataChanged(index, index);
    return true;
}


bool TagUserTableModel::insertRows(int row, int count, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_tagUserItems.insert(row, TagUserItem());
    endInsertRows();
    return true;
}


bool TagUserTableModel::removeRows(int row, int count, const QModelIndex&)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_tagUserItems.removeAt(row);
    endRemoveRows();
    return true;
}


void TagUserTableModel::load(const QJsonObject &json)
{
    QJsonObject jsonObj;

    m_tagUserItems.clear();
    QJsonArray UserArray = json["UserArray"].toArray();
    for (int i = 0; i < UserArray.size(); ++i)
    {
        TagUserItem item;
        jsonObj = UserArray[i].toObject();
        item.m_sIndex = jsonObj["sIndex"].toString();
        item.m_sName = jsonObj["sName"].toString();
        item.m_sPassWord = jsonObj["sPassWord"].toString();
        item.m_sAuthority = jsonObj["sAuthority"].toString();
        item.m_sComments = jsonObj["sComments"].toString();
        item.m_sName2 = jsonObj["sName2"].toString();
        item.m_sName3 = jsonObj["sName3"].toString();
        item.m_sName4 = jsonObj["sName4"].toString();
        item.m_sName5 = jsonObj["sName5"].toString();
        m_tagUserItems.append(item);
    }
}


void TagUserTableModel::save(QJsonObject &json)
{
    QJsonArray UserArray;
    QJsonObject jsonObj;
    TagUserItem item;

    for(int i = 0; i < m_tagUserItems.size(); i++)
    {
        item = m_tagUserItems.at(i);
        jsonObj["sIndex"] = item.m_sIndex;
        jsonObj["sName"] = item.m_sName;
        jsonObj["sPassWord"] = item.m_sPassWord;
        jsonObj["sAuthority"] = item.m_sAuthority;
        jsonObj["sComments"] = item.m_sComments;
        jsonObj["sName2"] = item.m_sName2;
        jsonObj["sName3"] = item.m_sName3;
        jsonObj["sName4"] = item.m_sName4;
        jsonObj["sName5"] = item.m_sName5;
        UserArray.append(jsonObj);
    }
    json["UserArray"] = UserArray;
}


/////////////////////////////////////////////////////////


UserAuthorityDialog::UserAuthorityDialog(QWidget *parent, QString ProjectPath) :
    QDialog(parent),
    ui(new Ui::UserAuthorityDialog),
    m_ProjectPath(ProjectPath)
{
    ui->setupUi(this);

    pTableViewUserModel = new TagUserTableModel(this);
    loadFromFile(DATA_SAVE_FORMAT);
    ui->tableViewUser->setModel(pTableViewUserModel);
    ui->tableViewUser->verticalHeader()->hide();
}

UserAuthorityDialog::~UserAuthorityDialog()
{
    delete ui;
}

void UserAuthorityDialog::on_btnHelp_clicked()
{
    // this time we do nothing!
}

void UserAuthorityDialog::on_btnCheck_clicked()
{
    if(check_data())
    {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void UserAuthorityDialog::on_btnOk_clicked()
{
    if(check_data())
    {
        saveToFile(DATA_SAVE_FORMAT);
        QDialog::accept();
    }
}

void UserAuthorityDialog::on_btnExit_clicked()
{
    QDialog::reject();
}

void UserAuthorityDialog::on_brnAddUser_clicked()
{
    QModelIndex index;
    pTableViewUserModel->insertRows(pTableViewUserModel->rowCount(), 1);
    index = pTableViewUserModel->index(pTableViewUserModel->rowCount()-1, 0);
    pTableViewUserModel->setData(index, QString("%1").arg(pTableViewUserModel->rowCount()));
}

void UserAuthorityDialog::on_brnDelUser_clicked()
{
    QModelIndex ModelIndex = ui->tableViewUser->selectionModel()->currentIndex();
    pTableViewUserModel->removeRow(ModelIndex.row(), ModelIndex.parent());
    check_data();
}


void UserAuthorityDialog::on_tableViewUser_clicked(const QModelIndex &index)
{

}

bool UserAuthorityDialog::check_data()
{
    bool ret = true;

    for(int i = 0; i < pTableViewUserModel->rowCount(); i++)
    {
        QModelIndex index = pTableViewUserModel->index(i, 0);
        pTableViewUserModel->setData(index, i+1);
    }
    return ret;
}

void UserAuthorityDialog::load(const QJsonObject &json)
{
    pTableViewUserModel->load(json);
    ui->checkLogIn->setChecked(json["bLogIn"].toBool());
    ui->checkLogOut->setChecked(json["bLogOut"].toBool());
}

void UserAuthorityDialog::save(QJsonObject &json)
{
    pTableViewUserModel->save(json);
    json["bLogIn"] = ui->checkLogIn->isChecked();
    json["bLogOut"] = ui->checkLogOut->isChecked();
}

bool UserAuthorityDialog::loadFromFile(SaveFormat saveFormat)
{
    QString file = m_ProjectPath + "/user.odb";
    QFile loadFile(file);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file.");
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(saveData) : QJsonDocument::fromBinaryData(saveData));
    load(loadDoc.object());
    loadFile.close();
    return true;
}

bool UserAuthorityDialog::saveToFile(SaveFormat saveFormat)
{
    QString file = m_ProjectPath + "/user.odb";
    QFile saveFile(file);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject obj;
    save(obj);
    QJsonDocument saveDoc(obj);
    saveFile.write(saveFormat == Json ? saveDoc.toJson() : saveDoc.toBinaryData());
    saveFile.close();
    return true;
}






