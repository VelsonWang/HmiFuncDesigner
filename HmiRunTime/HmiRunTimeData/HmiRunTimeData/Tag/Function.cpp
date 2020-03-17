#include "Function.h"
#include "../DB/RealTimeDB.h"

Function::Function()
{

}

Function::~Function()
{

}


void Function::LoadFuncObjects(const QString projectConverter)
{
    m_listFuncObject.clear();
    if(projectConverter == "")
        return;
    QStringList strObjList = projectConverter.split(';');
    for(int i=0; i<strObjList.size(); i++) {
        QStringList strObjTmpList = strObjList.at(i).split('-');
        if(strObjTmpList.size() != 2)
            continue;
        PFuncObjectItem pfuncItem = new FuncObjectItem();
        QString funcString = strObjTmpList.at(0);
        pfuncItem->name = funcString.left(funcString.indexOf("("));
        QString strArgs = (funcString.mid(funcString.indexOf("(") + 1, funcString.indexOf(")") - funcString.indexOf("(") - 1)).trimmed();

        if(strArgs.length()>0) {
            QStringList tmpList;
            tmpList = strArgs.split(',');
            for(int i=0; i<tmpList.count();i++) {
                TArgItem item;
                item.arg = tmpList.at(i);
                if(item.arg.indexOf("变量")>-1)
                    item.type = "V";
                else
                    item.type = "C";
                pfuncItem->argList.append(item);
            }
        }

        pfuncItem->event = strObjTmpList.at(1);
        //pfuncItem->ShowInfo();
        m_listFuncObject.append(pfuncItem);
    }
}

/*
* 强制变量var1的值为value
*/
bool Function::SetValueToVariable(RuntimeEvent e, QString var1, QString value)
{
    Q_UNUSED(e)
    QString szTagID = RealTimeDB::instance()->getIdByTagName(var1);
    if(szTagID != "") {
        RealTimeDB::instance()->SetDataString(szTagID, value);
    }
    return true;
}

/*
* 强制变量var1的值为变量var2
*/
bool Function::SetVariableToVariable(RuntimeEvent e, QString var1, QString var2)
{
    Q_UNUSED(e)
    QString szTagID1 = RealTimeDB::instance()->getIdByTagName(var1);
    QString szTagID2 = RealTimeDB::instance()->getIdByTagName(var2);
    if(szTagID1 != "" && szTagID2 != "") {
        RealTimeDB::instance()->SetDataString(szTagID1, RealTimeDB::instance()->GetDataString(szTagID2));
    }
    return true;
}

/*
* 获取变量var1的值保存至当前变量
*/
bool Function::GetVariableValue(RuntimeEvent e, QString var1)
{
    QString szTagID = RealTimeDB::instance()->getIdByTagName(var1);
    if(szTagID != "") {
        DBTagObject* pThisTag = (DBTagObject*)e.getObj();
        if(pThisTag != nullptr) {
            RealTimeDB::instance()->SetDataStringInner(pThisTag->m_pDBTagObject->szID, RealTimeDB::instance()->GetDataString(szTagID));
        }
    }
    return true;
}

bool Function::ExecFunctions(RuntimeEvent e)
{
    QString sEvent = "";
    switch(e.getType()) {
        case BEFORE_READ: {
           sEvent = "变量被读取前";
        }
        break;
        case VALUE_CHANGE: {
           sEvent = "变量值被修改";
        }
        break;
        default:{

        } break;
    }
    //qDebug() << "event:" << sEvent;
    if(sEvent == "")
        return false;

    for (int i = 0; i < m_listFuncObject.size(); ++i) {
        PFuncObjectItem pFuncObjItem = m_listFuncObject.at(i);
        if(pFuncObjItem->event == sEvent) {
            if(pFuncObjItem->name == "SetValueToVariable") {
                return SetValueToVariable(e, pFuncObjItem->argList.at(0).arg, pFuncObjItem->argList.at(1).arg);
            } else if(pFuncObjItem->name == "SetVariableToVariable") {
                return SetVariableToVariable(e, pFuncObjItem->argList.at(0).arg, pFuncObjItem->argList.at(1).arg);
            } else if(pFuncObjItem->name == "GetVariableValue") {
                return GetVariableValue(e, pFuncObjItem->argList.at(0).arg);
            }
        }
    }
    return true;
}
