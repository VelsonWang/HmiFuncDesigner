
#include <QTime>
#include <QDebug>

#include "NetPort.h"


NetPortThread::NetPortThread()
{
    buf_.clear();
    tcpSocketPtr_ = 0;
    serverIPPtr_ = 0;
    bConnected_ = false;
}


NetPortThread::~NetPortThread()
{
    buf_.clear();
    if(tcpSocketPtr_ != 0)
    {
        tcpSocketPtr_->disconnectFromHost();
        tcpSocketPtr_->waitForDisconnected();
        delete tcpSocketPtr_;
    }
    if(serverIPPtr_ != 0)
    {
        delete serverIPPtr_;
    }
}


bool NetPortThread::open(QString port, QStringList args)
{
    if(port == "" || args.length() != 2)
        return false;

    tcpSocketPtr_ = new QTcpSocket();
    connect(tcpSocketPtr_, SIGNAL(connected()), this, SLOT(SlotConnected()));
    connect(tcpSocketPtr_, SIGNAL(disconnected()), this, SLOT(SlotDisconnected()));
    connect(tcpSocketPtr_, SIGNAL(readyRead()), this, SLOT(DataReceived()));

    port_ = args.at(1).toInt();
    serverIPPtr_ = new QHostAddress();
    status_ = false;

    if(!serverIPPtr_->setAddress(args.at(0)))
    {
        qDebug() << "server ip address error!";
        return false;
    }
    //qDebug() << *serverIP << port;
    ConnectToHost();
    connect(this, SIGNAL(SignalSockWrite(unsigned char*,int)), SLOT(SlotSockWrite(unsigned char*,int)));
    return true;
}

void NetPortThread::ConnectToHost()
{
    if(bConnected_)
        return;
    tcpSocketPtr_->connectToHost (*serverIPPtr_, port_);
    if (tcpSocketPtr_->waitForConnected(1000))
    {
        qDebug("wait for connected ok!");
        bConnected_ = true;
    }
}


int NetPortThread::read(unsigned char *buf, int len, int ms)
{
    long start;

    QTime time;
    time.start();
    start = time.elapsed();

    while(buf_.size() < len)
    {
        if((time.elapsed() - start) > ms)
        {
            if(len > buf_.size()) len = buf_.size();
            break;
        }
    }

    for(int i=0; i<len; i++)
    {
        buf[i] = buf_[i];
    }
    buf_.remove(0, len);

    return len;
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
    return tcpSocketPtr_->write((char*)buf, len);
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
    bConnected_ = true;
}
void NetPortThread::SlotDisconnected()
{
    qDebug() << "Disconnected!";
    bConnected_ = false;
}

void NetPortThread::DataReceived()
{
    while(tcpSocketPtr_->bytesAvailable()>0)
    {
        buf_.append(tcpSocketPtr_->readAll());
    }
}



///////////////////////////////////////////////////////////


NetPort::NetPort()
{

}


NetPort::~NetPort()
{
    netPortThread_.terminate();
    netPortThread_.wait();
}


/*
*
*/
bool NetPort::open(QString port, QStringList args)
{
    if(port == "" || args.length() != 2)
        return false;

    netPortThread_.open(port, args);
    netPortThread_.start();

    return true;
}


int NetPort::read(unsigned char *buf, int len, int ms)
{
    return netPortThread_.read(buf, len, ms);
}

int NetPort::write(unsigned char *buf, int len, int ms)
{
    return netPortThread_.write(buf, len, ms);
}


bool NetPort::close()
{
    return true;
}



