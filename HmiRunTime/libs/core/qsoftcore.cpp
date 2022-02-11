#include "qsoftcore.h"
#include "../shared/qprojectcore.h"
#include "../shared/qabstractupdateplugin.h"
#include "../shared/pluginloader.h"
#include <QIcon>
#include <QApplication>
#include <QDir>

QSoftCore* QSoftCore::m_core = NULL;

QSoftCore::QSoftCore(QObject *parent) :
    QObject(parent),
    m_projectCore(new QProjectCore),
    m_undoGroup(new QUndoGroup(this))
{
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
    QStringList l = key.split(".");
    l.removeAll("");
    if(l.size() == 0) {
        return;
    }

    QActionArray *group = &m_actions;
    QActionArray *temp;

    foreach(const QString &str, l) {
        temp = group->m_children.value(str);
        if(temp == NULL) {
            temp = new QActionArray;
            group->m_children.insert(str, temp);
        }
        group = temp;
    }
    if(group->m_action != NULL) {
        delete group->m_action;
    }
    group->m_action = action;
}

QAction* QSoftCore::getAction(const QString &key)
{
    QStringList l = key.split(".");
    l.removeAll("");
    if(l.size() == 0) {
        return NULL;
    }

    QActionArray *group = &m_actions;
    QActionArray *temp;

    foreach(const QString &str, l) {
        temp = group->m_children.value(str);
        if(temp == NULL) {
            return NULL;
        }
        group = temp;
    }
    return group->m_action;
}

QSoftCore* QSoftCore::getCore()
{
    if(m_core == NULL) {
        m_core = new QSoftCore;
    }
    return m_core;
}

void QSoftCore::releaseCore()
{
    if(m_core != NULL) {
        delete m_core;
        m_core = NULL;
    }
}

void QSoftCore::initUserManagerAction()
{
    QAction *ac;

    ac = new QAction(QIcon(":/images/plus.png"), tr("Add User"), NULL);
    insertAction("UserManager.Add", ac);
    ac = new QAction(QIcon(":/images/minus.png"), tr("Remove User"), NULL);
    insertAction("UserManager.Del", ac);
    ac = new QAction(QIcon(""), tr("Remove users"), NULL);
    insertAction("UserManager.Remove", ac);
    ac = new QAction(QIcon(""), tr("Give up"), NULL);
    insertAction("UserManager.Give-Up", ac);
}

void QSoftCore::initLanguageManagerAction()
{
    QAction *ac;

    ac = new QAction(QIcon(":/images/plus.png"), tr("Add Language"), NULL);
    insertAction("LanguageManager.Add", ac);
    ac = new QAction(QIcon(":/images/minus.png"), tr("Remove Language"), NULL);
    insertAction("LanguageManager.Del", ac);
    ac = new QAction(QIcon(""), tr("Remove users"), NULL);
    insertAction("LanguageManager.Remove", ac);
    ac = new QAction(QIcon(""), tr("Give up"), NULL);
    insertAction("LanguageManager.Give-Up", ac);
}

void QSoftCore::initFormManagerAction()
{
    QAction *ac;
    ac = new QAction(QIcon(":/images/plus.png"), tr("Add Form"), NULL);
    insertAction("FormManager.Add", ac);
    ac = new QAction(QIcon(":/images/minus.png"), tr("Remove Form"), NULL);
    insertAction("FormManager.Del", ac);
    ac = new QAction(QIcon(":/images/left.png"), tr("Left"), NULL);
    insertAction("FormManager.Same.Left", ac);
    ac = new QAction(QIcon(":/images/v-center.png"), tr("V-Center"), NULL);
    insertAction("FormManager.Same.V-Center", ac);
    ac = new QAction(QIcon(":/images/right.png"), tr("Right"), NULL);
    insertAction("FormManager.Same.Right", ac);
    ac = new QAction(QIcon(":/images/top.png"), tr("Top"), NULL);
    insertAction("FormManager.Same.Top", ac);
    ac = new QAction(QIcon(":/images/h-center.png"), tr("H-Center"), NULL);
    insertAction("FormManager.Same.H-Center", ac);
    ac = new QAction(QIcon(":/images/bottom.png"), tr("Bottom"), NULL);
    insertAction("FormManager.Same.Bottom", ac);
    ac = new QAction(QIcon(":/images/same-width.png"), tr("Same Width"), NULL);
    insertAction("FormManager.Same.Width", ac);
    ac  = new QAction(QIcon(":/images/same-height.png"), tr("Same Height"), NULL);
    insertAction("FormManager.Same.Height", ac);
    ac = new QAction(QIcon(":/images/same-rect.png"), tr("Same Gemotry"), NULL);
    insertAction("FormManager.Same.Gemotry", ac);
    ac = new QAction(QIcon(":/images/editcopy.png"), tr("Copy"), NULL);
    insertAction("FormManager.Copy", ac);
    ac = new QAction(QIcon(":/images/editcut.png"), tr("Cut"), NULL);
    insertAction("FormManager.Cut", ac);
    ac = new QAction(QIcon(":/images/editpaste.png"), tr("Paste"), NULL);
    insertAction("FormManager.Paste", ac);
    ac = new QAction(QIcon(), tr("Delete"), NULL);
    insertAction("FormManager.Delete", ac);
    ac = new QAction(QIcon(), tr("Select All"), NULL);
    insertAction("FormManager.Select.All", ac);
}

void QSoftCore::initProjectAction()
{
    QAction *ac;
    ac = new QAction(QIcon(":/images/fileopen.png"), tr("Open Project"), NULL);
    insertAction("Project.Open", ac);
    ac = new QAction(QIcon(":/images/filesave.png"), tr("Save Project"), NULL);
    insertAction("Project.Save", ac);
    ac = new QAction(QIcon(":/images/filenew.png"), tr("New Project"), NULL);
    insertAction("Project.New", ac);
    ac = new QAction(QIcon(":/images/close.png"), tr("Close Project"), NULL);
    insertAction("Project.Close", ac);
}

