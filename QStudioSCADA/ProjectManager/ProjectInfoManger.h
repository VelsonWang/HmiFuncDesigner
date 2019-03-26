#ifndef PROJECTINFOMANGER_H
#define PROJECTINFOMANGER_H

#include "../Public/Public.h"
#include <QString>
#include <QJsonObject>

class ProjectInfoPrivate;

class ProjectInfoManger
{
public:
    ProjectInfoManger();
    ~ProjectInfoManger();

    bool loadFromFile(SaveFormat saveFormat, const QString &file);
    bool saveToFile(const QString &projPath,
                    const QString &projName,
                    SaveFormat saveFormat);

public:
    QString getProjectName() const;
    void setProjectName(const QString &name);

    QString getProjectDescription() const;
    void setProjectDescription(const QString &desc);

    QString getProjectPath() const;
    void setProjectPath(const QString &path);

    QString getDeviceType() const;
    void setDeviceType(const QString &type);

    QString getStationNumber() const;
    void setStationNumber(const QString &number);

    QString getStartPage() const;
    void setStartPage(const QString &page);

    QString getStationAddress() const;
    void setStationAddress(const QString &address);

    bool getProjectEncrypt();
    void setProjectEncrypt(bool encrypt);

    QString getPageScanPeriod() const;
    void setPageScanPeriod(const QString &period);

    QString getDataScanPeriod() const;
    void setDataScanPeriod(const QString &period);

    // 画面宽度
    int getGraphPageWidth();
    // 画面高度
    int getGraphPageHeight();

private:
    void load(const QJsonObject &json);
    void save(QJsonObject &json);

private:
    ProjectInfoPrivate *const dPtr_;
};

#endif // PROJECTINFOMANGER_H
