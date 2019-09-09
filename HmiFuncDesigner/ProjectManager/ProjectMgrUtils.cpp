#include "ProjectMgrUtils.h"
#include <QFileInfo>


ProjectMgrUtils::ProjectMgrUtils()
{

}

/**
 * @brief ProjectMgrUtils::getProjectPath
 * @details 获取工程路径
 * @param projectName 工程名称全路径
 * @return 工程路径
 */
QString ProjectMgrUtils::getProjectPath(const QString &projectName) {
    QString path = QString();
    int pos = projectName.lastIndexOf("/");
    if (pos != -1) {
        path = projectName.left(pos);
    }
    return path;
}

/**
 * @brief ProjectMgrUtils::getProjectNameWithSuffix
 * @details 获取包含后缀工程名称
 * @param projectName 工程名称全路径
 * @return 工程名称包含后缀
 */
QString ProjectMgrUtils::getProjectNameWithSuffix(const QString &projectName) {
    QFileInfo projFileInfo(projectName);
    return projFileInfo.fileName();
}

/**
 * @brief ProjectMgrUtils::getProjectNameWithOutSuffix
 * @details 获取不包含后缀工程名称
 * @param projectName 工程名称全路径
 * @return 工程名称不包含后缀
 */
QString ProjectMgrUtils::getProjectNameWithOutSuffix(const QString &projectName) {
    QFileInfo projFileInfo(projectName);
    return projFileInfo.baseName();
}
