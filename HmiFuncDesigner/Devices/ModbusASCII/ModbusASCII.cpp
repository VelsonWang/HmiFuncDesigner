#include "ModbusASCII.h"

#define COIL_STATUS    tr("线圈状态")
#define DISCRETE_STATUS    tr("离散量状态")
#define INPUT_REGISTER    tr("输入寄存器")
#define HOLDING_REGISTER    tr("保持寄存器")

ModbusASCII::ModbusASCII()
{

}


///
/// \brief ModbusASCII::getBitMaxRegPacket
/// \details 位组包最大寄存器个数
/// \return 寄存器个数
///
int ModbusASCII::getBitMaxRegPacket()
{
    return 100;
}


///
/// \brief ModbusASCII::getWordMaxRegPacket
/// \details 字组包最大寄存器个数
/// \return 寄存器个数
///
int ModbusASCII::getWordMaxRegPacket()
{
    return 20;
}


///
/// \brief ModbusASCII::getCommFailRetryCount
/// \details 通信失败重试次数
/// \return 次数
///
int ModbusASCII::getCommFailRetryTimes()
{
    return 2;
}


///
/// \brief ModbusASCII::getCommTimeout
/// \details 通信超时时间
/// \return 时间值
///
int ModbusASCII::getCommTimeout()
{
    return 1;
}


///
/// \brief ModbusASCII::getCommIntervalTime
/// \details 通信间隔时间
/// \return 时间值
///
int ModbusASCII::getCommIntervalTime()
{
    return 200;
}


///
/// \brief ModbusASCII::getCommResumeTime
/// \details 尝试恢复通讯间隔时间
/// \return 时间值
///
int ModbusASCII::getCommResumeTime()
{
    return 2;
}

///
/// \brief ModbusASCII::getDefaultDeviceProperty
/// \details 获取设备默认属性
/// \param properties
///
void ModbusASCII::getDefaultDeviceProperty(QVector<QPair<QString, QString>>& properties)
{
    properties.clear();
    properties.append(qMakePair(tr("设备ID"), QString("1")));
    properties.append(qMakePair(tr("内存地址起始位为0"), QString("true")));
    properties.append(qMakePair(tr("写线圈功能码为15"), QString("false")));
    properties.append(qMakePair(tr("写寄存器功能码为16"), QString("false")));
    properties.append(qMakePair(tr("8位逆序"), QString("false")));
    properties.append(qMakePair(tr("16位低字节在前高字节在后"), QString("false")));
    properties.append(qMakePair(tr("32位低字节在前高字节在后"), QString("false")));
    properties.append(qMakePair(tr("64位低字节在前高字节在后"), QString("false")));
}


///
/// \brief ModbusASCII::getDefaultDevicePropertyDataType
/// \details 获取设备默认属性数据类型
/// \param properties_type
///
void ModbusASCII::getDefaultDevicePropertyDataType(QVector<QPair<QString, QString>>& properties_type)
{
    properties_type.clear();
    properties_type.append(qMakePair(tr("设备ID"), QString("int")));
    properties_type.append(qMakePair(tr("内存地址起始位为0"), QString("bool")));
    properties_type.append(qMakePair(tr("写线圈功能码为15"), QString("bool")));
    properties_type.append(qMakePair(tr("写寄存器功能码为16"), QString("bool")));
    properties_type.append(qMakePair(tr("8位逆序"), QString("bool")));
    properties_type.append(qMakePair(tr("16位低字节在前高字节在后"), QString("bool")));
    properties_type.append(qMakePair(tr("32位低字节在前高字节在后"), QString("bool")));
    properties_type.append(qMakePair(tr("64位低字节在前高字节在后"), QString("bool")));
}

///
/// \brief ModbusASCII::writeAsXml
/// \details 保存属性至xml节点
/// \param xml xml节点
/// \param properties 属性
///
void ModbusASCII::writeAsXml(QXmlStreamWriter &xml, QVector<QPair<QString, QString>>& properties)
{
    xml.writeStartElement("property");
    xml.writeAttribute("id", getValue2ByValue1(tr("设备ID"), properties));
    xml.writeAttribute("startAddrBit", getValue2ByValue1(tr("内存地址起始位为0"), properties));
    xml.writeAttribute("writeCoilFn", getValue2ByValue1(tr("写线圈功能码为15"), properties));
    xml.writeAttribute("writeRegFn", getValue2ByValue1(tr("写寄存器功能码为16"), properties));
    xml.writeAttribute("addr8", getValue2ByValue1(tr("8位逆序"), properties));
    xml.writeAttribute("addr16", getValue2ByValue1(tr("16位低字节在前高字节在后"), properties));
    xml.writeAttribute("addr32", getValue2ByValue1(tr("32位低字节在前高字节在后"), properties));
    xml.writeAttribute("addr64", getValue2ByValue1(tr("64位低字节在前高字节在后"), properties));
    xml.writeEndElement();
}


