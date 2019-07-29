#include "ProjectData.h"
#include "ProjectDataSQLiteDatabase.h"
#include <QMutexLocker>
#include <QMutex>
#include <cstdlib>

#include <QDebug>

ProjectData* ProjectData::instance_ = nullptr;
ProjectDataSQLiteDatabase *ProjectData::dbData_ = nullptr;
QMutex ProjectData::mutex_;
QString ProjectData::szProjPath_ = "";
QString ProjectData::szProjName_ = "";


ProjectData::ProjectData() :
    dbPath_("")
{

}

ProjectData::~ProjectData()
{
    if(dbData_ != nullptr) {
        delete dbData_;
        dbData_ = nullptr;
    }
}


ProjectData* ProjectData::getInstance()
{
    QMutexLocker locker(&mutex_);
    if(instance_ == nullptr) {
        instance_ = new ProjectData();
        ::atexit(releaseInstance);
    }
    return instance_;
}

void ProjectData::releaseInstance()
{
    if(instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}


/**
 * @brief ProjectData::createOrOpenProjectData
 * @details 创建或打开工程数据
 * @param projFile 工程路径
 * @param projFile 工程名称
 * @return true-成功, false-失败
 */
bool ProjectData::createOrOpenProjectData(const QString &projPath,
                                          const QString &projName)
{
    QString fileName = projPath;

    if(fileName.endsWith('/'))
        fileName.chop(1);

    szProjPath_ = fileName;
    szProjName_ = projName;

    fileName = fileName + "/" + projName + ".pdt";

    if(dbPath_ != "") {
        if(dbData_ != nullptr) {
            delete dbData_;
            dbData_ = nullptr;
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

