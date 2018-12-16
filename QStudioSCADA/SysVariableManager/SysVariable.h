#ifndef SYSVARIABLE_H
#define SYSVARIABLE_H

#include "../Public/Public.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QAbstractTableModel>

namespace Ui {
class SysVariable;
}

#define SYSVARIABLE_BASE    (0)
#define TMPVARIABLE_BASE    (10000)
#define IOVARIABLE_BASE     (30000)

enum TAG_TYPE{
    TAG_SYS,
    TAG_IO,
    TAG_TMP
};
typedef TAG_TYPE TTag;

//////////////////////////////////////////////////////////

struct TagSysItem
{
    explicit TagSysItem(int varid=0,
            TTag tagtype=TAG_SYS,
            const QString &sName=QString(),
            const QString &sDescription=QString(),
            const QString &sUnit=QString(),
            const QString &sProjectConverter=QString(),
            const QString &sComments=QString())
        : m_TagID(varid),m_iTagType(tagtype), m_sName(sName), m_sDescription(sDescription),
          m_sUnit(sUnit),m_sProjectConverter(sProjectConverter),m_sComments(sComments) {}

    int m_TagID; // 变量ID
    TTag m_iTagType;  // 变量类型
    QString m_sName;  //名称
    QString m_sDescription;  //描述
    QString m_sUnit;  //单位
    QString m_sProjectConverter;  //工程转换
    QString m_sComments;  //备注
};

//////////////////////////////////////////////////////////


enum Column {TagID, Name, Description, Unit, ProjectConverter, Comments};


//////////////////////////////////////////////////////////


class TagSystemTableModel : public QAbstractTableModel
{
public:
    TagSystemTableModel();

    Q_OBJECT

public:
    explicit TagSystemTableModel(QObject *parent=0)
        : QAbstractTableModel(parent) {}

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    bool setHeaderData(int, Qt::Orientation, const QVariant&, int=Qt::EditRole) { return false; }

    bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());

    QString filename() const { return m_filename; }
    void load(const QString &filename=QString(), SaveFormat saveFormat=Json);
    void save(const QString &filename=QString(), SaveFormat saveFormat=Json);

private:
    QString m_filename;
    QList<TagSysItem> m_tagSysItems;
};


//////////////////////////////////////////////////////////


class SysVariable : public QWidget
{
    Q_OBJECT

public:
    explicit SysVariable(QWidget *parent = 0);
    ~SysVariable();

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void closeEvent(QCloseEvent *event);  // 关闭事件

private slots:
    void on_SysVariableManagerTableView_doubleClicked(const QModelIndex &index);
    void VariableAdd();
    void VariableDelete();

private:
    Ui::SysVariable *ui;
    TagSystemTableModel *pTableViewVarManagerModel;

};

#endif // SYSVARIABLE_H
