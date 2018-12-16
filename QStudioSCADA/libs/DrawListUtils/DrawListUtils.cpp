
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QTextStream>
#include <QDomAttr>
#include <QDebug>


#include "DrawListUtils.h"





DrawListUtils::DrawListUtils()
{
}



/*
* 加载画面名称列表
*/
void DrawListUtils::LoadDrawList(QString path, QStringList &slist)
{
    QFile file( path + "/DrawList.xml" );
    QDomDocument doc;

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug("Failed to open file for reading.");
        return;
    }

    if(!doc.setContent(&file))
    {
        qDebug("Failed to parse the file into a DOM tree.");
        file.close();
        return;
    }

    QDomElement docElement = doc.documentElement();  // Returns the root element of the document
    QDomNode node = docElement.firstChild();
    while(!node.isNull())
    {
        if(node.isElement())
        {
            QDomElement element = node.toElement();
            if(element.tagName() == "DRAW")
            {
                QString strAttribute = element.attribute("name", "");
                if(strAttribute != "")
                {
                    slist << strAttribute;
                }
            }
        }
        node = node.nextSibling();
    }

    file.close();
}



/*
* 保存画面名称列表
*/
void DrawListUtils::SaveDrawList(QString path, QStringList &slist)
{
    QFile file(path + "/DrawList.xml");
    QDomDocument doc;

    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "Can't create file!";
        return;
    }

    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\'1.0\'");
    doc.appendChild(instruction);

    QDomElement drawList = doc.createElement("List");

    for(int i=0; i<slist.count(); i++)
    {
        QDomElement element = doc.createElement("DRAW");
        QDomAttr attrType = doc.createAttribute("type");
        attrType.setValue("DRAW");
        QDomAttr name = doc.createAttribute("name");
        name.setValue(slist.at(i));
//        qDebug() << slist.at(i);
        element.setAttributeNode(attrType);
        element.setAttributeNode(name);
        drawList.appendChild(element);
    }
    doc.appendChild(drawList);

    QTextStream out(&file);
    doc.save(out, 4);
    file.close();
}

/*
* 获取画面缺省名最大数值
*/
int DrawListUtils::GetMaxDrawPageNum(QString name, QStringList &slist)
{
    int ret = 0;

    for(int i=0; i<slist.count(); i++)
    {
        QString strDrawPageName = slist.at(i);
        if(strDrawPageName.contains(name))
        {
            QString strNum = "";
            if(strDrawPageName.length() > name.length())
                strNum = strDrawPageName.right(strDrawPageName.length() - name.length());
            bool ok = false;
            int dec = strNum.toInt(&ok, 10);
            if(ok)
            {
                if(ret < dec)
                    ret = dec;
            }
        }
    }
    ret++;

    return ret;
}

