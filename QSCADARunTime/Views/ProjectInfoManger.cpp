#include "ProjectInfoManger.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

class ProjectInfoManger;

class ProjectInfoPrivate
{
public:
    ProjectInfoPrivate(ProjectInfoManger *parent) { }

    QString projectName_;
    QString projectDescription_;
    QString projectPath_;
    QString deviceType_;
    QString stationNumber_;
    QString startPage_;
    QString stationAddress_;
    bool projectEncrypt_;
    QString pageScanPeriod_;
    QString dataScanPeriod_;
};



ProjectInfoManger::ProjectInfoManger()
    : dPtr_(new ProjectInfoPrivate(this)) {

}

ProjectInfoManger::~ProjectInfoManger() {
    delete dPtr_;
}

void ProjectInfoManger::load(const QJsonObject &json)
{
    dPtr_->projectName_ = json["sProjectName"].toString();
    dPtr_->projectDescription_ = json["sProjectDescription"].toString();
    dPtr_->projectPath_ = json["sProjectPath"].toString();
    dPtr_->deviceType_ = json["sDeviceType"].toString();
    dPtr_->stationNumber_ = json["sStationNumber"].toString();
    dPtr_->startPage_ = json["sStartPage"].toString();
    dPtr_->stationAddress_ = json["sStationAddress"].toString();
    dPtr_->projectEncrypt_ = json["bProjectEncrypt"].toBool();
    dPtr_->pageScanPeriod_ = json["sPageScanPeriod"].toString();
    dPtr_->dataScanPeriod_ = json["sDataScanPeriod"].toString();
}

void ProjectInfoManger::save(QJsonObject &json)
{
    json["sProjectName"] = dPtr_->projectName_;
    json["sProjectDescription"] = dPtr_->projectDescription_;
    json["sProjectPath"] = dPtr_->projectPath_;
    json["sDeviceType"] = dPtr_->deviceType_;
    json["sStationNumber"] = dPtr_->stationNumber_;
    json["sStartPage"] = dPtr_->startPage_;
    json["sStationAddress"] = dPtr_->stationAddress_;
    json["bProjectEncrypt"] = dPtr_->projectEncrypt_;
    json["sPageScanPeriod"] = dPtr_->pageScanPeriod_;
    json["sDataScanPeriod"] = dPtr_->dataScanPeriod_;
}

/**
 * @brief ProjectInfoManger::loadFromFile
 * @details 读取工程信息
 * @param saveFormat 保存格式
 * @param file 工程路径
 * @return true-成功, false-失败
 */
bool ProjectInfoManger::loadFromFile(SaveFormat saveFormat, const QString &file)
{
    QFile loadFile(file);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file.");
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(saveData) : QJsonDocument::fromBinaryData(saveData));
    load(loadDoc.object());
    loadFile.close();
    return true;
}

/**
 * @brief ProjectInfoManger::saveToFile
 * @details 保存工程信息
 * @param projPath 工程保存路径
 * @param projName 工程名称
 * @param saveFormat 保存格式
 * @return true-成功, false-失败
 */
bool ProjectInfoManger::saveToFile(const QString &projPath,
                                   const QString &projName,
                                   SaveFormat saveFormat)
{
    QString strDir =  projPath;
    QDir dir(strDir);
    dir.mkpath(strDir + "/" + projName);
    QString file = strDir + "/" + projName + "/" + projName + ".proj";
    QFile saveFile(file);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject obj;
    save(obj);
    QJsonDocument saveDoc(obj);
    saveFile.write(saveFormat == Json ? saveDoc.toJson() : saveDoc.toBinaryData());
    saveFile.close();
    return true;
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

QString ProjectInfoManger::getStationNumber() const {
    return dPtr_->stationNumber_;
}

void ProjectInfoManger::setStationNumber(const QString &number) {
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

QString ProjectInfoManger::getPageScanPeriod() const {
    return dPtr_->pageScanPeriod_;
}

void ProjectInfoManger::setPageScanPeriod(const QString &period){
    dPtr_->pageScanPeriod_ = period;
}

QString ProjectInfoManger::getDataScanPeriod() const {
    return dPtr_->dataScanPeriod_;
}

void ProjectInfoManger::setDataScanPeriod(const QString &period) {
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

