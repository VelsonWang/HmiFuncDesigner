#ifndef SCRIPTMANAGEWIN_H
#define SCRIPTMANAGEWIN_H

#include "ChildBase.h"
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

class ScriptManageWin : public ChildBase
{
    Q_OBJECT
public:
    explicit ScriptManageWin(QWidget *parent = 0, QString itemName = "", QString projName = "");
    ~ScriptManageWin();
    void init();

public:
    // 打开文件
    void open();
    // 保存文件
    void save();
    // 显示大图标
    void showLargeIcon();
    // 显示小图标
    void showSmallIcon();

signals:

public slots:
    void ListWidgetClicked(QListWidgetItem *item);
    void NewScript();
    void ModifyScript();
    void DeleteScript();

protected:
    void contextMenuEvent(QContextMenuEvent * event);

private:
    QListWidget *m_pListWidget;
    QVBoxLayout *m_pVLayout;
};


#endif // SCRIPTMANAGEWIN_H
