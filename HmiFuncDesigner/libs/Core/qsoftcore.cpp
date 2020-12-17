#include "qsoftcore.h"

#include "qabstractpagewidget.h"
#include "qdevicesmanager.h"
#include "qfilemanager.h"

#include "../shared/qprojectcore.h"
#include "../shared/qabstractupdateplugin.h"
#include "../shared/pluginloader.h"

#include <QIcon>
#include <QApplication>
#include <QDir>

QSoftCore* QSoftCore::m_core=Q_NULLPTR;

QSoftCore::QSoftCore(QObject *parent) :
    QObject(parent),
    m_project_core(new QProjectCore),
    m_undo_group(new QUndoGroup(this)),
    m_devicesManager(new QDevicesManager(this)),
    m_file_manager(new QFileManager)
{
    QDir dir;
    dir.mkpath(QApplication::applicationDirPath()+"/resources");
    m_file_manager->load();

    initUserManagerAction();
    initLanguageManagerAction();
    initFormManagerAction();
    initProjectAction();
    initResourceAction();
    initImageViewAction();
    initScriptEditAction();
    initDataAction();
    initRunningAction();
    initDeviceAction();
    initUndoAction();
    initDriverAction();
}

void QSoftCore::insertAction(const QString &key, QAction *action)
{
    QStringList l=key.split(".");
    l.removeAll("");
    if(l.size()==0)
    {
        return;
    }


    QActionArray *group=&m_actions;
    QActionArray *temp;

    foreach(const QString &str,l)
    {
        temp=group->m_children.value(str);
        if(temp==Q_NULLPTR)
        {
            temp=new QActionArray;
            group->m_children.insert(str,temp);
        }
        group=temp;
    }
    if(group->m_action!=Q_NULLPTR)
    {
        delete group->m_action;
    }
    group->m_action=action;
}

QAction* QSoftCore::getAction(const QString &key)
{
    QStringList l=key.split(".");
    l.removeAll("");
    if(l.size()==0)
    {
        return Q_NULLPTR;
    }

    QActionArray *group=&m_actions;
    QActionArray *temp;

    foreach(const QString &str,l)
    {
        temp=group->m_children.value(str);
        if(temp==Q_NULLPTR)
        {
            return Q_NULLPTR;
        }
        group=temp;
    }
    return group->m_action;
}

QSoftCore* QSoftCore::getCore()
{
    if(m_core==Q_NULLPTR)
    {
        m_core=new QSoftCore;
    }
    return m_core;
}

void QSoftCore::releaseCore()
{
    if(m_core!=Q_NULLPTR)
    {
        delete m_core;
        m_core=Q_NULLPTR;
    }
}

void QSoftCore::initUserManagerAction()
{
    QAction *ac;

    ac=new QAction(QIcon(":/images/plus.png"),tr("Add User"),Q_NULLPTR);
    insertAction("UserManager.Add",ac);
    ac=new QAction(QIcon(":/images/minus.png"),tr("Remove User"),Q_NULLPTR);
    insertAction("UserManager.Del",ac);
    ac=new QAction(QIcon(""),tr("Remove users"),Q_NULLPTR);
    insertAction("UserManager.Remove",ac);
    ac=new QAction(QIcon(""),tr("Give up"),Q_NULLPTR);
    insertAction("UserManager.Give-Up",ac);
}

void QSoftCore::initLanguageManagerAction()
{
    QAction *ac;

    ac=new QAction(QIcon(":/images/plus.png"),tr("Add Language"),Q_NULLPTR);
    insertAction("LanguageManager.Add",ac);
    ac=new QAction(QIcon(":/images/minus.png"),tr("Remove Language"),Q_NULLPTR);
    insertAction("LanguageManager.Del",ac);
    ac=new QAction(QIcon(""),tr("Remove users"),Q_NULLPTR);
    insertAction("LanguageManager.Remove",ac);
    ac=new QAction(QIcon(""),tr("Give up"),Q_NULLPTR);
    insertAction("LanguageManager.Give-Up",ac);
}

