#include "ElementIDHelper.h"
#include "DrawListUtils.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>

QString ElementIDHelper::szProjectPath_ = QString();

ElementIDHelper::ElementIDHelper(QObject *parent)
    : QObject(parent) {

}

ElementIDHelper::~ElementIDHelper() {

}


/**
 * @brief TagManager::setProjectPath
 * @details 设置工程路径
 * @param path 工程路径
 */
void ElementIDHelper::setProjectPath(const QString &path) {
    if(szProjectPath_ != path) {
        szProjectPath_ = path;
    }
}


/**
 * @brief TagManager::getProjectPath
 * @details 取得工程路径
 * @return 工程路径
 */
QString ElementIDHelper::getProjectPath() {
    return szProjectPath_;
}


/**
 * @brief ElementIDHelper::getAllElementIDName
 * @details 获取工程所有控件的ID名称
 * @param proj_path
 * @param idList
 */
void ElementIDHelper::getAllElementIDName(const QString &proj_path, QStringList &idList) {
    szProjectPath_ = proj_path;
    DrawListUtils::loadDrawList(szProjectPath_);
    for(int i=0; i<DrawListUtils::drawList_.count(); i++) {
        QString szFileName = szProjectPath_ + "/" + DrawListUtils::drawList_.at(i) + ".drw";
        QXmlStreamReader reader;
        QFile file(szFileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0,
                                     tr("错误"),
                                     QString(tr("无法打开文件:%1")).arg(szFileName),
                                     QMessageBox::Ok);
            return;
        }
        reader.setDevice(&file);

        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartDocument) {
                continue;
            }

            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == "graphPage") {
                    QString szPageId = "";
                    if (reader.attributes().hasAttribute("graphPageId")) {
                        szPageId = reader.attributes().value("graphPageId").toString();
                    }
                    reader.readNext();
                    while (!(reader.tokenType() == QXmlStreamReader::EndElement &&
                             reader.name() == "graphPage")) {
                        if (reader.tokenType() == QXmlStreamReader::StartElement) {
                            if (reader.name() == "element") {
                                if (reader.attributes().hasAttribute("elementId")) {
                                    QString szID = reader.attributes().value("elementId").toString();
                                    if(szID != "") {
                                        idList.append(szPageId + "." + szID);
                                    }
                                }
                            }
                        }
                        reader.readNext();
                    }
                }
            }
        }
        file.close();
    }
}
