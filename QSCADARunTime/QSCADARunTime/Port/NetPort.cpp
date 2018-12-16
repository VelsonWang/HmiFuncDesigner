
#include <QTime>
#include <QDebug>

#include "NetPort.h"


NetPortThread::NetPortThread()
{
    m_pReadBuf = new unsigned char[1024];
    m_pWriteBuf = new unsigned char[1024];
    m_ReadIndex = 0;
    m_WriteIndex = 0;
    m_pTcpSocket = 0;
    m_pServerIP = 0;
    m_iWaitBytesNum = 0;
    m_bConnected = false;
}


NetPortThread::~NetPortThread()
{
    if(m_pTcpSocket != 0)
    {
        m_pTcpSocket->disconnectFromHost();
        m_pTcpSocket->waitForDisconnected();
        delete m_pTcpSocket;
    }
    if(m_pServerIP != 0)
    {
        delete m_pServerIP;
    }
    delete[] m_pReadBuf;
    delete[] m_pWriteBuf;
}


/*
*
*/
bool NetPortThread::open(QString port, QStringList args)
{
    if(port == "" || args.length() != 2)
        return false;

    m_pTcpSocket = new QTcpSocket();//QTcpSocket(this);
    connect(m_pTcpSocket, SIGNAL(connected()), this, SLOT(SlotConnected()));
    connect(m_pTcpSocket, SIGNAL(disconnected()), this, SLOT(SlotDisconnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(DataReceived()));

    m_iPort = args.at(1).toInt();
    m_pServerIP = new QHostAddress();
    status = false;

    if(!m_pServerIP->setAddress(args.at(0)))
    {
        qDebug() << "server ip address error!";
        return false;
    }
    //qDebug() << *serverIP << port;
    ConnectToHost();
    connect(this, SIGNAL(SignalSockWrite(unsigned char*,int)),SLOT(SlotSockWrite(unsigned char*,int)));
    return true;
}

void NetPortThread::ConnectToHost()
{
    if(m_bConnected)
        return;
    m_pTcpSocket->connectToHost (*m_pServerIP, m_iPort);
    if (m_pTcpSocket->waitForConnected(1000))
    {
        qDebug("wait for connected ok!");
        m_bConnected = true;
    }
}


int NetPortThread::read(unsigned char *buf, int len, int ms)
{
    long start, end;

    m_iWaitBytesNum = len;
    m_ReadIndex = 0;

    QTime time;
    time.start();
    start = end = time.elapsed();

    while(m_ReadIndex < m_iWaitBytesNum)
    {
        end = time.elapsed();
        if((end - start) > ms)
        {
            break;
        }
    }

    for(int i=0; i<m_ReadIndex; i++)
    {
        buf[i] = m_pReadBuf[i];
    }

    return m_ReadIndex;
}

int NetPortThread::write(unsigned char *buf, int len, int /*ms*/)
{
    // QSocketNotifier: Socket notifiers cannot be enabled or disabled from another thread
    // 故此采用信号
    emit SignalSockWrite(buf, len);
    return len;
}


int NetPortThread::SlotSockWrite(unsigned char *buf, int len)
{
    ConnectToHost();
    return m_pTcpSocket->write((char*)buf, len);
}

void NetPortThread::run()
{
    qDebug() << "NetPortThread::run()!";
    while(true)
    {
        this->usleep(500);
    }
}

void NetPortThread::SlotConnected()
{
    qDebug() << "Connected!";
    m_bConnected = true;
}
void NetPortThread::SlotDisconnected()
{
    qDebug() << "Disconnected!";
    m_bConnected = false;
}

void NetPortThread::DataReceived()
{
    qint64 readCnt = 0;
    while(m_pTcpSocket->bytesAvailable()>0)
    {
        readCnt = m_pTcpSocket->read((char*)m_pReadBuf, m_pTcpSocket->bytesAvailable());
        if(readCnt > 0)
        {
            m_ReadIndex += readCnt;
        }
    }
}



///////////////////////////////////////////////////////////


NetPort::NetPort()
{

}


NetPort::~NetPort()
{
    m_NetPortThread.terminate();
    m_NetPortThread.wait();
}


/*
*
*/
bool NetPort::open(QString port, QStringList args)
{
    if(port == "" || args.length() != 2)
        return false;

    m_NetPortThread.open(port, args);
    m_NetPortThread.start();

    return true;
}


int NetPort::read(unsigned char *buf, int len, int ms)
{
    return m_NetPortThread.read(buf, len, ms);
}

int NetPort::write(unsigned char *buf, int len, int ms)
{
    return m_NetPortThread.write(buf, len, ms);
}


bool NetPort::close()
{

    return true;
}



