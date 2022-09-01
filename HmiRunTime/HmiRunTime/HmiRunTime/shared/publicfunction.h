#ifndef PUBLIC_FUNCTION_H
#define PUBLIC_FUNCTION_H

#include <QString>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>
#include <QByteArray>

inline void BytesSetZero(unsigned char* pbuf, int len)
{
    for(int i = 0; i < len; i ++) {
        pbuf[i] = 0;
    }
}

// len==2,两个字节调换， len==4,前后16bit调换
inline void SwitchHighLow(unsigned char* pdat, int len)
{
    unsigned char i, j;
    unsigned char tmp;

    for(i = 0; i < len; i ++) {
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

    if(len == 0) {
        return;
    }

    for(i = len - 1; i >= 0; i --) {
        unsigned char val = psrc[i];
        unsigned char tmp;

        //处理高4位
        tmp = val >> 4;
        if(tmp <= 9) {
            tmp = tmp + '0';
        } else {
            tmp = tmp - 0x0A + 'A';
        }

        pdst[2 * i] = tmp;

        //处理低4位
        tmp = val & 0x0F;
        if(tmp <= 9) {
            tmp = tmp + '0';
        } else {
            tmp = tmp - 0x0A + 'A';
        }

        pdst[2 * i + 1] = tmp;
    }
}


//将Ascii码转换成16进制
inline void MakeAsiiToCode(unsigned char * psrc, unsigned char * pdst, int len)
{
    int i;

    if(len == 0) {
        return;
    }

    for(i = 0; i < len; i ++) {
        unsigned char val;
        unsigned char tmp;

        //处理高4位
        tmp = psrc[2 * i];
        if(tmp >= 'A') {
            tmp = tmp - 'A' + 0x0A;
        } else {
            tmp = tmp - '0';
        }

        val = tmp << 4;

        //处理低4位
        tmp = psrc[2 * i + 1];
        if(tmp >= 'A') {
            tmp = tmp - 'A' + 0x0A;
        } else {
            tmp = tmp - '0';
        }

        val |= (tmp & 0x0F);

        pdst[i] = val;
    }
}

inline QString hexToString(char *buf, int size)
{
    QString ret = "";
    for(int i = 0; i < size; i++) {
        ret = ret + QString("%1").arg(buf[i] & 0xff, 2, 16, QChar('0')) + " ";
    }
    ret.chop(1);
    return ret.toUpper();
}

inline void delayMs(unsigned int msec)
{
#if 0
    QTime reachTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < reachTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }
#else
    if(msec > 0) {
        QEventLoop eventloop;
        QTimer::singleShot(msec, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
#endif
}


/**
 * @brief lowWord
 * @details Return low word of a 32-bit integer.
 * @param ww quint32 ww (0x00000000..0xFFFFFFFF)
 * @return low word of input (0x0000..0xFFFF)
 */
inline quint16 lowWord(quint32 ww)
{
    return (quint16) ((ww) & 0xFFFF);
}


/**
 * @brief highWord
 * @details Return high word of a 32-bit integer.
 * @param ww quint32 ww (0x00000000..0xFFFFFFFF)
 * @return high word of input (0x0000..0xFFFF)
 */
inline quint16 highWord(quint32 ww)
{
    return (quint16) ((ww) >> 16);
}


/**
 * @brief crc16
 * @details Processor-independent CRC-16 calculation.

    Polynomial: x^16 + x^15 + x^2 + 1 (0xA001)<br>
    Initial value: 0xFFFF

    This CRC is normally used in disk-drive controllers.
 * @param crc quint16 crc (0x0000..0xFFFF)
 * @param a quint8 a (0x00..0xFF)
 * @return calculated CRC (0x0000..0xFFFF)
 */
inline quint16 crc16(quint16 crc, quint8 a)
{
    int i;

    crc ^= a;
    for (i = 0; i < 8; ++i) {
        if (crc & 1) {
            crc = (crc >> 1) ^ 0xA001;
        } else {
            crc = (crc >> 1);
        }
    }

    return crc;
}

inline quint8 byteSwitchHigh4bitLow4bit(quint8 dat)
{
    quint8 tmp = ((dat & 0xF0) >> 4) | ((dat & 0x0F) << 4);
    return tmp;
}

inline void boolToBytes(bool val, quint8* pDat)
{
    if(pDat) {
        pDat[0] = val ? 1 : 0;
    }
}

inline bool bytesToBool(quint8* pDat)
{
    if(pDat) {
        return (pDat[0] != 0);
    }
    return false;
}

inline void int8ToBytes(qint8 val, quint8* pDat)
{
    if(pDat) {
        pDat[0] = (quint8)val;
    }
}

inline qint8 bytesToInt8(quint8* pDat)
{
    if(pDat) {
        return (qint8)pDat[0];
    }
    return 0;
}

inline void uint8ToBytes(quint8 val, quint8* pDat)
{
    if(pDat) {
        pDat[0] = val;
    }
}

inline quint8 bytesToUint8(quint8* pDat)
{
    if(pDat) {
        return pDat[0];
    }
    return 0;
}

inline void int16ToBytes(qint16 val, quint8* pDat)
{
    union data {
        qint16 val;
        quint8 byte[2];
    };
    union data dat;
    dat.val = val;
    if(pDat) {
        pDat[0] = dat.byte[0];
        pDat[1] = dat.byte[1];
    }
}

inline qint16 bytesToInt16(quint8* pDat)
{
    if(pDat) {
        union data {
            qint16 val;
            quint8 byte[2];
        };
        union data dat;
        dat.byte[0] = pDat[0];
        dat.byte[1] = pDat[1];
        return dat.val;
    }
    return 0;
}

inline void uint16ToBytes(quint16 val, quint8* pDat)
{
    union data {
        quint16 val;
        quint8 byte[2];
    };
    union data dat;
    dat.val = val;
    if(pDat) {
        pDat[0] = dat.byte[0];
        pDat[1] = dat.byte[1];
    }
}

inline quint16 bytesToUint16(quint8* pDat)
{
    if(pDat) {
        union data {
            quint16 val;
            quint8 byte[2];
        };
        union data dat;
        dat.byte[0] = pDat[0];
        dat.byte[1] = pDat[1];
        return dat.val;
    }
    return 0;
}

inline void int32ToBytes(qint32 val, quint8* pDat)
{
    union data {
        qint32 val;
        quint8 byte[4];
    };
    union data dat;
    dat.val = val;
    if(pDat) {
        pDat[0] = dat.byte[0];
        pDat[1] = dat.byte[1];
        pDat[2] = dat.byte[2];
        pDat[3] = dat.byte[3];
    }
}

inline qint32 bytesToInt32(quint8* pDat)
{
    if(pDat) {
        union data {
            qint32 val;
            quint8 byte[4];
        };
        union data dat;
        dat.byte[0] = pDat[0];
        dat.byte[1] = pDat[1];
        dat.byte[2] = pDat[2];
        dat.byte[3] = pDat[3];
        return dat.val;
    }
    return 0;
}

inline void uint32ToBytes(quint32 val, quint8* pDat)
{
    union data {
        quint32 val;
        quint8 byte[4];
    };
    union data dat;
    dat.val = val;
    if(pDat) {
        pDat[0] = dat.byte[0];
        pDat[1] = dat.byte[1];
        pDat[2] = dat.byte[2];
        pDat[3] = dat.byte[3];
    }
}

inline quint32 bytesToUint32(quint8* pDat)
{
    if(pDat) {
        union data {
            quint32 val;
            quint8 byte[4];
        };
        union data dat;
        dat.byte[0] = pDat[0];
        dat.byte[1] = pDat[1];
        dat.byte[2] = pDat[2];
        dat.byte[3] = pDat[3];
        return dat.val;
    }
    return 0;
}

inline void int64ToBytes(qint64 val, quint8* pDat)
{
    union data {
        qint64 val;
        quint8 byte[8];
    };
    union data dat;
    dat.val = val;
    if(pDat) {
        pDat[0] = dat.byte[0];
        pDat[1] = dat.byte[1];
        pDat[2] = dat.byte[2];
        pDat[3] = dat.byte[3];
        pDat[4] = dat.byte[4];
        pDat[5] = dat.byte[5];
        pDat[6] = dat.byte[6];
        pDat[7] = dat.byte[7];
    }
}

inline qint64 bytesToInt64(quint8* pDat)
{
    if(pDat) {
        union data {
            qint64 val;
            quint8 byte[8];
        };
        union data dat;
        dat.byte[0] = pDat[0];
        dat.byte[1] = pDat[1];
        dat.byte[2] = pDat[2];
        dat.byte[3] = pDat[3];
        dat.byte[4] = pDat[4];
        dat.byte[5] = pDat[5];
        dat.byte[6] = pDat[6];
        dat.byte[7] = pDat[7];
        return dat.val;
    }
    return 0;
}

inline void uint64ToBytes(quint64 val, quint8* pDat)
{
    union data {
        quint64 val;
        quint8 byte[8];
    };
    union data dat;
    dat.val = val;
    if(pDat) {
        pDat[0] = dat.byte[0];
        pDat[1] = dat.byte[1];
        pDat[2] = dat.byte[2];
        pDat[3] = dat.byte[3];
        pDat[4] = dat.byte[4];
        pDat[5] = dat.byte[5];
        pDat[6] = dat.byte[6];
        pDat[7] = dat.byte[7];
    }
}

inline quint64 bytesToUint64(quint8* pDat)
{
    if(pDat) {
        union data {
            quint64 val;
            quint8 byte[8];
        };
        union data dat;
        dat.byte[0] = pDat[0];
        dat.byte[1] = pDat[1];
        dat.byte[2] = pDat[2];
        dat.byte[3] = pDat[3];
        dat.byte[4] = pDat[4];
        dat.byte[5] = pDat[5];
        dat.byte[6] = pDat[6];
        dat.byte[7] = pDat[7];
        return dat.val;
    }
    return 0;
}

inline void float32ToBytes(float val, quint8* pDat)
{
    union data {
        float val;
        quint8 byte[4];
    };
    union data dat;
    dat.val = val;
    if(pDat) {
        pDat[0] = dat.byte[0];
        pDat[1] = dat.byte[1];
        pDat[2] = dat.byte[2];
        pDat[3] = dat.byte[3];
    }
}

inline float bytesToFloat32(quint8* pDat)
{
    if(pDat) {
        union data {
            float val;
            quint8 byte[4];
        };
        union data dat;
        dat.byte[0] = pDat[0];
        dat.byte[1] = pDat[1];
        dat.byte[2] = pDat[2];
        dat.byte[3] = pDat[3];
        return dat.val;
    }
    return 0;
}


inline void float64ToBytes(double val, quint8* pDat)
{
    union data {
        double val;
        quint8 byte[8];
    };
    union data dat;
    dat.val = val;
    if(pDat) {
        pDat[0] = dat.byte[0];
        pDat[1] = dat.byte[1];
        pDat[2] = dat.byte[2];
        pDat[3] = dat.byte[3];
        pDat[4] = dat.byte[4];
        pDat[5] = dat.byte[5];
        pDat[6] = dat.byte[6];
        pDat[7] = dat.byte[7];
    }
}

inline double bytesToFloat64(quint8* pDat)
{
    if(pDat) {
        union data {
            double val;
            quint8 byte[8];
        };
        union data dat;
        dat.byte[0] = pDat[0];
        dat.byte[1] = pDat[1];
        dat.byte[2] = pDat[2];
        dat.byte[3] = pDat[3];
        dat.byte[4] = pDat[4];
        dat.byte[5] = pDat[5];
        dat.byte[6] = pDat[6];
        dat.byte[7] = pDat[7];
        return dat.val;
    }
    return 0;
}

inline void bcd8ToBytes(quint8 val, quint8* pDat)
{
    return uint8ToBytes(val, pDat);
}

inline quint8 bytesToBcd8(quint8* pDat)
{
    return bytesToUint8(pDat);
}

inline void bcd16ToBytes(quint16 val, quint8* pDat)
{
    return uint16ToBytes(val, pDat);
}

inline quint16 bytesToBcd16(quint8* pDat)
{
    return bytesToUint16(pDat);
}

inline void bcd32ToBytes(quint32 val, quint8* pDat)
{
    return uint32ToBytes(val, pDat);
}

inline quint32 bytesToBcd32(quint8* pDat)
{
    return bytesToUint32(pDat);
}

#endif // PUBLIC_FUNCTION_H
