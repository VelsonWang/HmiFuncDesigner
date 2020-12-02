#include "Core.h"
#include <QFileInfo>
#include <QDebug>


Core::Core()
{

}

Core::~Core()
{

}


Core* Core::getInstance()
{
    static Core instance_;
    return &instance_;
}


/**
 * @brief Core::insertAction
 * @details 添加全局action对象
 * @param szKey action对象对应的ID
 * @param pActObj action对象
 */
void Core::insertAction(const QString& szKey, QAction *pActObj)
{
    QStringList szActKeyList= szKey.toLower().split(".");
    szActKeyList.removeAll("");
    if(szActKeyList.size() == 0) return;

    QActionArray *pActGroupObjs = &m_actions;
    QActionArray *pTmpActObjs = Q_NULLPTR;

    foreach(const QString &szActKey, szActKeyList) {
        pTmpActObjs = pActGroupObjs->m_children.value(szActKey);
        if(pTmpActObjs == Q_NULLPTR) {
            pTmpActObjs = new QActionArray;
            pActGroupObjs->m_children.insert(szActKey, pTmpActObjs);
        }
        pActGroupObjs = pTmpActObjs;
    }

    if(pActGroupObjs->m_pActionObj != Q_NULLPTR) {
        delete pActGroupObjs->m_pActionObj;
        pActGroupObjs->m_pActionObj = Q_NULLPTR;
    }
    pActGroupObjs->m_pActionObj = pActObj;
}


/**
 * @brief Core::getAction
 * @details 获取全局action
 * @param szKey action对象对应的ID
 * @return action对象
 */
QAction* Core::getAction(const QString &szKey)
{
    QStringList szActKeyList= szKey.toLower().split(".");
    szActKeyList.removeAll("");
    if(szActKeyList.size() == 0) return Q_NULLPTR;

    QActionArray *pActGroupObjs = &m_actions;
    QActionArray *pTmpActObjs = Q_NULLPTR;

    foreach(const QString &szActKey, szActKeyList) {
        pTmpActObjs = pActGroupObjs->m_children.value(szActKey);
        if(pTmpActObjs == Q_NULLPTR) {
            return Q_NULLPTR;
        }
        pActGroupObjs = pTmpActObjs;
    }

    return pActGroupObjs->m_pActionObj;
}
