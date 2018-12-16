#include "ChildBase.h"


ChildBase::ChildBase(QWidget *parent, QString itemName, QString projName) :
    QWidget(parent),
    m_strItemName(itemName),
    m_strProjectName(projName)
{
    setMinimumSize(600,400);
    this->m_bModifiedFlag = false;
}
ChildBase::~ChildBase()
{

}

QString ChildBase::GetItemProjectName()
{
    return m_strProjectName;
}

void ChildBase::SetItemProjectName(QString s)
{
    m_strProjectName = s;
}

QString ChildBase::GetItemName()
{
    return m_strItemName;
}



void ChildBase::SetItemName(QString s)
{
    m_strItemName = s;
}


void ChildBase::SetModifiedFlag(bool b)
{
    this->m_bModifiedFlag = b;
}

bool ChildBase::GetModifiedFlag()
{
    return this->m_bModifiedFlag;
}



