#include "ProjectMgrUtils.h"

ProjectMgrUtils::ProjectMgrUtils()
{

}

QString ProjectMgrUtils::getProjectPath(const QString &projectName) {

    QString path = QString();
    int pos = projectName.lastIndexOf("/");
    if (pos != -1) {
        path = projectName.left(pos);
    }
    return path;
}
