
#include <QTime>
#include <QDebug>

#include "ComPort.h"

#define TIME_OUT 10

ComPort::ComPort()
{
    m_pSerialPort = NULL;
    m_pReadBuf = new unsigned char[1024];
    m_pWriteBuf = new unsigned char[1024];
    m_ReadIndex = 0;
    m_WriteIndex = 0;
    m_iWaitBytesNum = 0;
}


ComPort::~ComPort()
{
    close();
    delete[] m_pReadBuf;
    delete[] m_pWriteBuf;
    if(m_pSerialPort != NULL)
        delete m_pSerialPort;

}


/*
* port: "ttyUSBx", "ttySx", "COMx"
* args: ["9600","N","8","1"]
*/
bool ComPort::open(QString port, QStringList args)
{
    if(port == "" || args.length() != 4)
        return false;

#ifdef Q_OS_LINUX
    m_pSerialPort = new QextSerialPort("/dev/" + port);
#elif defined (Q_OS_WIN)
    m_pSerialPort = new QextSerialPort(port);
#endif

    //设置波特率
    QString strBaud = args.at(0);
    m_pSerialPort->setBaudRate((BaudRateType)strBaud.toInt());

    //设置数据位
    QString strDataBits = args.at(1);
    m_pSerialPort->setDataBits((DataBitsType)strDataBits.toInt());

    //设置校验
    QString strParity = args.at(2);
    if(strParity == "N")
    {
        m_pSerialPort->setParity(PAR_NONE);
    }
    else if(strParity == "O")
    {
        m_pSerialPort->setParity(PAR_ODD);
    }
    else if(strParity == "E")
    {
        m_pSerialPort->setParity(PAR_EVEN);
    }
    else
    {
        m_pSerialPort->setParity(PAR_NONE);
    }

    //设置停止位
    QString strStopBits = args.at(3);
    switch(strStopBits.toInt()){
    case 0:
        m_pSerialPort->setStopBits(STOP_1);
        break;
    case 1:
 #ifdef Q_OS_WIN
        m_pSerialPort->setStopBits(STOP_1_5);
 #endif
        break;
    case 2:
        m_pSerialPort->setStopBits(STOP_2);
        break;
    default:
        m_pSerialPort->setStopBits(STOP_1);
        break;
    }

    //设置数据流控制
    m_pSerialPort->setFlowControl(FLOW_OFF);
    //设置延时
    m_pSerialPort->setTimeout(TIME_OUT);

    if(!m_pSerialPort->open(QIODevice::ReadWrite))
    {
        return false;
    }

    return true;
}


int ComPort::read(unsigned char *buf, int len, int ms)
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
        if(m_pSerialPort->bytesAvailable())
        {
            QByteArray byteArray = m_pSerialPort->readAll();
            int count = byteArray.count();

            for(int i = 0; i < count; i++)
            {
                m_pReadBuf[m_ReadIndex++] = (unsigned char)byteArray.at(i);
            }
        }

        if((end - start) > ms)
        {
            return m_ReadIndex;
        }
    }

    for(int i=0; i<len; i++)
    {
        buf[i] = m_pReadBuf[i];
    }

#if 0
    QString strReadBuf;
    for(int i=0; i<len; i++)
    {
        QString s;
        s.sprintf("0x%02x ", m_pReadBuf[i]);
        strReadBuf += s;
    }
    qDebug()<< "read: " << strReadBuf;
#endif
    return len;
}

int ComPort::write(unsigned char *buf, int len, int /*ms*/)
{
    int count = 0;
#if 0
    QString strReadBuf;
    for(int i=0; i<len; i++)
    {
        QString s;
        s.sprintf("0x%02x ", buf[i]);
        strReadBuf += s;
    }
    qDebug()<< "write: " << strReadBuf;
#endif
    count = m_pSerialPort->write((char*)buf, len);
    return count;
}


bool ComPort::close()
{
    if(m_pSerialPort != NULL)
    {
        if(m_pSerialPort->isOpen())
        {
            m_pSerialPort->close();
            return true;
        }
    }
    return false;
}