///
/// \brief ModbusASCII::readFromXml
/// \details 从xml节点加载属性
/// \param xml xml节点
/// \param properties 属性
///
void ModbusASCII::readFromXml(QXmlStreamReader &xml, QVector<QPair<QString, QString>>& properties)
{
    properties.clear();
    QXmlStreamAttributes attributes = xml.attributes();
    if (attributes.hasAttribute("id")) {
        properties.append(qMakePair(tr("设备ID"), attributes.value("id").toString()));
    }
    if (attributes.hasAttribute("startAddrBit")) {
        properties.append(qMakePair(tr("内存地址起始位为0"), attributes.value("startAddrBit").toString()));
    }
    if (attributes.hasAttribute("writeCoilFn")) {
        properties.append(qMakePair(tr("写线圈功能码为15"), attributes.value("writeCoilFn").toString()));
    }
    if (attributes.hasAttribute("writeRegFn")) {
        properties.append(qMakePair(tr("写寄存器功能码为16"), attributes.value("writeRegFn").toString()));
    }
    if (attributes.hasAttribute("addr8")) {
        properties.append(qMakePair(tr("8位逆序"), attributes.value("addr8").toString()));
    }
    if (attributes.hasAttribute("addr16")) {
        properties.append(qMakePair(tr("16位低字节在前高字节在后"), attributes.value("addr16").toString()));
    }
    if (attributes.hasAttribute("addr32")) {
        properties.append(qMakePair(tr("32位低字节在前高字节在后"), attributes.value("addr32").toString()));
    }
    if (attributes.hasAttribute("addr64")) {
        properties.append(qMakePair(tr("64位低字节在前高字节在后"), attributes.value("addr64").toString()));
    }
}


///
/// \brief ModbusASCII::setDeviceProperty
/// \details 设置设备属性
/// \param properties
///
void ModbusASCII::setDeviceProperty(QVector<QPair<QString, QString>>& properties)
{
    m_properties.clear();
    m_properties.append(properties);
    QString szVal = getValue2ByValue1(tr("内存地址起始位为0"), m_properties);
    m_bStartAddrBit0 = (szVal.toLower() == "true") ? true : false;
}


///
/// \brief ModbusASCII::GetDeviceSupportProtocol
/// \details 获取设备支持的所有协议
/// \return
///
QStringList ModbusASCII::getDeviceSupportProtocol()
{
    QStringList list;
    list<<"ModbusASCII";
    return list;
}

///
/// \brief ModbusASCII::getDeviceSupportRegisterArea
/// \details 获取设备支持的所有寄存器区
/// \return
///
QStringList ModbusASCII::getDeviceSupportRegisterArea()
{
    QStringList list;
    list << COIL_STATUS
         << DISCRETE_STATUS
         << INPUT_REGISTER
         << HOLDING_REGISTER;
    return list;
}

///
/// \brief ModbusASCII::getDeviceSupportRegisterArea
/// \details 获取指定数据类型和读写属性设备支持的寄存器区
/// \param szDataType 数据类型
/// \param bWriteable 读写属性
/// \return 寄存器区
///
QStringList ModbusASCII::getDeviceSupportRegisterArea(const QString &szDataType, bool bWriteable)
{
    Q_UNUSED(szDataType)
    Q_UNUSED(bWriteable)
    return QStringList();
}

///
/// \brief ModbusASCII::GetDeviceSupportDataType
/// \details 获取设备支持的所有数据类型
/// \param szAreaName 寄存器区
/// \return
///
QStringList ModbusASCII::getDeviceSupportDataType(const QString &szAreaName)
{
    QStringList list;

    if(szAreaName == COIL_STATUS || szAreaName == DISCRETE_STATUS) {
        list << tr("bool");
    }
    else if(szAreaName == HOLDING_REGISTER || szAreaName == INPUT_REGISTER ||
            szAreaName == tr("所有") || szAreaName == "ALL") {
        list << tr("int16")
             << tr("uint16")
             << tr("int32")
             << tr("uint32")
             << tr("float32")
             << tr("double")
             << tr("bcd16")
             << tr("bcd32");
    }
    return list;
}


/**
 * @brief ModbusASCII::getRegisterAreaLimit
 * @details 获取寄存器区地址的下限和上限
 * @param szAreaName 寄存器区名称
 * @param iLowerLimit 寄存器区地址下限
 * @param iUpperLimit 寄存器区地址上限
 */
