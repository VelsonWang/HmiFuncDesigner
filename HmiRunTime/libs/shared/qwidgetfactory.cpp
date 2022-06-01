#include "qwidgetfactory.h"
#include "host/qabstracthost.h"
#include "widgets/qformwidget.h"
#include "xmlobject.h"

QMap<QString, tagWidgetInfo*> QWidgetFactory::m_metaMap;

QWidgetFactory::QWidgetFactory()
{
}

void QWidgetFactory::registerWidget(const QString name, const QMetaObject *host)
{
    tagWidgetInfo *info = m_metaMap.value(name);
    if(info == NULL) {
        info = new tagWidgetInfo;
    }
    info->m_hostObject = host;
    info->m_name = name;
    m_metaMap.insert(name, info);
}

QWidget* QWidgetFactory::createWidget(const QString &name)
{
    QWidget* pWidgetObj = NULL;
    if(name == "form") {
        pWidgetObj = new QFormWidget;
    } else {
        tagWidgetInfo *info = m_metaMap.value(name);
        if(info == NULL) {
            return NULL;
        }
        const QMetaObject* obj = info->m_hostObject;
        if(obj != NULL) {
            pWidgetObj = (QWidget*)obj->newInstance();
        }
    }

    return pWidgetObj;
}

QMap<QString, tagWidgetInfo*> QWidgetFactory::getWidgetInfo()
{
    return m_metaMap;
}

QWidget* QWidgetFactory::createWidget(XMLObject *xml)
{
    if(xml == NULL) {
        return NULL;
    }

    QWidget* pWidgetObj = NULL;

    if(xml->getTagName() == "form") {
        //解析画面控件
        pWidgetObj = new QFormWidget;
        pWidgetObj->setProperty("uuid", xml->getProperty("uuid"));
        ILoader *pLoaderObj = dynamic_cast<ILoader *>(pWidgetObj);
        if(pLoaderObj) {
            pLoaderObj->fromObject(xml);
        }
        //解析画面子控件
        QList<XMLObject*> objects = xml->getChildren();
        foreach(XMLObject* obj, objects) {
            if(obj->getTagName() == OBJECT_TITLE) {
                QWidget* pChildWidgetObj = createWidget(obj->getProperty(HOST_TYPE));
                if(pChildWidgetObj) {
                    pChildWidgetObj->setProperty("uuid", obj->getProperty("uuid"));
                    ILoader *pLoaderObj = dynamic_cast<ILoader *>(pChildWidgetObj);
                    if(pLoaderObj) {
                        pLoaderObj->fromObject(obj);
                    }
                    pChildWidgetObj->setParent(pWidgetObj);
                }
            }
        }
    } else {
        pWidgetObj = createWidget(xml->getProperty(HOST_TYPE));
        if(pWidgetObj) {
            pWidgetObj->setProperty("uuid", xml->getProperty("uuid"));
            ILoader *pLoaderObj = dynamic_cast<ILoader *>(pWidgetObj);
            if(pLoaderObj) {
                pLoaderObj->fromObject(xml);
            }
        }
    }
    return pWidgetObj;
}

tagWidgetInfo* QWidgetFactory::getWidgetInfo(const QString &name)
{
    return m_metaMap.value(name);
}
