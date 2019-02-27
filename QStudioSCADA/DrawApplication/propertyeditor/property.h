#ifndef PROPERTY_H
#define PROPERTY_H

#include <QVariant>
#include <QString>
#include <QStyleOptionViewItem>
#include <QAbstractItemDelegate>

class Property
{
public:
    enum ColumnHeader {
        ColumnProperty = 0,
        ColumnData = 1
    };

    Property(const QString &);
    virtual ~Property();

    virtual QVariant data(int column = ColumnProperty,int role = Qt::DisplayRole);
    virtual bool setData(const QVariant &data,int role = Qt::EditRole);
    virtual QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);
    virtual bool setEditorData(QWidget* editor);
    virtual QVariant getEditorData(QWidget* editor) const;
    virtual Qt::ItemFlags flags(int column = ColumnProperty);
    virtual void setValue(const QVariant& value);
    virtual QVariant getValue() const;
    virtual void setName(const QString& name);
    virtual QString getName() const;
    virtual bool paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index, const QAbstractItemDelegate* delegate) const;
    virtual void setId(uint);
    virtual uint getId() const;

protected:
    QVariant value;
    QString name;
    uint id;
};

#endif // PROPERTY_H
