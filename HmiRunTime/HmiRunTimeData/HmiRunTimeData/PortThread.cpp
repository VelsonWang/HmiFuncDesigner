#include "PortThread.h"
#include <RealTimeDB.h>
#include <QDebug>

PortThread::PortThread(QString name)
{
    m_name = name;
    mbIsRunning = false;
    state = __Thread_Stop;
}

PortThread::~PortThread()
{

}

void PortThread::AddVendor(Vendor *pVendor)
{
    m_VendorList.append(pVendor);
}

/*
* 是否运行
*/
bool PortThread::IsRunning()
{
    return mbIsRunning;
}


/*
* 启动
*/
void PortThread::Start()
{
    if(mbIsRunning)
        return;
    mbIsRunning = true;
    foreach (Vendor *pVendor, m_VendorList) {
        if(pVendor != Q_NULLPTR) {
            if(!pVendor->isRunning()) {
                pVendor->open();
                pVendor->initialize();
                pVendor->start();
            }
        }
    }
   	state = __Thread_Runing; 
    this->start();
}


/*
* 停止
*/
void PortThread::Stop()
{
    mbIsRunning = false;
    foreach (Vendor *pVendor, m_VendorList) {
        if(pVendor != Q_NULLPTR) {
            if(pVendor->isRunning()) {
                pVendor->stop();
                pVendor->close();
            }
        }
    }
    while(state != __Thread_Stop)
    {
        this->msleep(5);
    }
    this->wait();
}


void PortThread::run()
{
    qDebug()<< "PortThread run:" << m_name;
    while(mbIsRunning) {
        RealTimeDB::instance()->addNeedWriteTagToDeviceWriteQueue();
        foreach (Vendor *pVendor, m_VendorList) {
            if(!mbIsRunning)
                break;
            if(pVendor != Q_NULLPTR) {
                pVendor->doLoop();
            }
        }
        this->msleep(5);
    }
    state = __Thread_Stop;
}

