#ifndef PUBLIC_FUNCTION_H
#define PUBLIC_FUNCTION_H

#include <QString>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>

inline void RecoverData(unsigned char* psrc, unsigned char* pdst, int len)
{
    unsigned char i;
    for(i = 0; i < len; i ++)
    {
        pdst[len - i - 1] = psrc[i];
    }
}

inline void RecoverSelfData(unsigned char* pdat, int len)
{
    unsigned char i, j, tmp;
    for(i = 0; i < len/2; i ++)
    {
        j = len - i - 1;
        tmp = pdat[i];
        pdat[i] = pdat[j];
        pdat[j] = tmp;
    }
}

inline void BytesSetZero(unsigned char* pbuf, int len)
{
    for(int i = 0; i < len; i ++)
    {
        pbuf[i] = 0;
    }
}

// len==2,两个字节调换， len==4,前后16bit调换
inline void SwitchHighLow(unsigned char* pdat, int len)
{
    unsigned char i,j;
    unsigned char tmp;

    for(i = 0; i < len; i ++)
    {
        j = len + i;
        tmp = pdat[i];
        pdat[i] = pdat[j];
        pdat[j] = tmp;
    }
}


//将16进制转换成Ascii码
inline void MakeCodeToAsii(unsigned char * psrc, unsigned char * pdst, int len)
{
    int i;

    if(len == 0)
        return;

    for(i = len - 1; i>= 0; i --)
    {
        unsigned char val = psrc[i];
        unsigned char tmp;

        //处理高4位
        tmp = val >> 4;
        if(tmp <= 9)
            tmp = tmp + '0';
        else
            tmp = tmp - 0x0A + 'A';

        pdst[2 * i] = tmp;

        //处理低4位
        tmp = val & 0x0F;
        if(tmp <= 9)
            tmp = tmp + '0';
        else
            tmp = tmp - 0x0A + 'A';

        pdst[2 * i + 1] = tmp;
    }
}


//将Ascii码转换成16进制
inline void MakeAsiiToCode(unsigned char * psrc, unsigned char * pdst, int len)
{
    int i;

    if(len == 0)
        return;

    for(i = 0; i < len; i ++)
    {
        unsigned char val;
        unsigned char tmp;

        //处理高4位
        tmp = psrc[2 * i];
        if(tmp >= 'A')
            tmp = tmp - 'A' + 0x0A;
        else
            tmp = tmp - '0';

        val = tmp << 4;

        //处理低4位
        tmp = psrc[2 * i + 1];
        if(tmp >= 'A')
            tmp = tmp - 'A' + 0x0A;
        else
            tmp = tmp - '0';

        val |= (tmp & 0x0F);

        pdst[i] = val;
    }
}

inline void ShowBufferHex(QString msg, unsigned char * pbuf, int len)
{
    QString strBuf;
    for(int i=0; i<len; i++)
    {
        QString s;
        s.sprintf("0x%02x ", pbuf[i]);
        strBuf += s;
    }
    msg += strBuf;
    qDebug()<< msg;
}

inline QString hexToString(char *buf, int size)
{
    QString ret = "";
    for(int i=0; i<size; i++){
        ret = ret + QString("%1").arg(buf[i]&0xff, 2, 16, QChar('0')) + " ";
    }
    ret.chop(1);
    return ret.toUpper();
}

inline void delayMs(unsigned int msec)
{
#if 0
    QTime reachTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < reachTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }
#else
    QEventLoop eventloop;
    QTimer::singleShot(msec, &eventloop, SLOT(quit()));
    eventloop.exec();
#endif
}

#endif // PUBLIC_FUNCTION_H
