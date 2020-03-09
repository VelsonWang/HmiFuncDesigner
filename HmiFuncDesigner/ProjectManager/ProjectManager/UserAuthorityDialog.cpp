#include "UserAuthorityDialog.h"
#include "ui_UserAuthorityDialog.h"
#include "ProjectData.h"
#include <QStandardItem>
#include <QModelIndex>
#include <QMessageBox>
#include <QFile>
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
        QFontMetrics fontMetrics(data(index, Qt::FontRole).value<QFont>());
        option.fontMetrics = fontMetrics;
        QSize size(fontMetrics.width(option.currentText), fontMetrics.height());
        return qApp->style()->sizeFromContents(QStyle::CT_ComboBox, &option, size);
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


/////////////////////////////////////////////////////////


UserAuthorityDialog::UserAuthorityDialog(QWidget *parent, const QString &ProjectPath) :
    QDialog(parent),
    ui(new Ui::UserAuthorityDialog),
    szProjectPath_(ProjectPath)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    pUserModel_ = new TagUserTableModel(this);
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
    pUserModel_->insertRows(pUserModel_->rowCount(), 1);
    index = pUserModel_->index(pUserModel_->rowCount()-1, 0);
    pUserModel_->setData(index, QString("%1").arg(pUserModel_->rowCount()));
}

void UserAuthorityDialog::on_brnDelUser_clicked()
{
    QModelIndex ModelIndex = ui->tableViewUser->selectionModel()->currentIndex();
    pUserModel_->removeRow(ModelIndex.row(), ModelIndex.parent());
    check_data();
}


void UserAuthorityDialog::on_tableViewUser_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
}

bool UserAuthorityDialog::check_data()
{
    bool ret = true;

    for(int i = 0; i < pUserModel_->rowCount(); i++)
    {
        QModelIndex index = pUserModel_->index(i, 0);
        pUserModel_->setData(index, i+1);
    }
    return ret;
}

void UserAuthorityDialog::load()
{
    UserAuthority &userAuthority = ProjectData::getInstance()->userAuthority_;
    userAuthority.load(ProjectData::getInstance()->dbData_);

    pUserModel_->m_tagUserItems.clear();
    for (int i = 0; i < userAuthority.getCount(); ++i) {
        TagUserItem item;
        item.m_sIndex = userAuthority.getIndex(i);
        item.m_sName = userAuthority.getName(i);
        item.m_sPassWord = userAuthority.getPassword(i);
        item.m_sAuthority = userAuthority.getAuthority(i);
        item.m_sComments = userAuthority.getComments(i);
        item.m_sName2 = userAuthority.getName2(i);
        item.m_sName3 = userAuthority.getName3(i);
        item.m_sName4 = userAuthority.getName4(i);
        item.m_sName5 = userAuthority.getName5(i);
        pUserModel_->m_tagUserItems.append(item);
    }

    if(userAuthority.getCount()) {
        ui->checkLogIn->setChecked(userAuthority.isLogin(0));
        ui->checkLogOut->setChecked(userAuthority.isLogout(0));
    }

    ui->tableViewUser->setModel(pUserModel_);
}

void UserAuthorityDialog::save()
{
    UserAuthority &userAuthority = ProjectData::getInstance()->userAuthority_;
    userAuthority.delAll(ProjectData::getInstance()->dbData_);
    TagUserItem item;

    for(int i = 0; i < pUserModel_->m_tagUserItems.size(); i++)
    {
        UserAuthorityPrivate *pObj = new UserAuthorityPrivate();
        item = pUserModel_->m_tagUserItems.at(i);
        pObj->szIndex_ = item.m_sIndex;
        pObj->szName_ = item.m_sName;
        pObj->szPassword_ = item.m_sPassWord;
        pObj->szAuthority_ = item.m_sAuthority;
        pObj->szComments_ = item.m_sComments;
        pObj->szName2_ = item.m_sName2;
        pObj->szName3_ = item.m_sName3;
        pObj->szName4_ = item.m_sName4;
        pObj->szName5_ = item.m_sName5;
        pObj->bLogin_ = ui->checkLogIn->isChecked();
        pObj->bLogout_ = ui->checkLogOut->isChecked();
        userAuthority.insert(ProjectData::getInstance()->dbData_, pObj);
    }
}











