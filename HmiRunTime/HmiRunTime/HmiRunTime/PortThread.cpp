#include "PortThread.h"
#include "realtimedb.h"
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
    if(mbIsRunning) {
        return;
    }
    mbIsRunning = true;
    this->start();
}


/*
* 停止
*/
void PortThread::Stop()
{
    mbIsRunning = false;
    foreach (Vendor *pVendor, m_VendorList) {
        if(pVendor != NULL) {
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

    foreach (Vendor *pVendor, m_VendorList) {
        if(pVendor != NULL) {
            if(!pVendor->isRunning()) {
                pVendor->open();
                pVendor->initialize();
                pVendor->start();
            }
        }
    }
    state = __Thread_Runing;

    while(mbIsRunning) {
        foreach (Vendor *pVendor, m_VendorList) {
            if(!mbIsRunning)
                break;
            if(pVendor != Q_NULLPTR) {
                pVendor->doLoop();
            }
        }
        this->msleep(5);
    }
    qDebug()<< "PortThread run end.";
    state = __Thread_Stop;
}

