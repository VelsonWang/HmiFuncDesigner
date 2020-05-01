#include "tcpserver.h"
#include "threadhandle.h"
#include "MessageTransfer.h"

TcpServer::TcpServer(QObject *parent, int numConnections) :
    QTcpServer(parent)
{
     tcpClient = new  QHash<int,TcpSocket *>;
     setMaxPendingConnections(numConnections);
}

TcpServer::~TcpServer()
{
    emit this->sentDisConnect(-1);
    delete tcpClient;
}

void TcpServer::setMaxPendingConnections(int numConnections)
{
    this->QTcpServer::setMaxPendingConnections(numConnections); // 调用Qtcpsocket函数，设置最大连接数，主要是使maxPendingConnections()依然有效
    this->maxConnections = numConnections;
}

void TcpServer::incomingConnection(qintptr socketDescriptor) //多线程必须在此函数里捕获新连接
{
    if (tcpClient->size() > maxPendingConnections())//继承重写此函数后，QTcpServer默认的判断最大连接数失效，自己实现
    {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);
        tcp.disconnectFromHost();
        return;
    }
    auto th = ThreadHandle::getClass().getThread();
    auto tcpTemp = new TcpSocket(socketDescriptor);
    QString ip =  tcpTemp->peerAddress().toString();
    qint16 port = tcpTemp->peerPort();

    connect(tcpTemp, &TcpSocket::sockDisConnect, this, &TcpServer::sockDisConnectSlot);//NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是考的他
    connect(this, &TcpServer::sentDisConnect, tcpTemp, &TcpSocket::disConTcp);//断开信号
    connect(tcpTemp, &TcpSocket::reStartRuntime, this, &TcpServer::reStartRuntime);

    tcpTemp->moveToThread(th);//把tcp类移动到新的线程，从线程管理类中获取
    tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中
    emit connectClient(socketDescriptor,ip,port);
}

void TcpServer::sockDisConnectSlot(int handle,const QString & ip, quint16 prot,QThread * th)
{
    tcpClient->remove(handle);//连接管理中移除断开连接的socket
    ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了
    emit sockDisConnect(handle,ip,prot);
}


void TcpServer::clear()
{
    emit this->sentDisConnect(-1);
    ThreadHandle::getClass().clear();
    tcpClient->clear();
}

void TcpServer::reStartRuntime()
{
    MessageTransfer client;
    client.sendMessage("download_project", 1000);
}
