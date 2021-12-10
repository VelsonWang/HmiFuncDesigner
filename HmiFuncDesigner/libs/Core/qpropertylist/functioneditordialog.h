#ifndef FUNCTIONEDITORDIALOG_H
#define FUNCTIONEDITORDIALOG_H

#include <QDialog>
#include <QList>
#include <QMap>
#include <QDebug>
#include "../shared/projdata/deviceinfo.h"
#include "../shared/property/qabstractproperty.h"
#include "./qpropertylist/qpropertylistview.h"

class QListWidget;
class QListWidgetItem;

namespace Ui
{
class FunctionEditorDialog;
}

typedef struct ArgItem {
    ArgItem()
    {
        type = "";
        name = "";
        value = "";
    }

    ArgItem(const ArgItem &rh)
    {
        type = rh.type;
        name = rh.name;
        value = rh.value;
    }

    ArgItem &operator=(const ArgItem &rh)
    {
        type = rh.type;
        name = rh.name;
        value = rh.value;
        return *this;
    }

    QString type;
    QString name;
    QString value;
} TArgItem;

class FuncObjectItem
{
public:
    FuncObjectItem()
    {
        m_name = "";
        m_event = "";
        m_argList.clear();
        m_desc = "";
        m_funcNameOrg = "";
    }

    FuncObjectItem(const FuncObjectItem &rh)
    {
        m_name = rh.m_name;
        m_event = rh.m_event;
        m_argList = rh.m_argList;
        m_desc = rh.m_desc;
        m_funcNameOrg = rh.m_funcNameOrg;
    }

    FuncObjectItem &operator=(const FuncObjectItem &rh)
    {
        m_name = rh.m_name;
        m_event = rh.m_event;
        m_argList = rh.m_argList;
        m_desc = rh.m_desc;
        m_funcNameOrg = rh.m_funcNameOrg;
        return *this;
    }

    QString getFuncString()
    {
        QString ret = m_name;
        ret += "(";
        int count = m_argList.count();
        for(int i = 0; i < count; i++) {
            if(m_argList.at(i)->type == "GRAPHPAGELIST" ||
                    m_argList.at(i)->type == "TAGLIST" ||
                    m_argList.at(i)->type == "ELEMENTIDLIST") {
                ret += "\"";
            }
            ret += m_argList.at(i)->value;
            if(m_argList.at(i)->type == "GRAPHPAGELIST" ||
                    m_argList.at(i)->type == "TAGLIST" ||
                    m_argList.at(i)->type == "ELEMENTIDLIST") {
                ret += "\"";
            }
            ret += ",";
        }
        if(count > 0) {
            ret.chop(1);
        }
        ret += ")";
        return ret;
    }

    void setFuncString(const QString &szFuncInfo)
    {
        if(szFuncInfo == "") {
            return;
        }
        m_argList.clear();
        QStringList szObjList = szFuncInfo.split(':');
        if(szObjList.size() == 2) {
            m_event = szObjList.at(1);
            QString szFunc = szObjList.at(0);
            m_name = szFunc.left(szFunc.indexOf("("));
            QString szArgs = (szFunc.mid(szFunc.indexOf("(") + 1, szFunc.indexOf(")") - szFunc.indexOf("(") - 1)).trimmed();

            if(szArgs.length() > 0) {
                QStringList szList;
                szList = szArgs.split(',');
                for(int i = 0; i < szList.count(); i++) {
                    TArgItem *pArgItem = new TArgItem();
                    QString szValue = szList.at(i);
                    pArgItem->value = szValue.replace("\"", "");
                    m_argList.append(pArgItem);
                }
            }
        }
    }

    void showInfo()
    {
        qDebug() << "\n[";
        qDebug() << m_name;
        for(int i = 0; i < m_argList.count(); i++) {
            TArgItem *pArgItem = m_argList.at(i);
            qDebug() << pArgItem->name << " " << pArgItem->type << " " << pArgItem->value;
        }
        qDebug() << m_event;
        qDebug() << m_funcNameOrg;
        qDebug() << m_desc;
        qDebug() << "]\n";
    }

public:
    QString m_name;
    QList<TArgItem *> m_argList;
    QString m_event;

    QString m_desc;
    QString m_funcNameOrg;
};

typedef FuncObjectItem TFuncObjectItem;
typedef FuncObjectItem* PFuncObjectItem;

//////////////////////////////////////////////////////////////////

class FunctionEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FunctionEditorDialog(QWidget *parent = NULL, QStringList events = QStringList());
    ~FunctionEditorDialog();
    QStringList getFunctions();
    void setFunctions(const QStringList &funcs);
    void setSupportEvents(QStringList events);

private:
    void initUI();
    void initListWidget();
    void initTableWidget();
    TFuncObjectItem *getFuncObjectItem(const QString &name);
    void updatePropertyEditor();

public slots:
    void onPropertyEdit(QAbstractProperty *pro, const QVariant &value);

private slots:
    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnMoveUp_clicked();
    void on_btnMoveDown_clicked();
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void listItemClicked(QListWidgetItem *item);
    void listItemDoubleClicked(QListWidgetItem *item);
    void on_tableEventFunc_clicked(const QModelIndex &index);

private:
    Ui::FunctionEditorDialog *ui;
    QString m_selectedFuncName;
    int m_selectedCurRow;
    QList<TFuncObjectItem *> m_funcObjItemList;
    QList<TFuncObjectItem *> m_selectFuncObjItemList;
    QList<QListWidget *> m_listWidgetList;
    QStringList m_funcs;
    QStringList m_supportEvents;
    QMap<QString, QString> m_nameToShowName;
    QMap<QString, QString> m_showNameToName;
    QStringList m_tagNames;
    QStringList m_elementIds;
    QStringList m_graphPageNames;

private:
    QPropertyListView *m_propertyView;
};

#endif // FUNCTIONEDITORDIALOG_H
