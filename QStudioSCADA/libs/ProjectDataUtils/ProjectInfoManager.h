#ifndef PROJECTINFOMANAGER_H
#define PROJECTINFOMANAGER_H

#include <QString>

class ProjectInfoPrivate;
class ProjectDataSQLiteDatabase;

class ProjectInfoManager
{
public:
    ProjectInfoManager();
    ~ProjectInfoManager();

    bool load(ProjectDataSQLiteDatabase *pDB);
    bool save(ProjectDataSQLiteDatabase *pDB);

public:
    QString getProjectName() const;
    void setProjectName(const QString &name);

    QString getProjectDescription() const;
    void setProjectDescription(const QString &desc);

    QString getProjectPath() const;
    void setProjectPath(const QString &path);

    QString getDeviceType() const;
    void setDeviceType(const QString &type);

    int getStationNumber() const;
    void setStationNumber(int number);

    QString getStartPage() const;
    void setStartPage(const QString &page);

    QString getStationAddress() const;
    void setStationAddress(const QString &address);

    bool getProjectEncrypt();
    void setProjectEncrypt(bool encrypt);

    int getPageScanPeriod() const;
    void setPageScanPeriod(int period);

    int getDataScanPeriod() const;
    void setDataScanPeriod(int period);

    // 画面宽度
    int getGraphPageWidth();
    // 画面高度
    int getGraphPageHeight();

private:
    ProjectInfoPrivate *const dPtr_;
};

#endif // PROJECTINFOMANAGER_H
