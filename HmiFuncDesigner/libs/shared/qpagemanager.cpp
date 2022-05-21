#include "qpagemanager.h"
#include "xmlobject.h"
#include "host/qabstracthost.h"
#include "host/qformhost.h"
#include "qhostfactory.h"
#include "property/qabstractproperty.h"
#include <QFile>
#include <QDebug>

QPageManager::QPageManager()
{
}

void QPageManager::load(const QString &project_path)
{
    clear();
    m_project_path = project_path;
    QFile f(m_project_path + "/pages.xml");
    if(!f.open(QFile::ReadOnly)) {
        return;
    }
    QString buffer = f.readAll();
    f.close();
    XMLObject xml;
    if(!xml.load(buffer, 0)) {
        return;
    }

    QList<XMLObject*> children = xml.getChildren();

    foreach(XMLObject* obj, children) {
        load_page(obj->getProperty("path"));
    }
}

void QPageManager::save(const QString &project_path)
{
    XMLObject path;
    path.setTagName("Pages");

    XMLObject page;

    XMLObject *temp;

    QString p;
    QString old_path;

    foreach(QAbstractHost* h, m_page_list) {
        temp = new XMLObject(&path);
        temp->setTagName("Page");
        p = h->getHostType() + "/" + h->getPropertyValue("objectName").toString() + ".xml";
        temp->setProperty("path", p);
        page.clear();
        h->toObject(&page);
        old_path = h->property("old_file_name").toString();
        if(old_path != "" && old_path != p) {
            QFile::remove(project_path + "/pages/" + old_path);
        }
        h->setProperty("old_file_name", p);
        QFile f(project_path + "/pages/" + p);
        if(f.open(QFile::WriteOnly)) {
            f.resize(0);
            f.write(page.write().toLocal8Bit());
            f.close();
        }
    }

    QFile f(project_path + "/pages.xml");
    if(f.open(QFile::WriteOnly)) {
        f.resize(0);
        f.write(path.write().toLocal8Bit());
        f.close();
    }
}

void QPageManager::newPage(const QString &szPageName)
{
    QAbstractHost* pHostObj = QHostFactory::create_host(FORM_TITLE);
    if(pHostObj != NULL) {
        pHostObj->setID(QString::number(pHostObj->allocID()));
        m_page_list.insert(m_page_list.size(), pHostObj);
        m_id_to_page.insert(pHostObj->getID(), pHostObj);
        QList<QAbstractHost*> list;
        list.append(pHostObj);
        while(list.size() > 0) {
            QAbstractHost *h = list.takeFirst();
            QAbstractProperty* pro = h->getProperty("objectName");
            if(pro != NULL) {
                pro->setValue(QVariant(szPageName));
                connect(pro, SIGNAL(value_chaged(QVariant, QVariant)), this, SLOT(host_name_changed_slot(QVariant, QVariant)));
            }
            list += h->getChildren();
        }
        emit insert_page_signal(pHostObj);
        emit host_name_changed(pHostObj);
    }
}

void QPageManager::load(XMLObject *pXmlObj)
{
    clear();

    if(pXmlObj) { // 加载
        if(pXmlObj->getTagName() != "forms") {
            return;
        }
        QList<XMLObject*> listForms = pXmlObj->getChildren();
        foreach(XMLObject* pObj, listForms) {
            QAbstractHost* host = QHostFactory::create_host(pObj);
            if(host != NULL) {
                m_page_list.append(host);
                m_id_to_page.insert(host->getID(), host);
                host->setProperty("old_file_name", "");
                host->setDefault();
                QList<QAbstractHost*> list;
                list.append(host);
                while(list.size() > 0) {
                    QAbstractHost *h = list.takeFirst();
                    QAbstractProperty* pro = h->getProperty("objectName");
                    if(pro != NULL) {
                        connect(pro, SIGNAL(value_chaged(QVariant, QVariant)), this, SLOT(host_name_changed_slot(QVariant, QVariant)));
                    }
                    list += h->getChildren();
                }
            }
        }
    } else { // 新建
        QString buffer = "<form type=\"form\">\
                <Property name=\"objectName\" type=\"ByteArray\" value=\"main\"/>\
                <Property name=\"geometry\" type=\"Rect\">\
                <Property name=\"x\" type=\"Number\" value=\"0\"/>\
                <Property name=\"y\" type=\"Number\" value=\"0\"/>\
                <Property name=\"Width\" type=\"Number\" value=\"800\"/>\
                <Property name=\"Height\" type=\"Number\" value=\"600\"/>\
                </Property>\
                </form> ";
        XMLObject xml;
        QString str;
        if(!xml.load(buffer, &str)) {
            return;
        }

        QAbstractHost* host = QHostFactory::create_host(&xml);
        if(host != NULL) {
            m_page_list.append(host);
            m_id_to_page.insert(host->getID(), host);
            host->setProperty("old_file_name", "");
            host->setDefault();
            QList<QAbstractHost*> list;
            list.append(host);
            while(list.size() > 0) {
                QAbstractHost *h = list.takeFirst();
                QAbstractProperty* pro = h->getProperty("objectName");
                if(pro != NULL) {
                    connect(pro, SIGNAL(value_chaged(QVariant, QVariant)), this, SLOT(host_name_changed_slot(QVariant, QVariant)));
                }
                list += h->getChildren();
            }
        }
    }
}

