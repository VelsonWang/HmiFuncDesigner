#ifndef ILOADER_H
#define ILOADER_H

#include <QString>
#include <QDebug>
#include "../picoc/parsescript.h"

class XMLObject;

#define PROPERTY_TITLE "Property"
#define ATTR_EDITABLE "Editable"
#define ATTR_NEEDSAVE "Need_Save"
#define ATTR_VISIBLE  "Visible"
#define ATTR_CAN_SAME "Can_Same"
#define ATTR_RESET_ABLEABLE "Can_Reset"
#define OBJECT_TITLE "Object"


class ILoader
{
public:
    virtual void fromObject(XMLObject* xml) = 0;
};

inline QStringList praseFunctions(const QString &scriptText)
{
    QStringList listRetFuncs;
    QStringList listFuncs = scriptText.split("|");
    foreach (QString func, listFuncs) {
        QStringList listTmpFuncs = func.split(":");
        if(listTmpFuncs.count() == 2) {
            listRetFuncs.append(listTmpFuncs.at(0));
        }
    }
    return listRetFuncs;
}

inline int execFunction(const QStringList &funcs)
{
    if(funcs.count() == 0) {
        return -1;
    }
    foreach (QString func, funcs) {
        ParseScript::instance()->run_c(func);
    }
    return 1;
}

#endif // ILOADER_H
