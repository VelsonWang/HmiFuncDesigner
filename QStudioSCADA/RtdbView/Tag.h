#ifndef TAG_H
#define TAG_H

#include <QObject>
#include <QAbstractTableModel>
#include <QMap>
#include "../Public/Public.h"


//////////////////////////////////////////////////////////

struct TagItem
{
    explicit TagItem(const QString &sVarid=QString(),
            const QString &sName=QString(),
            const QString &sDescription=QString(),
            const QString &sDeviceInfo=QString(),
            const QString &sLogicValue=QString(),
            const QString &sStatus=QString())
        : mId(sVarid),
          mName(sName),
          mDescription(sDescription),
          mDeviceInfo(sDeviceInfo),
          mLogicValue(sLogicValue),
          mStatus(sStatus)
    {

    }

    QString mId; // 变量ID
    QString mName;  //名称
    QString mDescription;  //描述
    QString mDeviceInfo;  //设备信息
    QString mLogicValue;  //逻辑值
    QString mStatus;  //状态 报警
};

//////////////////////////////////////////////////////////


class TagTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column {TagID, Name, Description, DeviceInfo, LogicValue, Status};

public:
    explicit TagTableModel(QObject *parent=nullptr)
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

    void load(QList<TagItem *> pItems, int start, int end);
    void ClearItems();

public:
    void AppendRow(TagItem * item);
    void InsertRow(int i, TagItem * item);
    TagItem *GetRow(int i);
    void UpdateRow(int i, TagItem * item);
    void UpdateLogicValueAndStatus(QString id, QString logicValue, QString status);

private:
    QList<TagItem *> m_tagItems;
};



#endif // TAG_H
