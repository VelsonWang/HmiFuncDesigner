#ifndef FUNCTIONEDITORDIALOG_H
#define FUNCTIONEDITORDIALOG_H

#include "propertytableview.h"
#include "propertymodel.h"
#include "property.h"
#include <QDialog>
#include <QList>
#include <QMap>

#include <QDebug>

class QListWidget;
class QListWidgetItem;

namespace Ui {
class FunctionEditorDialog;
}

typedef struct ArgItem
{
    ArgItem() {
        type = "";
        name = "";
        value = "";
    }

    ArgItem(const ArgItem &rh) {
        type = rh.type;
        name = rh.name;
        value = rh.value;
    }

    ArgItem &operator=(const ArgItem &rh) {
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
    FuncObjectItem() {
        szName_ = "";
        szEvent_ = "";
        argList_.clear();
        szDesc_ = "";
        szFuncNameOrg_ = "";
    }

    FuncObjectItem(const FuncObjectItem &rh) {
        szName_ = rh.szName_;
        szEvent_ = rh.szEvent_;
        argList_ = rh.argList_;
        szDesc_ = rh.szDesc_;
        szFuncNameOrg_ = rh.szFuncNameOrg_;
    }

    FuncObjectItem &operator=(const FuncObjectItem &rh) {
        szName_ = rh.szName_;
        szEvent_ = rh.szEvent_;
        argList_ = rh.argList_;
        szDesc_ = rh.szDesc_;
        szFuncNameOrg_ = rh.szFuncNameOrg_;
        return *this;
    }

    QString getFuncString() {
        QString ret = szName_;
        ret += "(";
        int count = argList_.count();
        for(int i=0; i<count; i++) {
            if(argList_.at(i)->type == "GRAPHPAGELIST" ||
                    argList_.at(i)->type == "TAGLIST" ||
                    argList_.at(i)->type == "ELEMENTIDLIST") {
                ret += "\"";
            }
            ret += argList_.at(i)->value;
            if(argList_.at(i)->type == "GRAPHPAGELIST" ||
                    argList_.at(i)->type == "TAGLIST" ||
                    argList_.at(i)->type == "ELEMENTIDLIST") {
                ret += "\"";
            }
            ret += ",";
        }
        if(count > 0)
            ret.chop(1);
        ret += ")";
        return ret;
    }

    void setFuncString(const QString &data) {
        if(data == "")
            return;
        argList_.clear();
        QStringList szObjList = data.split(':');
        if(szObjList.size() == 2) {
            szEvent_ = szObjList.at(1);
            QString szFunc = szObjList.at(0);
            szName_ = szFunc.left(szFunc.indexOf("("));
            QString szArgs = (szFunc.mid(szFunc.indexOf("(") + 1, szFunc.indexOf(")") - szFunc.indexOf("(") - 1)).trimmed();

            if(szArgs.length() > 0) {
                QStringList szList;
                szList = szArgs.split(',');
                for(int i=0; i<szList.count(); i++) {
                    TArgItem *pArgItem = new TArgItem();
                    QString szValue = szList.at(i);
                    pArgItem->value = szValue.replace("\"", "");
                    argList_.append(pArgItem);
                }
            }
        }
    }

    void showInfo() {
        qDebug() << "\n[";
        qDebug() << szName_;
        for(int i=0; i<argList_.count();i++) {
            TArgItem *pArgItem = argList_.at(i);
            qDebug() << pArgItem->name << " " << pArgItem->type << " " << pArgItem->value;
        }
        qDebug() << szEvent_;
        qDebug() << szFuncNameOrg_;
        qDebug() << szDesc_;
        qDebug() << "]\n";
    }

public:
    QString szName_;
    QList<TArgItem *> argList_;
    QString szEvent_;

    QString szDesc_;
    QString szFuncNameOrg_;
};

typedef FuncObjectItem TFuncObjectItem;
typedef FuncObjectItem* PFuncObjectItem;

//////////////////////////////////////////////////////////////////

class FunctionEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FunctionEditorDialog(QWidget *parent = Q_NULLPTR, QStringList events = QStringList());
    ~FunctionEditorDialog();
    QStringList getFunctions();
    void setFunctions(const QStringList &funcs);
    void setSupportEvents(QStringList events);

private:
    void initUI();
    void initListWidget();
    void initTableWidget();
    TFuncObjectItem *getFuncObjectItem(const QString &name);
    void clearProperty();
    void updatePropertyModel();
    void createPropertyList();
    QList<Property *> getPropertyList() const;

private slots:
    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnMoveUp_clicked();
    void on_btnMoveDown_clicked();
    void on_btnOk_clicked();
    void on_btnCancel_clicked();    
    void listItemClicked(QListWidgetItem *item);
    void listItemDoubleClicked(QListWidgetItem *item);
    void propertyChanged(Property *property);
    void on_tableEventFunc_clicked(const QModelIndex &index);

private:
    Ui::FunctionEditorDialog *ui;
    PropertyTableView *propertyView_;
    PropertyModel *propertyModel_;
    QString szSelectedFuncName_;
    int iSelectedCurRow_;
    QList<TFuncObjectItem *> funcObjItemList_;
    QList<TFuncObjectItem *> selectFuncObjItemList_;
    QList<QListWidget *> listWidgetList_;
    QStringList funcs_;
    QStringList supportEvents_;
    QList <Property *> propList_;
    QMap<QString, QString> mapNameToShowName_;
    QMap<QString, QString> mapShowNameToName_;
};

#endif // FUNCTIONEDITORDIALOG_H
