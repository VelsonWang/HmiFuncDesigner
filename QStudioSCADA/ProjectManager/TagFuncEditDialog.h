#ifndef TAGFUNCEDITDIALOG_H
#define TAGFUNCEDITDIALOG_H

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "SplitterForm.h"

#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QSpacerItem>
#include <QTableView>
#include <QTextEdit>
#include <QToolButton>
#include <QTreeView>
#include <QVBoxLayout>
#include <QtProperty>
#include <QMap>
#include <QStringList>
#include <QList>
#include <QStandardItemModel>
#include <QAbstractTableModel>
#include <QVariant>
#include <QWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QDebug>

namespace Ui {
class TagFuncEditDialog;
}
typedef struct
{
    QString arg;
    QString type;
} TArgItem;

class FuncObjectItem
{
public:
    FuncObjectItem()
    {
        name = "";
        event = "";
        argList.clear();
    }

    QString GetFuncString()
    {
        QString ret = name;
        ret += "(";
        int count = argList.count();
        for(int i=0; i<count; i++)
        {
            ret += argList.at(i).arg;
            if(count>1 && i<argList.size()-1)
                ret += ",";
        }
        ret += ")";
        return ret;
    }

    void ShowInfo()
    {
        qDebug() << "\n[";
        qDebug() << name;
        for(int i=0; i<argList.count();i++)
        {
            TArgItem it = argList.at(i);
            qDebug() << it.arg << " " << it.type;
        }
        qDebug() << event;
        qDebug() << "]\n";
    }

public:
    QString name;
    QList<TArgItem> argList;
    QString event;
};

typedef FuncObjectItem TFuncObjectItem;
typedef FuncObjectItem* PFuncObjectItem;

//////////////////////////////////////////////////////////////////

class FuncModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        FUNC_NAME,
        EVENT_TYPE
    };

public:
    explicit FuncModel(QObject *parent = 0);
    ~FuncModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    bool setHeaderData(int, Qt::Orientation, const QVariant&, int=Qt::EditRole) { return false; }

    bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());

    void save(QString &data);
    void load(QString data);

public:
    void AppendRow(PFuncObjectItem item);
    void InsertRow(int i, PFuncObjectItem item);
    PFuncObjectItem GetRow(int i);
    void UpdateRow(int i, PFuncObjectItem item);
    void UpRow(int row);
    void DownRow(int row);

public:
    QList<PFuncObjectItem> m_FuncList;
};


//////////////////////////////////////////////////////////////////


class FuncTableView : public QTableView
{
    Q_OBJECT
public:
    explicit FuncTableView(QWidget *parent = 0);
    ~FuncTableView();

private:

protected:
    void mousePressEvent(QMouseEvent *event);

signals:

public slots:

private:


private:


};

///////////////////////////////////////////////////////////////////


class TagFuncEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagFuncEditDialog(QString projectPath, QWidget *parent = 0);
    ~TagFuncEditDialog();
    void SetData(QString data);
    QString GetData();

protected:
    void resizeEvent(QResizeEvent *e);

public:
    typedef struct FuncObject
    {
        FuncObject()
        {
            name = "";
            desc = "";
            args_type = "";
        }
        QString name;
        QString desc;
        QString args_type;
    }TFuncObject;
private:
    void TreeViewInit();
    bool GetFuncNameArgs(QString funcString, QString &funcName, QList<TArgItem> &argList);
    void GetElementProperty(PFuncObjectItem pFuncObj);
    void UpdateExpandState();
    void AddProperty(QtVariantProperty *property, const QString &id);

private slots:
    void FuncPropertyValueChanged(QtProperty *property, const QVariant &value);
    void FuncTableViewClicked(const QModelIndex &index);
    void on_treeViewFunc_doubleClicked(const QModelIndex &index);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_treeViewFunc_clicked(const QModelIndex &index);
    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnUp_clicked();
    void on_btnDown_clicked();

private:
    QTreeView *m_pTreeViewFunc;
    QTextEdit *m_pTextDesc;
    QStandardItemModel *m_pTreeViewItemModel;
    FuncModel *m_pfuncItemModel;
    QList<TFuncObject *> m_supportFuncList;
    FuncTableView *m_funcTableView;
    QStringList m_groupFuncNameList;
    SplitterForm *m_pSplitterMain;

    QtVariantPropertyManager *m_pVariantManager;
    QtTreePropertyBrowser *m_pPropertyEditor;
    QtVariantEditorFactory *m_pVariantFactory;
    QMap<QtProperty *, QString> m_mapPropertyToId;
    QMap<QString, QtVariantProperty *> m_mapIdToProperty;
    QMap<QString, bool> m_mapIdToExpanded;
    PFuncObjectItem m_pCurFuncObj;
    QStringList m_eventTypes;
    QStringList m_varsList;
    QString m_strProjectPath;
};

#endif // TAGFUNCEDITDIALOG_H