void ModbusASCII::getRegisterAreaLimit(const QString &szAreaName,
                                       quint32 &iLowerLimit,
                                       quint32 &iUpperLimit)
{
    iLowerLimit = 0;
    iUpperLimit = 0;
    if(szAreaName == COIL_STATUS) {
        iLowerLimit = 0x0000;
        iUpperLimit = 0xFFFF;
    } else if(szAreaName == DISCRETE_STATUS) {
        iLowerLimit = 0x0000;
        iUpperLimit = 0xFFFF;
    } else if(szAreaName == INPUT_REGISTER) {
        iLowerLimit = 0x0000;
        iUpperLimit = 0xFFFF;
    } else if(szAreaName == HOLDING_REGISTER) {
        iLowerLimit = 0x0000;
        iUpperLimit = 0xFFFF;
    }
    if(!m_bStartAddrBit0) iLowerLimit++;
}

///
/// \brief ModbusASCII::getAddressTypeAlias
/// \details 获取地址类型别名
/// \param szAreaName 寄存器区名称
/// \param szDataType 数据类型
/// \param szAddrOffset 地址偏移量
/// \return 地址类型别名
///
QString ModbusASCII::getAddressTypeAlias(const QString &szDataType,
                                         const QString &szAreaName,
                                         const QString &szAddrOffset,
                                         const QString &szAreaName2,
                                         const QString &szAddrOffset2)
{
    Q_UNUSED(szDataType)
    Q_UNUSED(szAreaName2)
    Q_UNUSED(szAddrOffset2)
    QString szAddrTypeAlias = "";
    if(szAreaName == COIL_STATUS) {
        szAddrTypeAlias = "0x";
    } else if(szAreaName == DISCRETE_STATUS) {
        szAddrTypeAlias = "1x";
    } else if(szAreaName == INPUT_REGISTER) {
        szAddrTypeAlias = "3x";
    } else if(szAreaName == HOLDING_REGISTER) {
        szAddrTypeAlias = "4x";
    }
    szAddrTypeAlias += QString("%1").arg(szAddrOffset, 5, QChar('0'));
    return szAddrTypeAlias;
}


///
/// \brief ModbusASCII::getAddressTypeAlias
/// \details 获取地址类型别名
/// \param szAreaName 寄存器区名称
/// \return 地址类型别名
///
QString ModbusASCII::getAddressTypeAlias(const QString &szAreaName)
{
    QString szAddrTypeAlias = "";
    if(szAreaName == COIL_STATUS) {
        szAddrTypeAlias = "0x";
    } else if(szAreaName == DISCRETE_STATUS) {
        szAddrTypeAlias = "1x";
    } else if(szAreaName == INPUT_REGISTER) {
        szAddrTypeAlias = "3x";
    } else if(szAreaName == HOLDING_REGISTER) {
        szAddrTypeAlias = "4x";
    }
    return szAddrTypeAlias;
}


///
/// \brief ModbusASCII::getAddressType
/// \details 获取指定地址类型别名的地址类型
/// \param szAddressTypeAlias 地址类型别名
/// \return 寄存器区名称
///
QString ModbusASCII::getAddressType(const QString &szAddressTypeAlias)
{
    QString szAddrType = "";
    if(szAddressTypeAlias.toLower() == "0x") {
        szAddrType = COIL_STATUS;
    } else if(szAddressTypeAlias.toLower() == "1x") {
        szAddrType = DISCRETE_STATUS;
    } else if(szAddressTypeAlias.toLower() == "3x") {
        szAddrType = INPUT_REGISTER;
    } else if(szAddressTypeAlias.toLower() == "4x") {
        szAddrType = HOLDING_REGISTER;
    }
    return szAddrType;
}


///
/// \brief ModbusASCII::getAutoAddrMapItemList
/// \details 获取寄存器地址映射列表
/// \param listAutoAddrMapItem
///
void ModbusASCII::getAutoAddrMapItemList(QList<PAutoAddrMapItem> &listAutoAddrMapItem)
{
    Q_UNUSED(listAutoAddrMapItem)
}



///
/// \brief ModbusASCII::getDeviceSupportRegisterAreaSubArea
/// \details 获取设备支持的地址类型所有子寄存器区名称
/// \param szAreaName 寄存器区名称
/// \return 地址类型所有子寄存器区名称
///
QStringList ModbusASCII::getDeviceSupportRegisterAreaSubArea(const QString &szAreaName)
{
    Q_UNUSED(szAreaName)
    return QStringList();
}



