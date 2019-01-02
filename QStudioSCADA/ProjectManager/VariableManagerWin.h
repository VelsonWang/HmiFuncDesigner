#ifndef VARIABLEMANAGERWIN_H
#define VARIABLEMANAGERWIN_H

#include "ChildBase.h"
#include "TagFuncEditDialog.h"
#include "../Public/Public.h"
#include <QList>
#include <QString>
#include <QWidget>
#include <QStandardItemModel>
#include <QJsonObject>
#include <QItemDelegate>
#include <QStringList>
#include <QTableView>
#include <QVBoxLayout>



#define SYSVARIABLE_BASE    (0)
#define TMPVARIABLE_BASE    (0)
#define IOVARIABLE_BASE     (0)


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
          m_sUnit(sUnit),m_sProjectConverter(sProjectConverter),m_sComments(sComments)
    {

    }

    int m_TagID; // 变量ID
    TTag m_iTagType;  // 变量类型
    QString m_sName;  //名称
    QString m_sDescription;  //描述
    QString m_sUnit;  //单位
    QString m_sProjectConverter;  //工程转换
    QString m_sComments;  //备注
};


//////////////////////////////////////////////////////////


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

    void load(const QJsonObject &json);
    void save(QJsonObject &json);

public:
    QList<TagSysItem> m_tagSysItems;
};



//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

struct TagTmpItem
{
    explicit TagTmpItem(int varid=0,
            TTag tagtype=TAG_TMP,
            const QString &sDataType=QString(),
            const QString &sName=QString(),
            const QString &sDescription=QString(),
            const QString &sUnit=QString(),
            const QString &sActionScope=QString(),
            const QString &sDataAttribute=QString(),
            const QString &sAlarm=QString(),
            const QString &sArchiveFile=QString(),
            const QString &sProjectConverter=QString(),
            const QString &sComments=QString())
        : m_TagID(varid),m_iTagType(tagtype), m_sDataType(sDataType),
          m_sName(sName), m_sDescription(sDescription),m_sUnit(sUnit),
          m_sActionScope(sActionScope), m_sDataAttribute(sDataAttribute),m_sAlarm(sAlarm),
          m_sArchiveFile(sArchiveFile),m_sProjectConverter(sProjectConverter),m_sComments(sComments)
    {
        m_iTagType = TAG_TMP;
    }

    int m_TagID; // 变量ID
    QString m_sDataType;  //数据类型
    TTag m_iTagType;  // 变量类型
    QString m_sName;  //名称
    QString m_sDescription;  //描述
    QString m_sUnit;  //单位
    QString m_sActionScope;  //作用域
    QString m_sDataAttribute;  //数据属性
    QString m_sAlarm;  //报警
    QString m_sArchiveFile;  //存盘
    QString m_sProjectConverter;  //工程转换
    QString m_sComments;  //备注
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
    explicit TagTmpTableModel(QObject *parent=0)
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
//////////////////////////////////////////////////////////

struct TagIOItem
{
    explicit TagIOItem(int varid=0,
            TTag tagtype=TAG_IO,
            const QString &sDataType=QString(),
            const QString &sName=QString(),
            const QString &sDescription=QString(),
            const QString &sUnit=QString(),
            const QString &sIOConnect=QString(),
            const QString &sAlarm=QString(),
            const QString &sArchiveFile=QString(),
            const QString &sProjectConverter=QString(),
            const QString &sComments=QString())
        : m_TagID(varid),m_iTagType(tagtype), m_sDataType(sDataType),
          m_sName(sName), m_sDescription(sDescription),m_sUnit(sUnit),
          m_sIOConnect(sIOConnect),m_sAlarm(sAlarm),
          m_sArchiveFile(sArchiveFile),m_sProjectConverter(sProjectConverter),m_sComments(sComments)
    {
        m_iTagType = TAG_IO;
    }

    int m_TagID; // 变量ID
    QString m_sDataType;  //数据类型
    TTag m_iTagType;  // 变量类型
    QString m_sName;  //名称
    QString m_sDescription;  //描述
    QString m_sUnit;  //单位
    QString m_sIOConnect;  //IO连接
    QString m_sAlarm;  //报警
    QString m_sArchiveFile;  //存盘
    QString m_sProjectConverter;  //工程转换
    QString m_sComments;  //备注
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
    explicit TagIOTableModel(QObject *parent=0)
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
    ComboBoxDelegate( QStringList list, QObject *parent = 0 );

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
    explicit VariableTableView(QWidget *parent = 0);
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
    explicit VariableManagerWin(QWidget *parent = 0,
                                const QString &itemName = "",
                                const QString &projName = "");
    ~VariableManagerWin();

    void init(const QString &itemName = "");
    //获取工程所有变量的名称
    static void GetAllProjectVariableName(const QString &proj_path,
                                          QStringList &varList,
                                          const QString &type = "ALL");


public:
    // 打开文件
    void open();
    // 保存文件
    void save();
    // 显示大图标
    void ShowLargeIcon();
    // 显示小图标
    void ShowSmallIcon();

public:
    bool loadFromFile(SaveFormat saveFormat, const QString &it);
    bool saveToFile(SaveFormat saveFormat, const QString &it);
    void exportToCsv(const QString &path, const QString &group);
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