void QSoftCore::initResourceAction()
{
    QAction *ac;

    ac = new QAction(QIcon(":/images/plus.png"), tr("Add Resource"), NULL);
    insertAction("ResourceManager.Add", ac);
    ac = new QAction(QIcon(":/images/minus.png"), tr("Remove Resource"), NULL);
    insertAction("ResourceManager.Del", ac);
}

void QSoftCore::initImageViewAction()
{
    QAction *ac;

    ac = new QAction(QIcon(":/images/fitinscreen.png"), tr("Fit image int the screen"), NULL);
    insertAction("ImageView.Fit", ac);
    ac = new QAction(QIcon(":/images/originalsize.png"), tr("Original size"), NULL);
    insertAction("ImageView.Original", ac);
    ac = new QAction(QIcon(":/images/zoomin.png"), tr("Zoomin"), NULL);
    insertAction("ImageView.Zoomin", ac);
    ac = new QAction(QIcon(":/images/zoomout.png"), tr("Zoomout"), NULL);
    insertAction("ImageView.Zoomout", ac);
}

void QSoftCore::initScriptEditAction()
{
    QAction *ac;
    ac = new QAction(QIcon(":/images/filesave_small.png"), tr("&Save"), NULL);
    insertAction("ScriptEdit.Save", ac);
    ac = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), NULL);
    insertAction("ScriptEdit.Undo", ac);
    ac = new QAction(QIcon(":/images/redo.png"), tr("&Redo"), NULL);
    insertAction("ScriptEdit.Redo", ac);
    ac = new QAction(QIcon(":/images/editcut.png"), tr("Cu&t"), NULL);
    insertAction("ScriptEdit.Cut", ac);
    ac = new QAction(QIcon(":/images/editcopy.png"), tr("&Copy"), NULL);
    insertAction("ScriptEdit.Copy", ac);
    ac = new QAction(QIcon(":/images/editpaste.png"), tr("&Paste"), NULL);
    insertAction("ScriptEdit.Paste", ac);
    ac = new QAction(QIcon(":/images/zoomin.png"), tr("Zoomin"), NULL);
    insertAction("ScriptEdit.Zoomin", ac);
    ac = new QAction(QIcon(":/images/zoomout.png"), tr("Zoomout"), NULL);
    insertAction("ScriptEdit.Zoomout", ac);
    ac = new QAction(QIcon(":/images/font.png"), tr("Set Font"), NULL);
    insertAction("ScriptEdit.Font", ac);
    ac = new QAction(QIcon(":/images/fitinscreen.png"), tr("Search"), NULL);
    insertAction("ScriptEdit.Search", ac);
}

void QSoftCore::initDataAction()
{
    QAction *ac;

    ac = new QAction(QIcon(":/images/plus.png"), tr("Add Group"), NULL);
    insertAction("Data.Group.Add", ac);
    ac = new QAction(QIcon(":/images/minus.png"), tr("Remove Group"), NULL);
    insertAction("Data.Group.Del", ac);
}

void QSoftCore::initDriverAction()
{
    QAction *ac;

    ac = new QAction(QIcon(":/images/plus.png"), tr("Add Driver"), NULL);
    insertAction("Driver.Add", ac);
    ac = new QAction(QIcon(":/images/minus.png"), tr("Remove Driver"), NULL);
    insertAction("Driver.Del", ac);
}

void QSoftCore::initRunningAction()
{
    QAction *ac;
    ac = new QAction(QIcon(":/images/run.png"), tr("Run"), NULL);
    insertAction("Debug.Run", ac);
    ac = new QAction(QIcon(":/images/stop.png"), tr("Stop"), NULL);
    insertAction("Debug.Stop", ac);
}

void QSoftCore::initDeviceAction()
{
    QAction* ac;
    ac = new QAction(QIcon(":/images/device.png"), tr("Update"), NULL);
    insertAction("Device.Update", ac);
    ac = new QAction(QIcon(":/images/sync.png"), tr("Sync Data"), NULL);
    insertAction("Device.Sync", ac);
}

void QSoftCore::initUndoAction()
{
    QAction* ac;
    ac = m_undoGroup->createRedoAction(NULL);
    ac->setIcon(QIcon(":/images/redo.png"));
    ac->setShortcut(QKeySequence::Redo);
    insertAction("Undo.Redo", ac);
    ac = m_undoGroup->createUndoAction(NULL);
    ac->setIcon(QIcon(":/images/undo.png"));
    ac->setShortcut(QKeySequence::Undo);
    insertAction("Undo.Undo", ac);
}

QProjectCore* QSoftCore::getProjectCore()
{
    return m_projectCore;
}

void QSoftCore::addUndoStack(QUndoStack *stack)
{
    m_undoGroup->addStack(stack);
}

void QSoftCore::setActivityStack(QUndoStack *stack)
{
    m_undoGroup->setActiveStack(stack);
}

/**
 * @brief Core::insertToolBar
 * @details 添加全局工具条
 * @param szKey 工具条对象对应的ID
 * @param pToolBarObj 工具条对象
 */
void QSoftCore::insertToolBar(const QString& szKey, QToolBar *pToolBarObj)
{
    m_mapToolBar.insert(szKey.toLower(), pToolBarObj);
}

/**
 * @brief Core::getToolBar
 * @details 获取全局工具条
 * @param szKey 工具条对象对应的ID
 * @return 工具条对象
 */
QToolBar* QSoftCore::getToolBar(const QString &szKey)
{
    return m_mapToolBar.value(szKey.toLower());
}
