#include "qresourceregister.h"

#include "qrccmaker.h"
#include "qresourcefile.h"

#include <QResource>
#include <QFile>

uchar*  QResourceRegister::m_data=NULL;

bool QResourceRegister::reg(QResourceFile *file)
{
    unreg();
    QRCCMaker rcc;
    rcc.setResourceFile(file);
    QByteArray array=rcc.rccData();

    m_data=new uchar[array.size()];
    memcpy(m_data,array.data(),array.size());
    if(!QResource::registerResource(m_data))
    {
        delete m_data;
        m_data=NULL;
        return false;
    }
    return true;
}

void QResourceRegister::unreg()
{
    if(m_data!=NULL)
    {
        QResource::unregisterResource(m_data);
        delete m_data;
        m_data=NULL;
    }
}
