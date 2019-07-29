#include <QString>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QObject>
#include <QDebug>
#include "TagManager.h"

QList<TagSysItem> TagManager::tagSysItems_;
QList<TagTmpItem> TagManager::tagTmpItems_;
QMap<QString, QList<TagIOItem>> TagManager::listTagIOItems_;
DBVarGroups TagManager::ioDBVarGroups_;
QString TagManager::szProjectPath_;

TagManager::TagManager()
{
    clearData();
}

TagManager::~TagManager()
{
    clearData();
}


/**
 * @brief TagManager::clearData
 * @details 清除数据
 */
void TagManager::clearData() {
    tagSysItems_.clear();
    tagTmpItems_.clear();
    listTagIOItems_.clear();
}


/**
 * @brief TagManager::setProjectPath
 * @details 设置工程路径
 * @param path 工程路径
 */
void TagManager::setProjectPath(const QString &path) {
    if(szProjectPath_ != path) {
        szProjectPath_ = path;
        clearData();
    }
}


/**
 * @brief TagManager::getProjectPath
 * @details 取得工程路径
 * @return 工程路径
 */
QString TagManager::getProjectPath() {
    return szProjectPath_;
}


/**
 * @brief TagManager::loadProjectTags
 * @details 读取工程所有的变量
 * @param path 工程路径
 */
void TagManager::loadProjectTags(const QString &path) {
    QString file = "";
    setProjectPath(path);

    /////////////////////////////////////////////////////////////////////
    /// 设备变量
    ioDBVarGroups_.setProjectPath(szProjectPath_);
    ioDBVarGroups_.loadFromFile(DATA_SAVE_FORMAT);

    foreach(DBVarGroup *var, TagManager::ioDBVarGroups_.varBlockGroupList_) {
        QString blockName = var->m_name;
        file = szProjectPath_ + "/DevVarList-" + blockName + ".odb";
        QFile loadIOTagFile(file);
        if (!loadIOTagFile.open(QIODevice::ReadOnly)) {
            qWarning() << QString("Couldn't open load file: %1.").arg(file);
            return;
        }
        QByteArray loadIOData = loadIOTagFile.readAll();
        QJsonDocument loadIODoc(DATA_SAVE_FORMAT == Json ? QJsonDocument::fromJson(loadIOData) : QJsonDocument::fromBinaryData(loadIOData));
        QJsonObject jsonIO = loadIODoc.object();

        QList<TagIOItem> tagIOItems;
        QJsonArray IOVarArray = jsonIO["IOVarArray"].toArray();
        for (int i = 0; i < IOVarArray.size(); ++i) {
            QJsonObject jsonObj = IOVarArray[i].toObject();
            TagIOItem item;
            item.load(jsonObj);
            tagIOItems.append(item);
        }
        listTagIOItems_[blockName] = tagIOItems;
    }

    /////////////////////////////////////////////////////////////////////
    /// 中间变量
    file = szProjectPath_ + "/TmpVarList.odb";
    QFile loadTmpTagFile(file);
    if (!loadTmpTagFile.open(QIODevice::ReadOnly)) {
        qWarning() << QString("Couldn't open load file: %1.").arg(file);
        return;
    }
    QByteArray loadTmpData = loadTmpTagFile.readAll();
    QJsonDocument loadTmpDoc(DATA_SAVE_FORMAT == Json ? QJsonDocument::fromJson(loadTmpData) : QJsonDocument::fromBinaryData(loadTmpData));
    QJsonObject jsonTmp = loadTmpDoc.object();

    tagTmpItems_.clear();
    QJsonArray TmpVarArray = jsonTmp["TmpVarArray"].toArray();
    for (int i = 0; i < TmpVarArray.size(); ++i) {
        QJsonObject jsonObj = TmpVarArray[i].toObject();
        TagTmpItem item;
        item.load(jsonObj);
        tagTmpItems_.append(item);
    }

    /////////////////////////////////////////////////////////////////////
    /// 系统变量
    file = szProjectPath_ + "/SysVarList.odb";
    QString srcfile = QCoreApplication::applicationDirPath() + "/SysVarList.odb";
    QFile testFile(file);
    if(testFile.exists()) {
        testFile.remove();
    }
    QFile::copy(srcfile ,file);

    QFile loadSysTagFile(file);
    if (!loadSysTagFile.open(QIODevice::ReadOnly)) {
        qWarning() << QString("Couldn't open load file: %1.").arg(file);
        return;
    }
    QByteArray loadSysData = loadSysTagFile.readAll();
    QJsonDocument loadSysDoc(DATA_SAVE_FORMAT == Json ? QJsonDocument::fromJson(loadSysData) : QJsonDocument::fromBinaryData(loadSysData));
    QJsonObject jsonSys = loadSysDoc.object();
    tagSysItems_.clear();
    QJsonArray tagSysArray = jsonSys["SysVarArray"].toArray();
    for (int i = 0; i < tagSysArray.size(); ++i) {
        QJsonObject jsonObj = tagSysArray[i].toObject();
        TagSysItem item;
        item.load(jsonObj);
        tagSysItems_.append(item);
    }
}


/**
 * @brief TagManager::saveProjectTags
 * @details 保存工程所有的变量
 * @param path 工程路径
 */
void TagManager::saveProjectTags(const QString &path) {
    Q_UNUSED(path)
}


/**
 * @brief TagManager::GetAllProjectVariableName
 * @details 获取工程所有变量的名称
 * @param projPath 工程路径
 * @param varList 存储变量列表
 * @param type IO, TMP, SYS, ALL
 */
void TagManager::getAllTagName(const QString &projPath,
                               QStringList &varList,
                               const QString &type)
{
    loadProjectTags(projPath);
    varList.clear();
    QString szType = type.toUpper();

    //-------------设备变量------------------//
    if(szType == "ALL" || szType == "IO") {
        QMap<QString, QList<TagIOItem>>::const_iterator iter = listTagIOItems_.constBegin();
        while (iter != listTagIOItems_.constEnd()) {
            QString blockName = iter.key();
            foreach(TagIOItem item, iter.value()) {
                varList << (QObject::tr("设备变量.") + blockName + "." + item.m_sName);
            }
            ++iter;
        }
    }

    //-------------中间变量------------------//
    if(szType == "ALL" || szType == "TMP") {
        foreach(TagTmpItem item, tagTmpItems_) {
            varList << (QObject::tr("中间变量.") + item.m_sName);
        }
    }

    //-------------系统变量------------------//
    if(szType == "ALL" || szType == "SYS") {
        foreach(TagSysItem item, tagSysItems_) {
            varList << (QObject::tr("系统变量.") + item.m_sName);
        }
    }
}





