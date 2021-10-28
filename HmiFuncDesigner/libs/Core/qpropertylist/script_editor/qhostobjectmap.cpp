#include "qhostobjectmap.h"
#include "../../qsoftcore.h"
#include "../../../shared/host/qabstracthost.h"
#include "../../../shared/qprojectcore.h"
#include "../../../shared/property/qabstractproperty.h"
#include "../../../shared/qpagemanager.h"
#include <QVariant>
#include <QMetaObject>
#include <QMetaMethod>

QHostObjectMap::QHostObjectMap(QAbstractProperty *property, QAbstractHost *host)
{
    QAbstractHost *h = QSoftCore::getCore()->getProjectCore()->getProjectHost();

    bool keyboard = host->property("title").toString() == "keyboard";
    bool project = host->property("title").toString() == "Project";

    tagHostInfo *info = makeInfo(h);
    info->m_name = "global";

    m_infos.insert("global", info);
    QList<QAbstractHost*> list = QSoftCore::getCore()->getProjectCore()->getPageManager()->getPages();

    if(!keyboard || project) {
        foreach(QAbstractHost* h, list) {
            if(h->property("title").toString() != "keyboard" || project) {
                tagHostInfo *info = makeInfo(h);
                m_infos.insert(info->m_name, info);
            }
        }
    }

    info = makeInfo(host);
    info->m_name = "self";
    m_infos.insert(info->m_name, info);
    if(!keyboard || project) {
        if(host->getParent() != NULL) {
            info = makeInfo(host->getParent());
            info->m_name = "parent";
            m_infos.insert(info->m_name, info);
        }
    } else {
        info = makeInfo(host);
        m_infos.insert(info->m_name, info);
    }

    m_paramList = property->getAttribute("param").toStringList();
}

tagHostInfo *QHostObjectMap::makeInfo(QAbstractHost *host)
{
    tagHostInfo *info = new tagHostInfo;
    info->m_host = host;
    info->m_type = "form";
    info->m_name = host->getPropertyValue("objectName").toString();

    QList<QAbstractHost*> list = host->getChildren();

    foreach(QAbstractHost* h, list) {
        tagHostInfo *in = makeInfo(h);
        info->m_children.insert(in->m_name, in);
    }

    const QMetaObject *meta = host->metaObject();

    int count = meta->methodCount();
    QString str;
    QStringList l = host->property("function_list").toStringList();
    for(int i = 0; i < count; i++) {
        QMetaMethod method = meta->method(i);
        if(method.methodType() == QMetaMethod::Slot && method.access() == QMetaMethod::Public) {
            str = method.methodSignature();
            if(l.contains(str)) {
                continue;
            }
            int index = str.indexOf("(");
            str = str.left(index);
            if(!info->m_children.keys().contains(str)) {
                info->m_methods.insert(str, method);
            }
        }
    }
    return info;
}

tagHostInfo* QHostObjectMap::getAllByText(const QString &text)
{
    QString s = text;
    if(s.endsWith(".")) {
        s = s.left(s.size() - 1);
    }
    QStringList list = s.split(".");
    if(list.size() == 0) {
        return NULL;
    }

    tagHostInfo *info = m_infos.value(list.takeFirst());

    while(info != NULL) {
        if(list.size() == 0) {
            break;
        }
        info = info->m_children.value(list.takeFirst());
    }
    return info;
}

tagReturnList QHostObjectMap::getInfo(const QString &text)
{
    tagReturnList ret;
    int index = text.lastIndexOf(".");
    QString temp1, temp2;
    if(index > 0) {
        tagHostInfo* info = getAllByText(text.left(index));
        if(info != NULL) {
            QString s = text.mid(index + 1);
            temp1 = s.toLower();
            QList<tagReturnInfo> retList1, retList2;
            if(info->m_type == "form") {
                QMapIterator<QString, tagHostInfo*> it(info->m_children);
                while(it.hasNext()) {
                    it.next();
                    temp2 = it.key().toLower();
                    tagReturnInfo info;
                    info.m_type = TYPE_FORM;
                    info.m_param = it.key();
                    info.m_is_last = false;
                    if(temp2.startsWith(temp1)) {
                        retList1.append(info);
                    } else if(temp2.indexOf(temp1) > 0) {
                        retList2.append(info);
                    }
                }

                QMapIterator<QString, QMetaMethod> itt(info->m_methods);
                while(itt.hasNext()) {
                    itt.next();
                    temp2 = itt.key().toLower();
                    tagReturnInfo info;
                    info.m_type = TYPE_FUNCTION;
                    info.m_method = itt.value();
                    info.m_param = itt.key();
                    info.m_is_last = true;
                    if(temp2.startsWith(temp1)) {
                        retList1.append(info);
                    } else if(temp2.indexOf(temp1) > 0) {
                        retList2.append(info);
                    }
                }
            } else {
                QMapIterator<QString, tagHostInfo*> it(info->m_children);
                while(it.hasNext()) {
                    it.next();
                    temp2 = it.key().toLower();
                    tagReturnInfo info;
                    info.m_type = TYPE_DATA;
                    info.m_param = it.key();
                    info.m_is_last = it.value()->m_children.size() == 0;
                    if(temp2.startsWith(temp1)) {
                        retList1.append(info);
                    } else if(temp2.indexOf(temp1) > 0) {
                        retList2.append(info);
                    }
                }
            }
            ret.m_returns = retList1 + retList2;
        }
    } else {
        QList<tagReturnInfo> retList1, retList2;
        QMapIterator<QString, tagHostInfo*> it(m_infos);
        temp1 = text.toLower();
        while(it.hasNext()) {
            it.next();
            temp2 = it.key().toLower();
            tagReturnInfo info;
            info.m_type = TYPE_FORM;
            info.m_param = it.key();
            info.m_is_last = false;
            if(temp2.startsWith(temp1)) {
                retList1.append(info);
            } else if(temp2.indexOf(temp1) > 0) {
                retList2.append(info);
            }
        }

        foreach(const QString &str, m_paramList) {
            temp2 = str.toLower();
            tagReturnInfo info;
            info.m_type = TYPE_PARAM;
            info.m_param = str;
            info.m_is_last = true;
            if(temp2.startsWith(temp1)) {
                retList1.append(info);
            } else if(temp2.indexOf(temp1) > 0) {
                retList2.append(info);
            }
        }
        ret.m_returns = retList1 + retList2;
    }

    foreach(const tagReturnInfo &info, ret.m_returns) {
        ret.m_name_to_return.insert(info.m_param, info);
    }

    return ret;
}
