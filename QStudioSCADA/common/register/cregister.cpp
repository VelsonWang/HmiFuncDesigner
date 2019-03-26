#include "cregister.h"
#include <QSettings>
#include <QDateTime>
#include "edncrypt.h"

#include <stdio.h>

// intrinsics
#if defined(__GNUC__)    // GCC
#include <cpuid.h>
#elif defined(_MSC_VER)    // MSVC
    #if _MSC_VER >=1400    // VC2005
#include <intrin.h>
    #endif    // #if _MSC_VER >=1400
#else
#error Only supports MSVC or GCC.
#endif    // #if defined(__GNUC__)



#define REGKEY          "HKEY_CURRENT_USER\\SOFTWARE\\XPEN\\{70FF-AD2F-CFDC-C29A-FB97-03CA-34C6-A7F1}"
#define REGKEYNAME      "value"
#define DTFORMAT        "yyyyMMddhhmmss"


void getcpuidex(unsigned int CPUInfo[4], unsigned int InfoType, unsigned int ECXValue)
{
#if defined(__GNUC__)    // GCC
    __cpuid_count(InfoType, ECXValue, CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
#elif defined(_MSC_VER)    // MSVC
    #if defined(_WIN64) || _MSC_VER>=1600    // 64位下不支持内联汇编. 1600: VS2010, 据说VC2008 SP1之后才支持__cpuidex.
        __cpuidex((int*)(void*)CPUInfo, (int)InfoType, (int)ECXValue);
    #else
        if (NULL==CPUInfo)    return;
        _asm{
            // load. 读取参数到寄存器.
            mov edi, CPUInfo;    // 准备用edi寻址CPUInfo
            mov eax, InfoType;
            mov ecx, ECXValue;
            // CPUID
            cpuid;
            // save. 将寄存器保存到CPUInfo
            mov    [edi], eax;
            mov    [edi+4], ebx;
            mov    [edi+8], ecx;
            mov    [edi+12], edx;
        }
    #endif
#endif    // #if defined(__GNUC__)
}

void getcpuid(unsigned int CPUInfo[4], unsigned int InfoType)
{
#if defined(__GNUC__)    // GCC
    __cpuid(InfoType, CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
#elif defined(_MSC_VER)    // MSVC
    #if _MSC_VER>=1400    // VC2005才支持__cpuid
        __cpuid((int*)(void*)CPUInfo, (int)InfoType);
    #else
        getcpuidex(CPUInfo, InfoType, 0);
    #endif
#endif    // #if defined(__GNUC__)
}


/**
 * @brief cpu_getvendor
 * @details 取得CPU厂商（Vendor）
 * @param pvendor 接收厂商信息的字符串缓冲区。至少为13字节.
 * @return 成功时返回字符串的长度（一般为12）。失败时返回0.
 */
int cpu_getvendor(char* pvendor)
{
    unsigned int dwBuf[4];
    if (NULL==pvendor)    return 0;
    // Function 0: Vendor-ID and Largest Standard Function
    getcpuid(dwBuf, 0);
    // save. 保存到pvendor
    *(unsigned int *)&pvendor[0] = dwBuf[1];    // ebx: 前四个字符.
    *(unsigned int *)&pvendor[4] = dwBuf[3];    // edx: 中间四个字符.
    *(unsigned int *)&pvendor[8] = dwBuf[2];    // ecx: 最后四个字符.
    pvendor[12] = '\0';
    return 12;
}



/**
 * @brief cpu_getbrand
 * @details 取得CPU商标（Brand）
 * @param pbrand 接收商标信息的字符串缓冲区。至少为49字节.
 * @return 成功时返回字符串的长度（一般为48）。失败时返回0.
 */
int cpu_getbrand(char* pbrand)
{
    unsigned int dwBuf[4];
    if (NULL==pbrand)    return 0;
    // Function 0x80000000: Largest Extended Function Number
    getcpuid(dwBuf, 0x80000000U);
    if (dwBuf[0] < 0x80000004U)    return 0;
    // Function 80000002h,80000003h,80000004h: Processor Brand String
    getcpuid((unsigned int *)&pbrand[0], 0x80000002U);    // 前16个字符.
    getcpuid((unsigned int *)&pbrand[16], 0x80000003U);    // 中间16个字符.
    getcpuid((unsigned int *)&pbrand[32], 0x80000004U);    // 最后16个字符.
    pbrand[48] = '\0';
    return 48;
}

QString getCpuID()
{
    QString cpu_id = "";
    unsigned int dwBuf[4]={0};
    unsigned long long ret = 0;

    getcpuid(dwBuf, 0);
    ret = dwBuf[3];
    ret = ret << 32;

    QString str0 = QString::number(dwBuf[3], 16).toUpper();
    QString str0_1 = str0.rightJustified(8, '0');
    QString str1 = QString::number(dwBuf[0], 16).toUpper();
    QString str1_1 = str1.rightJustified(8, '0');
    cpu_id = str0_1 + str1_1;

    return cpu_id;
}

CRegister::CRegister()
{

}

CRegister::~CRegister()
{

}

bool CRegister::CheckRegisterCode()
{
    QSettings regkey(REGKEY, QSettings::NativeFormat);

    const std::string code = regkey.value(REGKEYNAME).toString().toStdString();
    const std::string key = get_cpuid();
    const std::string origin = aes_decode(key, code);

    QDateTime dt = QDateTime::fromString(origin.c_str(), DTFORMAT);
    if (dt.toMSecsSinceEpoch() > QDateTime::currentDateTime().toMSecsSinceEpoch())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CRegister::AuthRegisterCode(const std::string& code)
{
    const std::string key = get_cpuid();
    const std::string origin = aes_decode(key, code);

    QDateTime dt = QDateTime::fromString(origin.c_str(), DTFORMAT);
    if (dt.toMSecsSinceEpoch() < QDateTime::currentDateTime().toMSecsSinceEpoch())
    {
        return false;
    }

    // save to regedit
    QSettings regkey(REGKEY, QSettings::NativeFormat);
    regkey.setValue(REGKEYNAME, code.c_str());

    return true;
}

std::string CRegister::GengerateCode(const std::string &key, const std::string& origin)
{
    return aes_encode(key, origin);
}

std::string CRegister::get_cpuid()
{
    return getCpuID().toStdString();
}

std::string CRegister::aes_decode(const std::string &key, const std::string &code)
{
    return EDncrypt::Dncrypt(code.c_str(), AES, key.c_str()).toStdString();
}

std::string CRegister::aes_encode(const std::string &key, const std::string &code)
{
    return EDncrypt::Encrypt(code.c_str(), AES, key.c_str()).toStdString();
}
