#include "userauthoritydialog.h"
#include "ui_userauthoritydialog.h"
#include "qsoftcore.h"
#include "../shared/qprojectcore.h"
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
        theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEditable |
                    Qt::ItemIsEnabled;
    return theFlags;
}


QVariant TagUserTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
            index.row() < 0 || index.row() >= m_tagUserItems.count() ||
            index.column() < 0 || index.column() >= UserItemMaxColumns) {
        return QVariant();
    }
    const TagUserItem &item = m_tagUserItems.at(index.row());
    if (role == Qt::SizeHintRole) {
        QStyleOptionComboBox option;
        switch (index.column()) {
            case Index:
                option.currentText = item.m_index;
                break;
            case Name:
                option.currentText = item.m_name;
                break;
            case PassWord:
                option.currentText = item.m_passWord;
                break;
            case Authority:
                option.currentText = item.m_authority;
                break;
            case Comments:
                option.currentText = item.m_comments;
                break;
            case Name2:
                option.currentText = item.m_name2;
                break;
            case Name3:
                option.currentText = item.m_name3;
                break;
            case Name4:
                option.currentText = item.m_name4;
                break;
            case Name5:
                option.currentText = item.m_name5;
                break;
            default:
                Q_ASSERT(false);
        }
        QFontMetrics fontMetrics(data(index, Qt::FontRole).value<QFont>());
        option.fontMetrics = fontMetrics;
        QSize size(fontMetrics.horizontalAdvance(option.currentText), fontMetrics.height());
        return qApp->style()->sizeFromContents(QStyle::CT_ComboBox, &option, size);
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case Index:
                return item.m_index;
            case Name:
                return item.m_name;
            case PassWord:
                return item.m_passWord;
            case Authority:
                return item.m_authority;
            case Comments:
                return item.m_comments;
            case Name2:
                return item.m_name2;
            case Name3:
                return item.m_name3;
            case Name4:
                return item.m_name4;
            case Name5:
                return item.m_name5;

            default:
                Q_ASSERT(false);
        }
    }
    return QVariant();
}


QVariant TagUserTableModel::headerData(int section,
                                       Qt::Orientation orientation,
                                       int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Horizontal) {
        switch (section) {
            case Index:
                return tr("序号");
            case Name:
                return tr("姓名");
            case PassWord:
                return tr("密码");
            case Authority:
                return tr("权限");
            case Comments:
                return tr("备注");
            case Name2:
                return tr("姓名2");
            case Name3:
                return tr("姓名3");
            case Name4:
                return tr("姓名4");
            case Name5:
                return tr("姓名5");

            default:
                Q_ASSERT(false);
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
            index.column() < 0 || index.column() >= UserItemMaxColumns) {
        return false;
    }

    TagUserItem &item = m_tagUserItems[index.row()];
    switch (index.column()) {
        case Index:
            item.m_index = value.toString();
            break;
        case Name:
            item.m_name = value.toString();
            break;
        case PassWord:
            item.m_passWord = value.toString();
            break;
        case Authority:
            item.m_authority = value.toString();
            break;
        case Comments:
            item.m_comments = value.toString();
            break;
        case Name2:
            item.m_name2 = value.toString();
            break;
        case Name3:
            item.m_name3 = value.toString();
            break;
        case Name4:
            item.m_name4 = value.toString();
            break;
        case Name5:
            item.m_name5 = value.toString();
            break;

        default:
            Q_ASSERT(false);
    }
    emit dataChanged(index, index);
    return true;
}


bool TagUserTableModel::insertRows(int row, int count, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        m_tagUserItems.insert(row, TagUserItem());
    }
    endInsertRows();
    return true;
}


bool TagUserTableModel::removeRows(int row, int count, const QModelIndex&)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        m_tagUserItems.removeAt(row);
    }
    endRemoveRows();
    return true;
}


/////////////////////////////////////////////////////////


UserAuthorityDialog::UserAuthorityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserAuthorityDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    pUserModel = new TagUserTableModel(this);
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
    if(check_data()) {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void UserAuthorityDialog::on_btnOk_clicked()
{
    if(check_data()) {
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
    pUserModel->insertRows(pUserModel->rowCount(), 1);
    index = pUserModel->index(pUserModel->rowCount() - 1, 0);
    pUserModel->setData(index, QString("%1").arg(pUserModel->rowCount()));
}

void UserAuthorityDialog::on_brnDelUser_clicked()
{
    QModelIndex ModelIndex = ui->tableViewUser->selectionModel()->currentIndex();
    pUserModel->removeRow(ModelIndex.row(), ModelIndex.parent());
    check_data();
}


void UserAuthorityDialog::on_tableViewUser_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
}

bool UserAuthorityDialog::check_data()
{
    bool ret = true;

    for(int i = 0; i < pUserModel->rowCount(); i++) {
        QModelIndex index = pUserModel->index(i, 0);
        pUserModel->setData(index, i + 1);
    }
    return ret;
}

void UserAuthorityDialog::load()
{
    UserAuthority &userAuthority = QSoftCore::getCore()->getProjectCore()->m_userAuthority;
    pUserModel->m_tagUserItems.clear();
    for (int i = 0; i < userAuthority.getCount(); ++i) {
        TagUserItem item;
        item.m_index = userAuthority.getIndex(i);
        item.m_name = userAuthority.getName(i);
        item.m_passWord = userAuthority.getPassword(i);
        item.m_authority = userAuthority.getAuthority(i);
        item.m_comments = userAuthority.getComments(i);
        item.m_name2 = userAuthority.getName2(i);
        item.m_name3 = userAuthority.getName3(i);
        item.m_name4 = userAuthority.getName4(i);
        item.m_name5 = userAuthority.getName5(i);
        pUserModel->m_tagUserItems.append(item);
    }

    if(userAuthority.getCount()) {
        ui->checkLogIn->setChecked(userAuthority.isLogin(0));
        ui->checkLogOut->setChecked(userAuthority.isLogout(0));
    }

    ui->tableViewUser->setModel(pUserModel);
}

void UserAuthorityDialog::save()
{
    UserAuthority &userAuthority = QSoftCore::getCore()->getProjectCore()->m_userAuthority;
    userAuthority.m_userAuthority.clear();
    TagUserItem item;

    for(int i = 0; i < pUserModel->m_tagUserItems.size(); i++) {
        UserAuthorityPrivate *pObj = new UserAuthorityPrivate();
        item = pUserModel->m_tagUserItems.at(i);
        pObj->m_index = item.m_index;
        pObj->m_name = item.m_name;
        pObj->m_password = item.m_passWord;
        pObj->m_authority = item.m_authority;
        pObj->m_comments = item.m_comments;
        pObj->m_name2 = item.m_name2;
        pObj->m_name3 = item.m_name3;
        pObj->m_name4 = item.m_name4;
        pObj->m_name5 = item.m_name5;
        pObj->m_login = ui->checkLogIn->isChecked();
        pObj->m_logout = ui->checkLogOut->isChecked();
        userAuthority.m_userAuthority.append(pObj);
    }
}











