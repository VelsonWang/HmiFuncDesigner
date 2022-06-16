#include "qpagemanager.h"
#include "xmlobject.h"
#include "qwidgetfactory.h"
#include <QFile>
#include <QDebug>

QPageManager::QPageManager()
{
}

void QPageManager::load(XMLObject *pXmlObj)
{
    clear();

    if(pXmlObj) {
        if(pXmlObj->getTagName() != "forms") {
            return;
        }
        QList<XMLObject*> listForms = pXmlObj->getChildren();
        foreach(XMLObject* pObj, listForms) {
            QWidget* pWidgetObj = QWidgetFactory::createWidget(pObj);
            if(pWidgetObj != NULL) {
                m_pages.append(pWidgetObj);
                m_idToPage.insert(pWidgetObj->property("id").toString(), pWidgetObj);
            }
        }
    }
}

void QPageManager::clear()
{
    foreach(QWidget* pObj, m_pages) {
        pObj->deleteLater();
    }
    m_pages.clear();
}

QList<QWidget*> QPageManager::getPages()
{
    return m_pages;
}

QWidget* QPageManager::getPage(int index)
{
    if(index < 0 || index >= m_pages.size()) {
        return NULL;
    } else {
        return m_pages.at(index);
    }
}

QWidget* QPageManager::getPage(const QString &id)
{
    return m_idToPage.value(id);
}

/**
 * @brief QPageManager::getWidget 获取指定ID的控件
 * @param id [画面id, 控件id, 画面id.控件id]
 * @return 控件对象指针
 */
QWidget* QPageManager::getWidget(const QString& id)
{
    if(id.indexOf(".") > 0) {
        QStringList ids = id.split(".");
        if(ids.size() == 2) {
            if(m_idToPage.count(id) > 0) {
                QWidget *pPageObj = m_idToPage.value(id);
                if(pPageObj) {
                    QWidgetList list = pPageObj->findChildren<QWidget*>();
                    for(int i=0; i<list.count(); ++i) {
                        if(list.at(i)->property("id").toString().toLower() == id.toLower()) {
                            return list.at(i);
                        }
                    }
                }
            }
        }
    } else {
        if(m_idToPage.count(id) > 0) {
            return m_idToPage.value(id);
        } else {
            foreach(QWidget *pPageObj, m_pages) {
                QWidgetList list = pPageObj->findChildren<QWidget*>();
                for(int i=0; i<list.count(); ++i) {
                    if(list.at(i)->property("id").toString().toLower() == id.toLower()) {
                        return list.at(i);
                    }
                }
            }
        }
    }
    return NULL;
}

QList<QWidget*> QPageManager::getPagesByTitle(const QString &title)
{
    QList<QWidget*> list;
    foreach(QWidget* pObj, m_pages) {
        if(pObj->property("title").toString() == title) {
            list.append(pObj);
        }
    }
    return list;
}

int QPageManager::pageCount()
{
    return m_pages.size();
}
