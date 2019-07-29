#include "PictureResourceManager.h"
#include "ProjectDataSQLiteDatabase.h"
#include "ProjectData.h"
#include <QFileInfo>
#include <QFile>


PictureResourceManager::PictureResourceManager()
{

}

bool PictureResourceManager::add(ProjectDataSQLiteDatabase *pDB,
                                 const QString &name)
{
    bool bRet = false;
    QString expr = QString("name = '%1'").arg(name);
    int iCnt = pDB->getRowCount(szTableName_, expr);
    if(iCnt) {
        QString key = "ref_count";
        QString value = "";
        bRet = pDB->getRecord(szTableName_, key, value);
        if(bRet && value != "") {
            int iRefCnt = value.toInt();
            bRet = pDB->setRecord(szTableName_, key, QString::number(iRefCnt + 1));
        }
    } else {
        QStringList keyList,valueList;

        keyList << "name" << "ref_count ";
        valueList << name << QString::number(1);

        bRet = pDB->insertRecord(szTableName_, keyList, valueList);
    }

    return bRet;
}


bool PictureResourceManager::del(ProjectDataSQLiteDatabase *pDB,
                                 const QString &name)
{
    bool bRet = false;
    QString expr = QString("name = '%1'").arg(name);
    int iCnt = pDB->getRowCount(szTableName_, expr);
    if(iCnt) {
        QString key = "ref_count";
        QString value = "";
        bRet = pDB->getRecord(szTableName_, key, value);
        if(bRet && value != "") {
            int iRefCnt = value.toInt();
            if(iRefCnt == 1) {
                bRet = pDB->deleteRecord(szTableName_, expr);
                QString szPicturePath = ProjectData::getInstance()->szProjPath_ + "/Pictures";
                QString szPicFile = szPicturePath + "/" + name;
                QFileInfo infoPic(szPicFile);
                if(infoPic.exists()) {
                    QFile::remove(szPicFile);
                }
            } else {
                bRet = pDB->setRecord(szTableName_, key, QString::number(iRefCnt - 1));
            }
        }
    }

    return bRet;
}

