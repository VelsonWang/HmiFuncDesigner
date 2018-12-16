#include <QJsonArray>
#include "GraphPage.h"
#include <QDebug>


GraphPage::GraphPage()
{

}

/*
* 添加一个图形元素
*/
void GraphPage::AddGraphElement(StdElement *e)
{
    this->m_ElementList.append(e);
}


/*
* 删除一个图形元素
*/
void GraphPage::DelGraphElement(StdElement *e)
{
    this->m_ElementList.removeOne(e);
}


/*
* 加载元素
*/
void GraphPage::load(const QJsonObject &json)
{
    // do nothing
}

/*
* 保存元素
*/
void GraphPage::save(QJsonObject &json)
{
    QJsonArray elementArray;
    for(int i=0; i<this->m_ElementList.size(); i++)
    {
        IElement *pIEle = this->m_ElementList.at(i)->GetInterface();
        QJsonObject jsonobj;
        pIEle->save(jsonobj);
        elementArray.append(jsonobj);
    }
    json["elements"] = elementArray;
}





