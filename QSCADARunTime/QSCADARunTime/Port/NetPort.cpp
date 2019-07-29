#include <QTime>
#include "NetPort.h"
#include "Public/PublicFunction.h"

NetPort::NetPort() {
    //init only required for windows, no-op on *nix
    net::init();
}


NetPort::~NetPort() {

}


/*
*
*/
bool NetPort::open(QString port, QStringList args) {
    if(port == "" || args.length() != 2)
        return false;

    ip_ = args.at(0);
    port_ = args.at(1).toInt();

    //inetV4 socket
    sock_.init(net::af::inet, net::sock::stream);

    //make sure socket creation and binding did not fail
    if( !sock_.good() )	{
        std::cerr << "error creating socket" << std::endl;
        return false;
    }

    endpoint_.set(ip_.toStdString(), port_, net::af::inet);
    sock_.setnonblocking(true);
    sock_.connect(endpoint_);

    return true;
}


int NetPort::read(unsigned char *buf, int len, int ms) {
    long start;

    QTime time;
    time.start();
    start = time.elapsed();
    QByteArray byteArray;
    char tmpBuf[512] = {0};
    int readLen = 0;
    int iLen = 0;

    while(byteArray.size() < len) {
        iLen = (int)sock_.recv(tmpBuf, (std::size_t)len);
        byteArray.append((const char *)tmpBuf, iLen);
        readLen += iLen;

        if((time.elapsed() - start) > ms) {
            if(len > byteArray.size())
                len = byteArray.size();
            break;
        }
    }

    for(int i=0; i<len; i++) {
        buf[i] = byteArray[i];
    }

#if 0
    qDebug()<< "read: " << hexToString(byteArray.data(), len);
#endif

    return len;
}

int NetPort::write(unsigned char *buf, int len, int ms) {
#if 0
    qDebug()<< "write: " << hexToString((char *)buf, len);
#endif
    std::size_t sendLen = sock_.send((const char* )buf, (std::size_t)len);
    return (int)sendLen;
}


bool NetPort::close() {
    return true;
}


