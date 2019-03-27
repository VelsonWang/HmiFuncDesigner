#include "DrawListUtils.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QTextStream>
#include <QTextCodec>
#include <QDomAttr>
#include <QDebug>


QStringList DrawListUtils::drawList_;


DrawListUtils::DrawListUtils() {
    drawList_.clear();
}


/**
 * @brief DrawListUtils::loadDrawList
 * @details 加载画面名称列表
 * @param path 工程路径
 */
void DrawListUtils::loadDrawList(const QString &path) {

    drawList_.clear();

    QFile file( path + "/DrawList.xml" );
    QDomDocument doc;

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qDebug("Failed to open file for reading.");
        return;
    }

    if(!doc.setContent(&file)) {
        qDebug("Failed to parse the file into a DOM tree.");
        file.close();
        return;
    }

    QDomElement docElement = doc.documentElement();  // Returns the root element of the document
    QDomNode node = docElement.firstChild();
    while(!node.isNull()) {
        if(node.isElement()) {
            QDomElement element = node.toElement();
            if(element.tagName() == "DRAW") {
                QString strAttribute = element.attribute("name", "");
                if(strAttribute != "") {
                    drawList_ << strAttribute;
                }
            }
        }
        node = node.nextSibling();
    }

    file.close();
}



/**
 * @brief DrawListUtils::saveDrawList
 * @details 保存画面名称列表
 * @param path 工程路径
 */
void DrawListUtils::saveDrawList(const QString &path) {
    QFile file(path + "/DrawList.xml");
    QDomDocument doc;

    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Can't create file!";
        return;
    }

    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\'1.0\'");
    doc.appendChild(instruction);

    QDomElement drawList = doc.createElement("List");

    for(int i=0; i<drawList_.count(); i++) {
        QDomElement element = doc.createElement("DRAW");
        QDomAttr attrType = doc.createAttribute("type");
        attrType.setValue("DRAW");
        QDomAttr name = doc.createAttribute("name");
        name.setValue(drawList_.at(i));
        element.setAttributeNode(attrType);
        element.setAttributeNode(name);
        drawList.appendChild(element);
    }
    doc.appendChild(drawList);

    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    doc.save(out, 4);
    file.close();
}


/**
 * @brief DrawListUtils::getMaxDrawPageNum
 * @details 获取画面缺省名最大数值
 * @param name 名称
 */
int DrawListUtils::getMaxDrawPageNum(const QString &name)
{
    int ret = 0;

    for(int i=0; i<drawList_.count(); i++) {
        QString strDrawPageName = drawList_.at(i);
        if(strDrawPageName.contains(name)) {
            QString strNum = "";
            if(strDrawPageName.length() > name.length())
                strNum = strDrawPageName.right(strDrawPageName.length() - name.length());
            bool ok = false;
            int dec = strNum.toInt(&ok, 10);
            if(ok) {
                if(ret < dec)
                    ret = dec;
            }
        }
    }
    ret++;

    return ret;
}

