#ifndef TABLEVIEWDELEGATE_H
#define TABLEVIEWDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QStringList>


class TableViewLineEditDelegate : public QItemDelegate
{
	Q_OBJECT
public:
    TableViewLineEditDelegate(QObject *parent = 0);

	//返回一个编辑控件，用来编辑指定项的数据
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	//将Model中数据赋值到控件上
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	//设定模型数据，根据指定项中对应编辑控件的数据
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	//更新编辑框几何形状
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setLineEditConditions(QString conditions);

private:
    QString m_szLineEditConditions = "";
};

///////////////////////////////////////////////////////////////////////////////



class TableViewComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TableViewComboBoxDelegate(const QStringList &texts, bool editable = false, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setItems(QStringList szList);

private:
    QStringList m_szItemTexts;
    bool m_bEditable;
};

///////////////////////////////////////////////////////////////////////////////



class TableViewSpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TableViewSpinBoxDelegate(int iMin = -32768, int iMax = 32767, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    int m_iMin = -32768;
    int m_iMax = 32767;
};



#endif // TABLEVIEWDELEGATE_H
