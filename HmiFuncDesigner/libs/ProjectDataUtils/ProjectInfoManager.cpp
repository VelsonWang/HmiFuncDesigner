#include "ProjectInfoManager.h"

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



ProjectInfoManager::ProjectInfoManager() : dPtr_(new ProjectInfoPrivate()) {

}

ProjectInfoManager::~ProjectInfoManager() {
    if(dPtr_ != Q_NULLPTR) {
        delete dPtr_;
    }
}

/**
 * @brief ProjectInfoManager::openFromXml
 * @details 读取工程信息
 * @param pXmlObj
 * @return true-成功, false-失败
 */
bool ProjectInfoManager::openFromXml(XMLObject *pXmlObj) {
    XMLObject *projInfoObj = pXmlObj->getCurrentChild("project_info");
    dPtr_->projectEncrypt_ = projInfoObj->getProperty("encrypt") == "1";
    dPtr_->dataScanPeriod_= projInfoObj->getProperty("data_scan").toUInt();
    dPtr_->deviceType_ = projInfoObj->getProperty("device");
    dPtr_->pageScanPeriod_= projInfoObj->getProperty("page_scan").toUInt();
    dPtr_->projectDescription_= projInfoObj->getProperty("desc");
    dPtr_->projectName_= projInfoObj->getProperty("name");
    dPtr_->projectPath_= projInfoObj->getProperty("path");
    dPtr_->startPage_ = projInfoObj->getProperty("start");
    dPtr_->stationAddress_ = projInfoObj->getProperty("address");
    dPtr_->stationNumber_ = projInfoObj->getProperty("number").toInt();
    return true;
}

/**
 * @brief ProjectInfoManager::saveToXml
 * @details 保存工程信息
 * @param pXmlObj
 * @return true-成功, false-失败
 */
bool ProjectInfoManager::saveToXml(XMLObject *pXmlObj) {
    XMLObject *projInfoObj = new XMLObject(pXmlObj);
    projInfoObj->setTagName("project_info");
    projInfoObj->setProperty("encrypt", dPtr_->projectEncrypt_ ? "1" : "0");
    projInfoObj->setProperty("data_scan", QString::number(dPtr_->dataScanPeriod_));
    projInfoObj->setProperty("device", dPtr_->deviceType_);
    projInfoObj->setProperty("page_scan", QString::number(dPtr_->pageScanPeriod_));
    projInfoObj->setProperty("desc", dPtr_->projectDescription_);
    projInfoObj->setProperty("name", dPtr_->projectName_);
    projInfoObj->setProperty("path", dPtr_->projectPath_);
    projInfoObj->setProperty("start", dPtr_->startPage_);
    projInfoObj->setProperty("address", dPtr_->stationAddress_);
    projInfoObj->setProperty("number", QString::number(dPtr_->stationNumber_));
    return true;
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

