#include "ChildBase.h"


ChildBase::ChildBase(QWidget *parent,
                     const QString &itemName,
                     const QString &projName) :
    QWidget(parent),
    m_szItemName(itemName),
    m_szProjectName(projName)
{
    setMinimumSize(600, 400);
    this->m_bModifiedFlag = false;
}

ChildBase::~ChildBase()
{

}

QString ChildBase::getProjectName() const
{
    return m_szProjectName;
}

void ChildBase::setProjectName(const QString &s)
{
    m_szProjectName = s;
}

QString ChildBase::getItemName() const
{
    return m_szItemName;
}

void ChildBase::setItemName(const QString &s)
{
    m_szItemName = s;
}

void ChildBase::setModifiedFlag(bool b)
{
    this->m_bModifiedFlag = b;
}

bool ChildBase::getModifiedFlag()
{
    return this->m_bModifiedFlag;
}



