#ifndef PROJECTMGRUTILS_H
#define PROJECTMGRUTILS_H

#include <QString>



class ProjectMgrUtils
{
public:
    static QString getProjectPath(const QString &projectName);

private:
    ProjectMgrUtils();
};

#endif // PROJECTMGRUTILS_H
