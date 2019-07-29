#include "ProjectInfoManager.h"
#include "ProjectDataSQLiteDatabase.h"
#include "ulog.h"
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

class ProjectInfoManager;

class ProjectInfoPrivate
{
public:
    ProjectInfoPrivate() { }

    QString projectName_;
    QString projectDescription_;
    QString projectPath_;
    QString deviceType_;
    int stationNumber_;
    QString startPage_;
    QString stationAddress_;
    bool projectEncrypt_;
    int pageScanPeriod_;
    int dataScanPeriod_;
};



ProjectInfoManager::ProjectInfoManager()
    : dPtr_(new ProjectInfoPrivate()) {

}

ProjectInfoManager::~ProjectInfoManager() {
    if(dPtr_ != nullptr) {
        delete dPtr_;
    }
}

/**
 * @brief ProjectInfoManager::load
 * @details 读取工程信息
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool ProjectInfoManager::load(ProjectDataSQLiteDatabase *pDB) {
    QSqlQuery query(pDB->db_);
    QSqlRecord rec;

    bool ret = query.exec("select * from t_system_parameters");
    if(!ret) {
        LogError(QString("get record: %1 failed! %2 ,error: %3!")
                 .arg("t_system_parameters")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return false;
    }

    while (query.next()) {
        rec = query.record();
        dPtr_->projectName_ = rec.value("project_name").toString();
        dPtr_->projectDescription_ = rec.value("project_description").toString();
        dPtr_->projectPath_ = rec.value("project_path").toString();
        dPtr_->deviceType_ = rec.value("device_type").toString();
        dPtr_->stationNumber_ = rec.value("station_number").toInt();
        dPtr_->startPage_ = rec.value("start_page").toString();
        dPtr_->stationAddress_ = rec.value("station_address").toString();
        dPtr_->projectEncrypt_ = rec.value("project_encrypt").toInt() > 0 ? true : false;
        dPtr_->pageScanPeriod_ = rec.value("page_scan_period").toInt();
        dPtr_->dataScanPeriod_ = rec.value("data_scan_period").toInt();
    }

    return ret;
}

/**
 * @brief ProjectInfoManager::save
 * @details 保存工程信息
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool ProjectInfoManager::save(ProjectDataSQLiteDatabase *pDB) {
    QSqlQuery query(pDB->db_);
    query.prepare("update t_system_parameters set project_encrypt = :encrypt, "
                  "data_scan_period = :data_scan, device_type = :device, "
                  "page_scan_period = :page_scan, project_description = :desc, "
                  "project_name = :name, project_path = :path, start_page = :start, "
                  "station_address = :address, station_number = :number where id = 1");

    query.bindValue(":encrypt", dPtr_->projectEncrypt_ ? 1 : 0);
    query.bindValue(":data_scan", dPtr_->dataScanPeriod_);
    query.bindValue(":device", dPtr_->deviceType_);
    query.bindValue(":page_scan", dPtr_->pageScanPeriod_);
    query.bindValue(":desc", dPtr_->projectDescription_);
    query.bindValue(":name", dPtr_->projectName_);
    query.bindValue(":path", dPtr_->projectPath_);
    query.bindValue(":start", dPtr_->startPage_);
    query.bindValue(":address", dPtr_->stationAddress_);
    query.bindValue(":number", dPtr_->stationNumber_);

    bool ret = query.exec();
    if(!ret) {
        LogError(QString("update record: %1 failed! %2 ,error: %3!")
                 .arg("t_system_parameters")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    return ret;
}

QString ProjectInfoManager::getProjectName() const {
    return dPtr_->projectName_;
}


void ProjectInfoManager::setProjectName(const QString &name) {
    dPtr_->projectName_ = name;
}

QString ProjectInfoManager::getProjectDescription() const {
    return dPtr_->projectDescription_;
}


void ProjectInfoManager::setProjectDescription(const QString &desc) {
    dPtr_->projectDescription_ = desc;
}


QString ProjectInfoManager::getProjectPath() const {
    return dPtr_->projectPath_;
}

void ProjectInfoManager::setProjectPath(const QString &path) {
    dPtr_->projectPath_ = path;
}

QString ProjectInfoManager::getDeviceType() const {
    return dPtr_->deviceType_;
}

void ProjectInfoManager::setDeviceType(const QString &type){
    dPtr_->deviceType_ = type;
}

int ProjectInfoManager::getStationNumber() const {
    return dPtr_->stationNumber_;
}

void ProjectInfoManager::setStationNumber(int number) {
    dPtr_->stationNumber_ = number;
}

QString ProjectInfoManager::getStartPage() const {
    return dPtr_->startPage_;
}

void ProjectInfoManager::setStartPage(const QString &page){
    dPtr_->startPage_ = page;
}

QString ProjectInfoManager::getStationAddress() const {
    return dPtr_->stationAddress_;
}

void ProjectInfoManager::setStationAddress(const QString &address){
    dPtr_->stationAddress_ = address;
}

bool ProjectInfoManager::getProjectEncrypt() {
    return dPtr_->projectEncrypt_;
}

void ProjectInfoManager::setProjectEncrypt(bool encrypt) {
    dPtr_->projectEncrypt_ = encrypt;
}

int ProjectInfoManager::getPageScanPeriod() const {
    return dPtr_->pageScanPeriod_;
}

void ProjectInfoManager::setPageScanPeriod(int period){
    dPtr_->pageScanPeriod_ = period;
}

int ProjectInfoManager::getDataScanPeriod() const {
    return dPtr_->dataScanPeriod_;
}

void ProjectInfoManager::setDataScanPeriod(int period) {
    dPtr_->dataScanPeriod_ = period;
}

bool getWidthHeight(const QString &input, int &width, int &height) {

    QString str = input;
    int posLeft = str.indexOf("(");
    int posRight = str.indexOf(")");

    if(posLeft == -1 || posRight == -1)
        return false;

    QString strWidthHeight = str.mid(posLeft+1, posRight-posLeft-1);

    QStringList listWidthHeight = strWidthHeight.split('*');

    if(listWidthHeight.size() != 2)
        return false;

    width = listWidthHeight.at(0).toInt();
    height = listWidthHeight.at(1).toInt();

    return true;
}

/**
 * @brief ProjectInfoManager::getGraphPageWidth
 * @return 画面宽度
 */
int ProjectInfoManager::getGraphPageWidth() {
    QString devType = dPtr_->deviceType_;
    int width = 0;
    int height = 0;
    if(getWidthHeight(devType, width, height))
        return width;
    return 0;
}

/**
 * @brief ProjectInfoManager::getGraphPageHeight
 * @return 画面高度
 */
int ProjectInfoManager::getGraphPageHeight() {
    QString devType = dPtr_->deviceType_;
    int width = 0;
    int height = 0;
    if(getWidthHeight(devType, width, height))
        return height;
    return 0;
}

