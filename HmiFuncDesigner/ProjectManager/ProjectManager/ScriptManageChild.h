#ifndef ScriptManageChild_H
#define ScriptManageChild_H

#include "ChildInterface.h"
#include "../../Public/Public.h"
#include <QWidget>
#include <QListWidget>
#include <QList>
#include <QVBoxLayout>
#include <QStringList>
#include <QJsonObject>


class ScriptObject
{
public:
    ScriptObject();
    ~ScriptObject();
    void load(QJsonObject &json);
    void save(QJsonObject &json);

public:
    QString m_strName;
    bool m_bInUse;
    QString m_strDescription;
    QString m_strRunMode;
    QString m_strRunModeArgs;
};

class ScriptFileManage
{
public:
    static void AddScriptInfo(ScriptObject * obj);
    static void ModifyScriptInfo(ScriptObject * oldobj, ScriptObject * newobj);
    static void DeleteScriptInfo(ScriptObject * obj);
    static ScriptObject *GetScriptObject(const QString &name);
    static void load(const QString &filename=QString(), SaveFormat saveFormat=Json);
    static void save(const QString &filename=QString(), SaveFormat saveFormat=Json);

public:
    static QList<ScriptObject *> m_listScriptInfo;
};

class ScriptManageChild : public QWidget, public ChildInterface
{
    Q_OBJECT
    Q_INTERFACES(ChildInterface)
public:
    explicit ScriptManageChild(QWidget *parent = Q_NULLPTR);
    ~ScriptManageChild();
    void init();


public:
    void buildUserInterface(QMainWindow* pMainWin);
    void removeUserInterface(QMainWindow* pMainWin);
    QString userFriendlyCurrentFile();
    QString currentFile() const;
    QString wndTitle() const;

private:
    bool open();
    bool save();

public slots:
    void ListWidgetClicked(QListWidgetItem *item);
    void NewScript();
    void ModifyScript();
    void DeleteScript();

protected:
    void contextMenuEvent(QContextMenuEvent * event);

private:
    QListWidget *m_pListWidgetObj;
    QVBoxLayout *m_pVLayoutObj;
};


#endif // ScriptManageChild_H
