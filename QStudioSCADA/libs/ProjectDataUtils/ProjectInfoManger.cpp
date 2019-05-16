#include "ProjectInfoManger.h"
#include "ProjectDataSQLiteDatabase.h"
#include "ulog.h"
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

class ProjectInfoManger;

class ProjectInfoPrivate
{
public:
    ProjectInfoPrivate(ProjectInfoManger *parent) { }

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



ProjectInfoManger::ProjectInfoManger()
    : dPtr_(new ProjectInfoPrivate(this)) {

}

ProjectInfoManger::~ProjectInfoManger() {
    delete dPtr_;
}

/**
 * @brief ProjectInfoManger::load
 * @details 读取工程信息
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool ProjectInfoManger::load(ProjectDataSQLiteDatabase *pDB) {
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
 * @brief ProjectInfoManger::save
 * @details 保存工程信息
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool ProjectInfoManger::save(ProjectDataSQLiteDatabase *pDB) {
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

QString ProjectInfoManger::getProjectName() const {
    return dPtr_->projectName_;
}


void ProjectInfoManger::setProjectName(const QString &name) {
    dPtr_->projectName_ = name;
}

QString ProjectInfoManger::getProjectDescription() const {
    return dPtr_->projectDescription_;
}


void ProjectInfoManger::setProjectDescription(const QString &desc) {
    dPtr_->projectDescription_ = desc;
}


QString ProjectInfoManger::getProjectPath() const {
    return dPtr_->projectPath_;
}

void ProjectInfoManger::setProjectPath(const QString &path) {
    dPtr_->projectPath_ = path;
}

QString ProjectInfoManger::getDeviceType() const {
    return dPtr_->deviceType_;
}

void ProjectInfoManger::setDeviceType(const QString &type){
    dPtr_->deviceType_ = type;
}

int ProjectInfoManger::getStationNumber() const {
    return dPtr_->stationNumber_;
}

void ProjectInfoManger::setStationNumber(int number) {
    dPtr_->stationNumber_ = number;
}

QString ProjectInfoManger::getStartPage() const {
    return dPtr_->startPage_;
}

void ProjectInfoManger::setStartPage(const QString &page){
    dPtr_->startPage_ = page;
}

QString ProjectInfoManger::getStationAddress() const {
    return dPtr_->stationAddress_;
}

void ProjectInfoManger::setStationAddress(const QString &address){
    dPtr_->stationAddress_ = address;
}

bool ProjectInfoManger::getProjectEncrypt() {
    return dPtr_->projectEncrypt_;
}

void ProjectInfoManger::setProjectEncrypt(bool encrypt) {
    dPtr_->projectEncrypt_ = encrypt;
}

int ProjectInfoManger::getPageScanPeriod() const {
    return dPtr_->pageScanPeriod_;
}

void ProjectInfoManger::setPageScanPeriod(int period){
    dPtr_->pageScanPeriod_ = period;
}

int ProjectInfoManger::getDataScanPeriod() const {
    return dPtr_->dataScanPeriod_;
}

void ProjectInfoManger::setDataScanPeriod(int period) {
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
 * @brief ProjectInfoManger::getGraphPageWidth
 * @return 画面宽度
 */
int ProjectInfoManger::getGraphPageWidth() {
    QString devType = dPtr_->deviceType_;
    int width = 0;
    int height = 0;
    if(getWidthHeight(devType, width, height))
        return width;
    return 0;
}

/**
 * @brief ProjectInfoManger::getGraphPageHeight
 * @return 画面高度
 */
int ProjectInfoManger::getGraphPageHeight() {
    QString devType = dPtr_->deviceType_;
    int width = 0;
    int height = 0;
    if(getWidthHeight(devType, width, height))
        return height;
    return 0;
}

