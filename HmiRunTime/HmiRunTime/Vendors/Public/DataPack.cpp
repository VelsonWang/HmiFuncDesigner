#include "DataPack.h"
#include <string.h>

quint32 AddCheckSum(quint8 *pBuf, quint32 len)
{
    quint32 i;
    quint32 dwRet = 0;

    for(i = 0; i < len; i ++) {
        dwRet += pBuf[i];
    }

    return dwRet;
}

quint8 XorCheckSum(quint8 *pBuf, quint32 len)
{
    quint32 i;
    quint8 byRet = 0;

    if(len == 0) {
        return byRet;
    } else {
        byRet = pBuf[0];
    }

    for(i = 1; i < len; i ++) {
        byRet = byRet ^ pBuf[i];
    }

    return byRet;
}

void RecoverSelfWordData(quint8 *pDst, quint32 Len)
{
    quint8 buffer[2];
    qint32 iWordCount = Len / 2;
    size_t i;
    quint8 *pSw;
    quint8 *pDw;

    Len = iWordCount / 2;
    for(i = 0; i < Len; i ++) {
        pSw = pDst + i * 2;
        pDw = pDst + (iWordCount - 1 - i) * 2;

        memcpy(buffer, pSw, 2);
        memcpy(pSw, pDw, 2);
        memcpy(pDw, buffer, 2);
    }
}

void _RecoverSelfWordData(quint8 *pDst, quint32 Len)
{
    quint32 iWordCount = Len / 2;
    quint32 i;
    quint8 byTmp;

    for(i = 0; i < iWordCount; i ++) {
        byTmp = pDst[i * 2];
        pDst[i * 2] = pDst[i * 2 + 1];
        pDst[i * 2 + 1] = byTmp;
    }
}

void RecoverData(quint8 *pSrc, quint8 *pDst, quint32 Len)
{
    quint32 i;

    for(i = 0; i < Len; i ++) {
        pDst[Len - i - 1] = pSrc[i];
    }
}

void RecoverSelfData(quint8 *pData, quint32 Len)
{
    int iCount = Len;
    quint32 i;
    int iTemp;
    quint8 byTemp;

    Len = Len >> 1;
    for(i = 0; i < Len; i ++) {
        iTemp = iCount - i - 1;
        byTemp = pData[i];
        pData[i] = pData[iTemp];
        pData[iTemp] = byTemp;
    }
}

void RecoverSelfData(QByteArray &data, quint32 Len)
{
    quint8 *pData = new quint8[Len];
    memcpy(pData, data.data(), Len);

    int iCount = Len;
    int iTemp;
    quint8 byTemp;

    Len = Len >> 1;
    for(quint32 i = 0; i < Len; i ++) {
        iTemp = iCount - i - 1;
        byTemp = pData[i];
        pData[i] = pData[iTemp];
        pData[iTemp] = byTemp;
    }

    data.clear();
    for(quint32 i = 0; i < Len; i++) {
        data.append((char) pData[i]);
    }

    delete[] pData;
}



//将16进制转换成Ascii码
void MakeCodeToAsii(quint8 *pSrc, quint8 *pDst, quint32 len)
{
    int i;

    if(len == 0) {
        return;
    }

    for(i = (int)len - 1; i >= 0; i --) {
        quint8 byValue = pSrc[i];
        quint8 byTemp;

        //处理高4位
        byTemp = byValue >> 4;
        if(byTemp <= 9) {
            byTemp = byTemp + '0';
        } else {
            byTemp = byTemp - 0x0A + 'A';
        }

        pDst[2 * i] = byTemp;

        //处理低4位
        byTemp = byValue & 0x0F;
        if(byTemp <= 9) {
            byTemp = byTemp + '0';
        } else {
            byTemp = byTemp - 0x0A + 'A';
        }

        pDst[2 * i + 1] = byTemp;
    }
}


//将Ascii码转换成16进制
void MakeAsiiToCode(quint8 *pSrc, quint8 *pDst, quint32 len)
{
    quint32 i;

    if(len == 0) {
        return;
    }

    for(i = 0; i < len; i ++) {
        quint8 byValue;
        quint8 byTemp;

        //处理高4位
        byTemp = pSrc[2 * i];
        if(byTemp >= 'A') {
            byTemp = byTemp - 'A' + 0x0A;
        } else {
            byTemp = byTemp - '0';
        }

        byValue = byTemp << 4;

        //处理低4位
        byTemp = pSrc[2 * i + 1];
        if(byTemp >= 'A') {
            byTemp = byTemp - 'A' + 0x0A;
        } else {
            byTemp = byTemp - '0';
        }

        byValue |= (byTemp & 0x0F);

        pDst[i] = byValue;
    }
}

void SetDataAsWord(quint8 *pData, quint16 w)
{
    memcpy(pData, (quint8 *)&w, 2);
}

quint16 GetDataAsWord(quint8 *pData)
{
    quint16 w;
    memcpy((void *)&w, pData, 2);
    return w;
}

void SetDataAsquint32(quint8 *pData, quint32 dw)
{
    memcpy(pData, (quint8 *)&dw, 4);
}

quint32 GetDataAsquint32(quint8 *pData)
{
    quint32 dw;
    memcpy((void *)&dw, pData, 4);
    return dw;
}

size_t SetWordToAscData(quint8 *pData, quint32 dw, size_t len)
{
    quint8  by;
    size_t i;
    size_t iRet = 0;

    for(i = len - 1; i >= 0; i --) {
        if(dw > 0) {
            iRet ++;
        } else {
            break;
        }

        by = (quint8)(dw % 10);
        pData[i] = by + '0';
        dw = dw / 10;
    }

    if(iRet < len) {
        memset(pData, '0', len - iRet);
    }

    return iRet;
}

size_t SetWordToBcdData(quint8 *pData, quint32 dw, size_t len)
{
    int i;
    size_t iPos;
    size_t iRet = 0;
    quint8 byValue;

    for(i = len * 2 - 1; i >= 0; i --) {
        if(dw == 0) {
            break;
        }

        byValue = (quint8)(dw % 10);
        iPos = i / 2;

        if((i & 0x01) == 0x01) {
            pData[iPos] = byValue;
            iRet ++;
        } else {
            pData[iPos] |= (byValue << 4);
        }

        dw = dw / 10;
    }

    if(iRet < len) {
        memset(pData, 0, len - iRet);
    }

    return iRet;
}

quint32 GetWordAsBcdData(quint8 *pData, size_t len)
{
    size_t i;
    quint32 dwRet = 0;
    quint8 byValue;

    for(i = 0; i < len; i ++) {
        byValue = pData[i] & 0xF0;
        dwRet = 10 * dwRet + (byValue >> 4);
        dwRet = 10 * dwRet + (pData[i] & 0x0F);
    }

    return dwRet;
}

quint8 HEX_to_BCD(quint8 x)
{
    quint8 ucMSB, ucLSB, ucRet;

    ucMSB = x & 0xf0;
    ucRet = (ucMSB >> 4) * 10;
    ucLSB =	x & 0x0f;
    ucRet = ucRet + ucLSB;

    return ucRet;
}
