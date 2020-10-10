#include "ProjectData.h"
#include "ProjectDataSQLiteDatabase.h"
#include "XMLObject.h"
#include "Helper.h"
#include <QDebug>

ProjectDataSQLiteDatabase *ProjectData::dbData_ = Q_NULLPTR;
QString ProjectData::szProjPath_ = "";
QString ProjectData::szProjName_ = "";


ProjectData::ProjectData()
    : dbPath_(""),
      szProjVersion_("V1.0.0")
{

}

ProjectData::~ProjectData()
{
    if(dbData_ != Q_NULLPTR) {
        delete dbData_;
        dbData_ = Q_NULLPTR;
    }
}


ProjectData* ProjectData::getInstance()
{
    static ProjectData instance_;
    return &instance_;
}


bool ProjectData::openFromXml(const QString &szProjFile)
{
    QString buffer = Helper::readString(szProjFile);
    XMLObject xml;
    if(!xml.load(buffer, 0))
        return false;
/*
    QList<XMLObject*> children = xml.getChildren();
    foreach(XMLObject* obj, children)
        obj->showXMLObject();

    qDebug() << "\n\n\n=========get child name is person=========\n";
    children.clear();
    children = xml.getCurrentChildren("person");
    foreach(XMLObject* obj, children)
        obj->showXMLObject();

    qDebug() << "\n\n\n=========getChildrenByParentTagName=========\n";
    QStringList parents;
    parents<< "persons";
    parents<< "person";
    parents<< "baby";
    children.clear();
    xml.getChildrenByParentTagName(parents, children);
    foreach(XMLObject* obj, children){
        obj->showXMLObject();
        obj->setProperty("cb", "123");
    }
    FileHelper::writeString(project_path + "/pages.xml", xml.write());
*/
    return true;
}


bool ProjectData::saveToXml(const QString &szProjFile)
{
    XMLObject projObjs;
    projObjs.setTagName("projects");

    XMLObject *pProjObj = new XMLObject(&projObjs);
    pProjObj->setTagName("project");
    pProjObj->setProperty("version", szProjVersion_);

    // 工程信息管理
    projInfoMgr_.saveToXml(pProjObj);
    // 网络配置
    netSetting_.saveToXml(pProjObj);


    Helper::writeString(szProjFile, projObjs.write());
/*
    XMLObject persons;
    persons.setTagName("persons");

    XMLObject *person;
    person = new XMLObject(&persons);
    person->setTagName("person");
    person->setText("hi, my name is jason.");
    person->setProperty("name", "jason");
    person->setProperty("age", "29");
    person->setProperty("sex", "male");

    person = new XMLObject(&persons);
    person->setTagName("person");
    person->setText("hi, my name is velson.");
    person->setProperty("name", "velson");
    person->setProperty("age", "28");
    person->setProperty("sex", "male");

    person = new XMLObject(&persons);
    person->setTagName("person");
    person->setText("hi, my name is lucy.");
    person->setProperty("name", "lucy");
    person->setProperty("age", "25");
    person->setProperty("sex", "female");

    XMLObject *baby = new XMLObject(person);
    baby->setTagName("baby");
    baby->setText("lucy==baby");
    baby->setProperty("name", "baby");
    baby->setProperty("age", "1");
    baby->setProperty("sex", "female");



 */
    return true;
}


/**
 * @brief ProjectData::createOrOpenProjectData
 * @details 创建或打开工程数据
 * @param projFile 工程路径
 * @param projFile 工程名称
 * @return true-成功, false-失败
 */
bool ProjectData::createOrOpenProjectData(const QString &projPath, const QString &projName)
{
    QString fileName = projPath;

    if(fileName.endsWith('/'))
        fileName.chop(1);

    szProjPath_ = fileName;
    szProjName_ = projName;

    if(szProjPath_.isEmpty() || szProjName_.isEmpty()) {
        qWarning() << __FILE__ << __LINE__ <<__FUNCTION__
                   << "szProjPath_: " << szProjPath_ << " "
                   << "szProjName_: " << szProjName_;
        return false;
    }

    fileName = szProjPath_ + "/" + szProjName_ + ".pdt";

    if(dbPath_ != "") {
        if(dbData_ != Q_NULLPTR) {
            delete dbData_;
            dbData_ = Q_NULLPTR;
        }
    }

    dbPath_ = fileName;
    dbData_ = new ProjectDataSQLiteDatabase(fileName);
    if(dbData_->openDatabase()) {
        dbData_->createTables();
        return true;
    }

    return false;
}


/**
 * @brief ProjectData::getDBPath
 * @details 获取工程数据文件路径
 * @return 工程数据文件路径
 */
QString ProjectData::getDBPath() const
{
    return dbPath_;
}


/**
 * @brief ProjectData::GetAllProjectVariableName
 * @details 获取工程所有变量的名称
 * @param varList 存储变量列表
 * @param type IO, TMP, SYS, ALL
 */
void ProjectData::getAllTagName(QStringList &varList, const QString &type)
{
    if(dbData_ == Q_NULLPTR)
        return;

    varList.clear();
    QString szType = type.toUpper();

    //-------------设备变量------------------//
    if(szType == "ALL" || szType == "IO") {
        tagIO_.load(dbData_);
        for(int i=0; i<tagIO_.listTagIODBItem_.count(); i++) {
            TagIODBItem *pObj = tagIO_.listTagIODBItem_.at(i);
            varList << (QObject::tr("设备变量.") + pObj->m_szName + "[" + pObj->m_szTagID + "]");
        }
        qDeleteAll(tagIO_.listTagIODBItem_);
        tagIO_.listTagIODBItem_.clear();
    }

    //-------------中间变量------------------//
    if(szType == "ALL" || szType == "TMP") {
        tagTmp_.load(dbData_);
        for(int i=0; i<tagTmp_.listTagTmpDBItem_.count(); i++) {
            TagTmpDBItem *pObj = tagTmp_.listTagTmpDBItem_.at(i);
            varList << (QObject::tr("中间变量.") + pObj->m_szName + "[" + pObj->m_szTagID + "]");
        }
        qDeleteAll(tagTmp_.listTagTmpDBItem_);
        tagTmp_.listTagTmpDBItem_.clear();
    }

    //-------------系统变量------------------//
    if(szType == "ALL" || szType == "SYS") {
        tagSys_.load(dbData_);
        for(int i=0; i<tagSys_.listTagSysDBItem_.count(); i++) {
            TagSysDBItem *pObj = tagSys_.listTagSysDBItem_.at(i);
            varList << (QObject::tr("系统变量.") + pObj->m_szName + "[" + pObj->m_szTagID + "]");
        }
        qDeleteAll(tagSys_.listTagSysDBItem_);
        tagSys_.listTagSysDBItem_.clear();
    }
}
