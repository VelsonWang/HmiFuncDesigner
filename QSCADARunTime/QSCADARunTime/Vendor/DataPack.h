#ifndef __DATAPACK_H__
#define __DATAPACK_H__

#include <QtGlobal>

//计算校验和
extern quint32 AddCheckSum(quint8* pBuf, quint32 len);
extern quint8 XorCheckSum(quint8* pBuf, quint32 len);
//将数据前后字倒换
extern void RecoverSelfWordData(quint8 *pDst, quint32 Len);
extern void _RecoverSelfWordData(quint8 *pDst, quint32 Len);
//将数据排列顺序反过来放到另一个地址
extern void RecoverData(quint8 *pSrc, quint8 *pDst, quint32 Len);
//将数据排列顺序反过来覆盖自身地址
extern void RecoverSelfData(quint8 * pData, quint32 Len);
//将16进制转换成Ascii码
extern void MakeCodeToAsii(quint8 *pSrc, quint8 *pDst, quint32 len);
//将Ascii码转换成16进制
extern void MakeAsiiToCode(quint8 *pSrc, quint8 *pDst, quint32 len);

extern void SetDataAsWord(quint8 *pData, quint16 w);
extern quint16 GetDataAsWord(quint8 *pData);
extern void SetDataAsquint32(quint8 *pData, quint32 dw);
extern quint32 GetDataAsquint32(quint8 *pData);

//将16进制数据值转换成数字ASCII码
extern size_t SetWordToAscData(quint8 *pData, quint32 dw, size_t len);
//将16进制数据值转换成数字BCD码
extern size_t SetWordToBcdData(quint8 *pData, quint32 dw, size_t len);
//将BCD码转换为16进制数
extern quint32 GetWordAsBcdData(quint8 *pData, size_t len);
//将一个字符的16进制数转换为BCD码
extern quint8 HEX_to_BCD(quint8 x);

#endif
