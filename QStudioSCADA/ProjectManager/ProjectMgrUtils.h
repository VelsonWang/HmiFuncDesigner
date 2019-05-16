#ifndef PROJECTMGRUTILS_H
#define PROJECTMGRUTILS_H

#include <QString>



class ProjectMgrUtils
{
public:
    static QString getProjectPath(const QString &projectName);
    static QString getProjectNameWithSuffix(const QString &projectName);
    static QString getProjectNameWithOutSuffix(const QString &projectName);

private:
    ProjectMgrUtils();
};

#endif // PROJECTMGRUTILS_H