void QPageManager::save(XMLObject *pXmlObj)
{
    foreach(QAbstractHost* h, m_page_list) {
        XMLObject *pPageObj = new XMLObject(pXmlObj);
        h->toObject(pPageObj);
        h->setProperty("old_file_name", "");
    }
}



void QPageManager::clear()
{
    //qDeleteAll(m_page_list);
    //m_page_list.clear();


    foreach(QAbstractHost* h, m_page_list) {
        h->deleteLater();
    }

    m_page_list.clear();
    m_project_path = "";
}

void QPageManager::load_page(const QString &fileName)
{
    QFile f(m_project_path + "/pages/" + fileName);

    if(!f.open(QFile::ReadOnly)) {
        return;
    }

    QString buffer = f.readAll();
    f.close();
    XMLObject xml;
    QString str;
    if(!xml.load(buffer, &str)) {
        return;
    }

    QAbstractHost* host = QHostFactory::create_host(&xml);
    if(host != NULL) {
        m_page_list.append(host);
        m_id_to_page.insert(host->getID(), host);
        host->setProperty("old_file_name", fileName);
        host->setDefault();
        QList<QAbstractHost*> list;
        list.append(host);
        while(list.size() > 0) {
            QAbstractHost *h = list.takeFirst();
            QAbstractProperty* pro = h->getProperty("objectName");
            if(pro != NULL) {
                connect(pro, SIGNAL(value_chaged(QVariant, QVariant)), this, SLOT(host_name_changed_slot(QVariant, QVariant)));
            }
            list += h->getChildren();
        }
    }
}

QList<QAbstractHost*> QPageManager::getPages()
{
    return m_page_list;
}

QAbstractHost* QPageManager::get_page(int index)
{
    if(index < 0 || index >= m_page_list.size()) {
        return NULL;
    } else {
        return m_page_list.at(index);
    }
}

QAbstractHost* QPageManager::get_page(const QString &id)
{
    return m_id_to_page.value(id);
}

void QPageManager::insert_page(QAbstractHost *host, int index)
{
    if(index < 0 || index > m_page_list.size()) {
        index = m_page_list.size();
    }
    m_page_list.insert(index, host);
    m_id_to_page.insert(host->getID(), host);
    QList<QAbstractHost*> list;
    list.append(host);
    while(list.size() > 0) {
        QAbstractHost *h = list.takeFirst();
        QAbstractProperty* pro = h->getProperty("objectName");
        if(pro != NULL) {
            connect(pro, SIGNAL(value_chaged(QVariant, QVariant)), this, SLOT(host_name_changed_slot(QVariant, QVariant)));
        }
        list += h->getChildren();
    }
    emit insert_page_signal(host);
    emit host_name_changed(host);
}

void QPageManager::remove_page(QAbstractHost *host)
{
    QList<QAbstractHost*> list;
    list.append(host);
    while(list.size() > 0) {
        QAbstractHost *h = list.takeFirst();
        QAbstractProperty* pro = h->getProperty("objectName");
        if(pro != NULL) {
            disconnect(pro, SIGNAL(value_chaged(QVariant, QVariant)), this, SLOT(host_name_changed_slot(QVariant, QVariant)));
        }
        list += h->getChildren();
    }
    emit remove_page_signal(host);
    emit host_name_changed(host);
    m_page_list.removeAll(host);
    m_id_to_page.remove(host->getID());
}

QList<QAbstractHost*> QPageManager::getPages_by_title(const QString &title)
{
    QList<QAbstractHost*> list;
    foreach(QAbstractHost* host, m_page_list) {
        if(host->property("title").toString() == title) {
            list.append(host);
        }
    }
    return list;
}

void QPageManager::host_name_changed_slot(const QVariant &, const QVariant &)
{
    QAbstractProperty *pro = (QAbstractProperty*)sender();

    emit host_name_changed(pro->getHost());
}


/**
 * @brief QPageManager::getAllElementIDName
 * @details 获取工程所有控件的ID名称
 * @param szIDList 所有控件的ID名称
 */
void QPageManager::getAllElementIDName(QStringList &szIDList)
{
    //    if(pImplGraphPageSaveLoadObj_) {
    //        pImplGraphPageSaveLoadObj_->getAllElementIDName(szIDList);
    //    }
}

/**
 * @brief QProjectCore::getAllPageName
 * @details 获取工程所有画面名称
 * @param szList 所有画面名称
 */
void QPageManager::getAllPageName(QStringList &szList)
{
    szList.clear();
    foreach(QAbstractHost* pHostObj, m_page_list) {
        QString szTitle = pHostObj->property("title").toString();
        QString id = pHostObj->getAttribute("id");
        //qDebug() << "title: " << szTitle;
        if(szTitle == FORM_TITLE) {
            QAbstractProperty* pProObj = pHostObj->getProperty("objectName");
            if(pProObj) {
                QString szPageName = pProObj->getValue().toString();
                //qDebug() << "page name: " << szPageName;
                szList.append(QString("%1:%2").arg(id).arg(szPageName));
            }
        }
    }
}
