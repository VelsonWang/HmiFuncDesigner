#include "ChildBase.h"


ChildBase::ChildBase(QWidget *parent,
                     const QString &itemName,
                     const QString &projName) :
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

QString ChildBase::getProjectName() const
{
    return m_strProjectName;
}

void ChildBase::setProjectName(const QString &s)
{
    m_strProjectName = s;
}

QString ChildBase::getItemName() const
{
    return m_strItemName;
}

void ChildBase::setItemName(const QString &s)
{
    m_strItemName = s;
}

void ChildBase::setModifiedFlag(bool b)
{
    this->m_bModifiedFlag = b;
}

bool ChildBase::getModifiedFlag()
{
    return this->m_bModifiedFlag;
}



