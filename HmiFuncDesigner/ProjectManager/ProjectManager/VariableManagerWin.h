#ifndef VARIABLEMANAGERWIN_H
#define VARIABLEMANAGERWIN_H

#include "ChildBase.h"
#include "TagFuncEditDialog.h"
#include "../../Public/Public.h"
#include "TagManager.h"
#include <QList>
#include <QString>
#include <QWidget>
#include <QStandardItemModel>
#include <QJsonObject>
#include <QItemDelegate>
#include <QStringList>
#include <QTableView>
#include <QVBoxLayout>

#define IOVARIABLE_BASE     (0)

class TagSystemTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        TagID,
        Name,
        Description,
        Unit,
        ProjectConverter,
        Comments
    };

public:
    explicit TagSystemTableModel(QObject *parent=nullptr)
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

    void load(const QJsonObject &json);
    void save(QJsonObject &json);

public:
    QList<TagSysItem> m_tagSysItems;
};

//////////////////////////////////////////////////////////

class TagTmpTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        TagID,
        DataType,
        Name,
        Description,
        Unit,
        ActionScope,
        DataAttribute,
        Alarm,
        ArchiveFile,
        ProjectConverter,
        Comments
    };

public:
    explicit TagTmpTableModel(QObject *parent=nullptr)
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

    void load(const QJsonObject &json);
    void save(QJsonObject &json);

    void exportToCsv(QString path, QString group);
    void importFromCsv(QString path);

public:
    void AppendRow(TagTmpItem item);
    void InsertRow(int i, TagTmpItem item);
    TagTmpItem GetRow(int i);
    void UpdateRow(int i, TagTmpItem item);

public:
    QList<TagTmpItem> m_tagTmpItems;
};

//////////////////////////////////////////////////////////

class TagIOTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        TagID,
        DataType,
        Name,
        Description,
        Unit,
        IOConnect,
        Alarm,
        ArchiveFile,
        ProjectConverter,
        Comments
    };

public:
    explicit TagIOTableModel(QObject *parent=nullptr)
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

    void load(const QJsonObject &json);
    void save(QJsonObject &json);

    void exportToCsv(QString path, QString group);
    void importFromCsv(QString path);

public:
    void AppendRow(TagIOItem item);
    void InsertRow(int i, TagIOItem item);
    TagIOItem GetRow(int i);
    void UpdateRow(int i, TagIOItem item);

public:
    QList<TagIOItem> m_tagIOItems;
};


//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////


class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ComboBoxDelegate( QStringList list, QObject *parent = nullptr);

    QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
    void setEditorData( QWidget *editor, const QModelIndex &index ) const;
    void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
    void updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
private:
    QStringList m_strItems;
};


//////////////////////////////////////////////////////////


class VariableTableView : public QTableView
{
    Q_OBJECT
public:
    explicit VariableTableView(QWidget *parent = nullptr);
    ~VariableTableView();

private:

protected:
    void mousePressEvent(QMouseEvent *event);

signals:

public slots:

private:


private:


};

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////


namespace Ui {
class VariableManagerWin;
}

class VariableManagerWin : public ChildBase
{
    Q_OBJECT

public:
    explicit VariableManagerWin(QWidget *parent = nullptr,
                                const QString &itemName = "",
                                const QString &projName = "");
    ~VariableManagerWin();

    void init(const QString &itemName = "");

public:
    // 打开文件
    void open();
    // 保存文件
    void save();
    // 显示大图标
    void showLargeIcon();
    // 显示小图标
    void showSmallIcon();

public:
    bool loadFromFile(SaveFormat saveFormat, const QString &it);
    bool saveToFile(SaveFormat saveFormat, const QString &it);
    void exportToCsv(const QString &path);
    void importFromCsv(const QString &path);
    void SetTitle(const QString &t);

private:
    void load(const QJsonObject &json, const QString &it);
    void save(QJsonObject &json, const QString &it);

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void closeEvent(QCloseEvent *event);  // 关闭事件

public slots:
    void tableViewVariableDoubleClicked(const QModelIndex &index);
    void VariableAdd();
    void VariableAppend();
    void VariableRowCopy();
    void VariableColCopy();
    void VariableModify();
    void VariableDelete();

public:
    QString m_IOVariableListWhat;

private:
    Ui::VariableManagerWin *ui; 
    VariableTableView *m_variableTableView;
    TagTmpTableModel* pTagTmpTableModel;
    TagSystemTableModel *pTagSystemTableModel;
    TagIOTableModel *pTagIOTableModel;
    QString m_strCurTagType;
};

#endif // VARIABLEMANAGERWIN_H