void QSoftCore::initFormManagerAction()
{
    QAction *ac;
    ac=new QAction(QIcon(":/images/plus.png"),tr("Add Form"),Q_NULLPTR);
    insertAction("FormManager.Add",ac);
    ac=new QAction(QIcon(":/images/minus.png"),tr("Remove Form"),Q_NULLPTR);
    insertAction("FormManager.Del",ac);
    ac=new QAction(QIcon(":/images/left.png"),tr("Left"),Q_NULLPTR);
    insertAction("FormManager.Same.Left",ac);
    ac=new QAction(QIcon(":/images/v-center.png"),tr("V-Center"),Q_NULLPTR);
    insertAction("FormManager.Same.V-Center",ac);
    ac=new QAction(QIcon(":/images/right.png"),tr("Right"),Q_NULLPTR);
    insertAction("FormManager.Same.Right",ac);
    ac=new QAction(QIcon(":/images/top.png"),tr("Top"),Q_NULLPTR);
    insertAction("FormManager.Same.Top",ac);
    ac=new QAction(QIcon(":/images/h-center.png"),tr("H-Center"),Q_NULLPTR);
    insertAction("FormManager.Same.H-Center",ac);
    ac=new QAction(QIcon(":/images/bottom.png"),tr("Bottom"),Q_NULLPTR);
    insertAction("FormManager.Same.Bottom",ac);
    ac=new QAction(QIcon(":/images/same-width.png"),tr("Same Width"),Q_NULLPTR);
    insertAction("FormManager.Same.Width",ac);
    ac=new QAction(QIcon(":/images/same-height.png"),tr("Same Height"),Q_NULLPTR);
    insertAction("FormManager.Same.Height",ac);
    ac=new QAction(QIcon(":/images/same-rect.png"),tr("Same Gemotry"),Q_NULLPTR);
    insertAction("FormManager.Same.Gemotry",ac);
    ac=new QAction(QIcon(":/images/editcopy.png"),tr("Copy"),Q_NULLPTR);
    insertAction("FormManager.Copy",ac);
    ac=new QAction(QIcon(":/images/editcut.png"),tr("Cut"),Q_NULLPTR);
    insertAction("FormManager.Cut",ac);
    ac=new QAction(QIcon(":/images/editpaste.png"),tr("Paste"),Q_NULLPTR);
    insertAction("FormManager.Paste",ac);
    ac=new QAction(QIcon(),tr("Delete"),Q_NULLPTR);
    insertAction("FormManager.Delete",ac);
    ac=new QAction(QIcon(),tr("Select All"),Q_NULLPTR);
    insertAction("FormManager.Select.All",ac);
}

void QSoftCore::initProjectAction()
{
    QAction *ac;
    ac=new QAction(QIcon(":/images/fileopen.png"),tr("Open Project"),Q_NULLPTR);
    insertAction("Project.Open",ac);
    ac=new QAction(QIcon(":/images/filesave.png"),tr("Save Project"),Q_NULLPTR);
    insertAction("Project.Save",ac);
    ac=new QAction(QIcon(":/images/filenew.png"),tr("New Project"),Q_NULLPTR);
    insertAction("Project.New",ac);
    ac=new QAction(QIcon(":/images/close.png"),tr("Close Project"),Q_NULLPTR);
    insertAction("Project.Close",ac);
}

void QSoftCore::initResourceAction()
{
    QAction *ac;

    ac=new QAction(QIcon(":/images/plus.png"),tr("Add Resource"),Q_NULLPTR);
    insertAction("ResourceManager.Add",ac);
    ac=new QAction(QIcon(":/images/minus.png"),tr("Remove Resource"),Q_NULLPTR);
    insertAction("ResourceManager.Del",ac);
}

void QSoftCore::initImageViewAction()
{
    QAction *ac;

    ac=new QAction(QIcon(":/images/fitinscreen.png"),tr("Fit image int the screen"),Q_NULLPTR);
    insertAction("ImageView.Fit",ac);
    ac=new QAction(QIcon(":/images/originalsize.png"),tr("Original size"),Q_NULLPTR);
    insertAction("ImageView.Original",ac);
    ac=new QAction(QIcon(":/images/zoomin.png"),tr("Zoomin"),Q_NULLPTR);
    insertAction("ImageView.Zoomin",ac);
    ac=new QAction(QIcon(":/images/zoomout.png"),tr("Zoomout"),Q_NULLPTR);
    insertAction("ImageView.Zoomout",ac);
}

void QSoftCore::initScriptEditAction()
{
    QAction *ac;
    ac=new QAction(QIcon(":/images/filesave_small.png"),tr("&Save"),Q_NULLPTR);
    insertAction("ScriptEdit.Save",ac);
    ac=new QAction(QIcon(":/images/undo.png"),tr("&Undo"),Q_NULLPTR);
    insertAction("ScriptEdit.Undo",ac);
    ac=new QAction(QIcon(":/images/redo.png"),tr("&Redo"),Q_NULLPTR);
    insertAction("ScriptEdit.Redo",ac);
    ac=new QAction(QIcon(":/images/editcut.png"),tr("Cu&t"),Q_NULLPTR);
    insertAction("ScriptEdit.Cut",ac);
    ac=new QAction(QIcon(":/images/editcopy.png"),tr("&Copy"),Q_NULLPTR);
    insertAction("ScriptEdit.Copy",ac);
    ac=new QAction(QIcon(":/images/editpaste.png"),tr("&Paste"),Q_NULLPTR);
    insertAction("ScriptEdit.Paste",ac);
    ac=new QAction(QIcon(":/images/zoomin.png"),tr("Zoomin"),Q_NULLPTR);
    insertAction("ScriptEdit.Zoomin",ac);
    ac=new QAction(QIcon(":/images/zoomout.png"),tr("Zoomout"),Q_NULLPTR);
    insertAction("ScriptEdit.Zoomout",ac);
    ac=new QAction(QIcon(":/images/font.png"),tr("Set Font"),Q_NULLPTR);
    insertAction("ScriptEdit.Font",ac);
    ac=new QAction(QIcon(":/images/fitinscreen.png"),tr("Search"),Q_NULLPTR);
    insertAction("ScriptEdit.Search",ac);
}

