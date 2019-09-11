#include "PortThread.h"

#include <QDebug>


PortThread::PortThread(QString name)
{
    m_name = name;
    mbIsRunning = false;
}

PortThread::~PortThread()
{

}

void PortThread::AddVendor(IVendor *pVendor)
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
    foreach (IVendor *pVendor, m_VendorList) {
        if(pVendor != Q_NULLPTR) {
            //pVendor->SetOwnThread(this);
            if(!pVendor->IsRunning()) {
                pVendor->Open();
                pVendor->Initialize();
                pVendor->Start();
            }
        }
    }
    this->start();
}


/*
* 停止
*/
void PortThread::Stop()
{
    mbIsRunning = false;
    foreach (IVendor *pVendor, m_VendorList) {
        if(pVendor != Q_NULLPTR) {
            if(pVendor->IsRunning()) {
                pVendor->Stop();
                pVendor->Close();
            }
        }
    }
    this->wait();
}


void PortThread::run()
{
    qDebug()<< "PortThread run:" << m_name;
    while(mbIsRunning) {
        foreach (IVendor *pVendor, m_VendorList) {
            if(!mbIsRunning)
                return;
            if(pVendor != Q_NULLPTR) {
                pVendor->DoLoop();
            }
        }
        this->msleep(5);
    }
}

