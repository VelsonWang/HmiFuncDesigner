#ifndef USERAUTHORITYDIALOG_H
#define USERAUTHORITYDIALOG_H

#include "corelibglobal.h"
#include <QDialog>
#include <QStandardItemModel>


//////////////////////////////////////////////////////////

struct CORELIB_EXPORT TagUserItem {
    explicit TagUserItem(const QString &sIndex = QString(),
                         const QString &sName = QString(),
                         const QString &sPassWord = QString(),
                         const QString &sAuthority = QString(),
                         const QString &sComments = QString(),
                         const QString &sName2 = QString(),
                         const QString &sName3 = QString(),
                         const QString &sName4 = QString(),
                         const QString &sName5 = QString())
        : m_sIndex(sIndex), m_sName(sName), m_sPassWord(sPassWord), m_sAuthority(sAuthority),
          m_sComments(sComments), m_sName2(sName2), m_sName3(sName3), m_sName4(sName4), m_sName5(sName5)
    {

    }

    QString m_sIndex;
    QString m_sName;
    QString m_sPassWord;
    QString m_sAuthority;
    QString m_sComments;
    QString m_sName2;
    QString m_sName3;
    QString m_sName4;
    QString m_sName5;
};

//////////////////////////////////////////////////////////


enum Column {Index, Name, PassWord, Authority, Comments, Name2, Name3, Name4, Name5};


//////////////////////////////////////////////////////////


class CORELIB_EXPORT TagUserTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TagUserTableModel(QObject *parent = NULL)
        : QAbstractTableModel(parent) {}

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool setHeaderData(int, Qt::Orientation, const QVariant&, int = Qt::EditRole)
    {
        return false;
    }

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

public:
    QList<TagUserItem> m_tagUserItems;
};


//////////////////////////////////////////////////////////


namespace Ui
{
class UserAuthorityDialog;
}

class CORELIB_EXPORT UserAuthorityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserAuthorityDialog(QWidget *parent = NULL);
    ~UserAuthorityDialog();


public:
    void load();
    void save();

private:
    bool check_data();

private slots:
    void on_btnHelp_clicked();
    void on_btnCheck_clicked();
    void on_btnOk_clicked();
    void on_btnExit_clicked();
    void on_brnAddUser_clicked();
    void on_brnDelUser_clicked();

    void on_tableViewUser_clicked(const QModelIndex &index);

private:
    Ui::UserAuthorityDialog *ui;
    TagUserTableModel *pUserModel_;
};

#endif // USERAUTHORITYDIALOG_H