void QSoftCore::initDataAction()
{
    QAction *ac;

    ac=new QAction(QIcon(":/images/plus.png"),tr("Add Group"),Q_NULLPTR);
    insertAction("Data.Group.Add",ac);
    ac=new QAction(QIcon(":/images/minus.png"),tr("Remove Group"),Q_NULLPTR);
    insertAction("Data.Group.Del",ac);
}

void QSoftCore::initDriverAction()
{
    QAction *ac;

    ac=new QAction(QIcon(":/images/plus.png"),tr("Add Driver"),Q_NULLPTR);
    insertAction("Driver.Add",ac);
    ac=new QAction(QIcon(":/images/minus.png"),tr("Remove Driver"),Q_NULLPTR);
    insertAction("Driver.Del",ac);
}

void QSoftCore::initRunningAction()
{
    QAction *ac;
    ac=new QAction(QIcon(":/images/run.png"),tr("Run"),Q_NULLPTR);
    insertAction("Debug.Run",ac);
    ac=new QAction(QIcon(":/images/stop.png"),tr("Stop"),Q_NULLPTR);
    insertAction("Debug.Stop",ac);
}

void QSoftCore::initDeviceAction()
{
    QAction* ac;
    ac=new QAction(QIcon(":/images/device.png"),tr("Update"),Q_NULLPTR);
    insertAction("Device.Update",ac);
    ac=new QAction(QIcon(":/images/sync.png"),tr("Sync Data"),Q_NULLPTR);
    insertAction("Device.Sync",ac);
}

void QSoftCore::initUndoAction()
{
    QAction* ac;
    ac=m_undo_group->createRedoAction(Q_NULLPTR);
    ac->setIcon(QIcon(":/images/redo.png"));
    ac->setShortcut(QKeySequence::Redo);
    insertAction("Undo.Redo",ac);
    ac=m_undo_group->createUndoAction(Q_NULLPTR);
    ac->setIcon(QIcon(":/images/undo.png"));
    ac->setShortcut(QKeySequence::Undo);
    insertAction("Undo.Undo",ac);
}

QProjectCore* QSoftCore::getProjectCore()
{
    return m_project_core;
}

void QSoftCore::addUndoStack(QUndoStack *stack)
{
    m_undo_group->addStack(stack);
}

void QSoftCore::setActivityStack(QUndoStack *stack)
{
    m_undo_group->setActiveStack(stack);
}

void QSoftCore::startUpdatePlugin()
{
    QMap<QString,QAbstractPlugin*> plugins=PluginLoader::getPluginByType(UPDATE_PLUGIN_TYPE);

    QMapIterator<QString,QAbstractPlugin*> it(plugins);

    while(it.hasNext())
    {
        it.next();
        QAbstractUpdatePlugin *pl=(QAbstractUpdatePlugin*)it.value();
        QStringList list=pl->supports();

        foreach(const QString &n,list)
        {
            QAbstractUpdate *up=pl->create(n);
            if(up!=Q_NULLPTR)
            {
                m_devicesManager->addUpdate(up);
            }
        }
    }
}

QDevicesManager *QSoftCore::getDevicesManager()
{
    return m_devicesManager;
}

QFileManager* QSoftCore::getFileManager()
{
    return m_file_manager;
}



/**
 * @brief Core::insertToolBar
 * @details 添加全局工具条
 * @param szKey 工具条对象对应的ID
 * @param pToolBarObj 工具条对象
 */
void QSoftCore::insertToolBar(const QString& szKey, QToolBar *pToolBarObj) {
    m_mapToolBar.insert(szKey.toLower(), pToolBarObj);
}

/**
 * @brief Core::getToolBar
 * @details 获取全局工具条
 * @param szKey 工具条对象对应的ID
 * @return 工具条对象
 */
QToolBar* QSoftCore::getToolBar(const QString &szKey) {
    return m_mapToolBar.value(szKey.toLower());
}
