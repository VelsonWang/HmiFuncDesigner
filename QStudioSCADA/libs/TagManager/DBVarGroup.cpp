#include "DBVarGroup.h"
#include <QFile>

DBVarGroup::DBVarGroup() :
    m_type(""),
    m_name("") {

}

DBVarGroups::DBVarGroups() {

}

void DBVarGroups::setProjectPath(const QString &path) {
    szProjectPath_ = path;
}

void DBVarGroups::load(const QJsonObject &json) {
    varBlockGroupList_.clear();
    QJsonArray DevVarTabArray = json["DevVarTabArray"].toArray();
    for (int i = 0; i < DevVarTabArray.size(); ++i) {
        DBVarGroup *pVarGroup = new DBVarGroup();
        QJsonObject jsonObj = DevVarTabArray[i].toObject();
        pVarGroup->m_iPageID = jsonObj["pageid"].toInt();
        pVarGroup->m_type = jsonObj["type"].toString();
        pVarGroup->m_name = jsonObj["name"].toString();
        varBlockGroupList_.append(pVarGroup);
    }
}

void DBVarGroups::save(QJsonObject &json) {
    QJsonArray DevVarTabArray;
    for(int i = 0; i < varBlockGroupList_.count(); i++)
    {
        QJsonObject jsonObj;
        varBlockGroupList_.at(i)->m_iPageID = i;
        jsonObj["pageid"] = varBlockGroupList_.at(i)->m_iPageID;
        jsonObj["type"] = varBlockGroupList_.at(i)->m_type;
        jsonObj["name"] = varBlockGroupList_.at(i)->m_name;
        DevVarTabArray.append(jsonObj);
    }
    json["DevVarTabArray"] = DevVarTabArray;
}

bool DBVarGroups::loadFromFile(SaveFormat saveFormat) {
    QString file = szProjectPath_ + "/DBVarList.odb";
    QFile loadFile(file);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file.");
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(saveData) : QJsonDocument::fromBinaryData(saveData));
    load(loadDoc.object());
    loadFile.close();
    return true;
}

bool DBVarGroups::saveToFile(SaveFormat saveFormat) {
    QString file = szProjectPath_ + "/DBVarList.odb";
    QFile saveFile(file);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject obj;
    save(obj);
    QJsonDocument saveDoc(obj);
    saveFile.write(saveFormat == Json ? saveDoc.toJson() : saveDoc.toBinaryData());
    saveFile.close();
    return true;
}
