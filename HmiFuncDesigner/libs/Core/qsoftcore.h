#ifndef QSOFTCORE_H
#define QSOFTCORE_H

#include "corelibglobal.h"

#include <QObject>
#include <QString>
#include <QMap>
#include <QAction>
#include <QToolBar>
#include <QUndoGroup>
#include <QUndoStack>
#include <QUndoGroup>

class QAbstractPageWidget;
class QProjectCore;
class QDevicesManager;
class QFileManager;

class QActionArray
{
public:
    QActionArray():m_type(""),m_action(NULL){}
    ~QActionArray()
    {
        qDeleteAll(m_children.values());
        if(m_action!=NULL)delete m_action;
    }
    QString                     m_type;
    QMap<QString,QActionArray*> m_children;
    QAction*                    m_action;
};

class CORELIB_EXPORT QSoftCore : public QObject
{
    Q_OBJECT
public:
    static QSoftCore* getCore();
    static void releaseCore();

    QProjectCore* getProjectCore();

    void addUndoStack(QUndoStack* stack);
    void setActivityStack(QUndoStack* stack);

    void startUpdatePlugin();

    QDevicesManager* getDevicesManager();
    QFileManager* getFileManager();
protected:
    explicit QSoftCore(QObject *parent = 0);

    void    initUserManagerAction();
    void    initLanguageManagerAction();
    void    initFormManagerAction();
    void    initProjectAction();
    void    initResourceAction();
    void    initImageViewAction();
    void    initScriptEditAction();
    void    initDataAction();
    void    initRunningAction();
    void    initDeviceAction();
    void    initUndoAction();
    void    initDriverAction();
public:
    void insertAction(const QString& key,QAction *action);//添加全局action
    QAction* getAction(const QString &key);//获取全局action
    QStringList getPages();

    // 添加全局工具条
    void insertToolBar(const QString& szKey, QToolBar *pToolBarObj);
    // 获取全局工具条
    QToolBar* getToolBar(const QString &szKey);

protected:
    QActionArray m_actions; // 全局action
    QMap<QString, QToolBar*> m_mapToolBar; // 全局工具条
    static  QSoftCore* m_core; // 单实例
    QProjectCore* m_project_core;
    QUndoGroup* m_undo_group;
    QDevicesManager* m_devicesManager;
    QFileManager* m_file_manager;
};

#endif // QSOFTCORE_H
