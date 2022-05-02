#include "qwidgetfactory.h"
#include "host/qabstracthost.h"
#include "host/qformhost.h"
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
    if(name == FORM_TITLE) {
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

    if(xml->getTagName() == FORM_TITLE) {
        pWidgetObj = new QFormWidget;
        pWidgetObj->setProperty("uuid", xml->getProperty("uuid"));
        ILoader *pLoaderObj = dynamic_cast<ILoader *>(pWidgetObj);
        if(pLoaderObj) {
            pLoaderObj->fromObject(xml);
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
