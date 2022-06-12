#include "FX2N.h"



FX2N::FX2N()
{

}


///
/// \brief FX2N::getBitMaxRegPacket
/// \details 位组包最大寄存器个数
/// \return 寄存器个数
///
int FX2N::getBitMaxRegPacket()
{
    return 40;
}


///
/// \brief FX2N::getWordMaxRegPacket
/// \details 字组包最大寄存器个数
/// \return 寄存器个数
///
int FX2N::getWordMaxRegPacket()
{
    return 20;
}


///
/// \brief FX2N::getCommFailRetryCount
/// \details 通信失败重试次数
/// \return 次数
///
int FX2N::getCommFailRetryTimes()
{
    return 2;
}


///
/// \brief FX2N::getCommTimeout
/// \details 通信超时时间
/// \return 时间值
///
int FX2N::getCommTimeout()
{
    return 3;
}


///
/// \brief FX2N::getCommIntervalTime
/// \details 通信间隔时间
/// \return 时间值
///
int FX2N::getCommIntervalTime()
{
    return 200;
}


///
/// \brief FX2N::getCommResumeTime
/// \details 尝试恢复通讯间隔时间
/// \return 时间值
///
int FX2N::getCommResumeTime()
{
    return 2;
}

///
/// \brief FX2N::getDefaultDeviceProperty
/// \details 获取设备默认属性
/// \param properties
///
void FX2N::getDefaultDeviceProperty(QVector<QPair<QString, QString>>& properties)
{
    properties.clear();
    //properties.append(qMakePair(tr("从站ID"), QString("5")));
}


///
/// \brief FX2N::getDefaultDevicePropertyDataType
/// \details 获取设备默认属性数据类型
/// \param properties_type
///
void FX2N::getDefaultDevicePropertyDataType(QVector<QPair<QString, QString>>& properties_type)
{
    properties_type.clear();
    //properties_type.append(qMakePair(tr("从站ID"), QString("int")));
}

///
/// \brief FX2N::writeAsXml
/// \details 保存属性至xml节点
/// \param xml xml节点
/// \param properties 属性
///
void FX2N::writeAsXml(QXmlStreamWriter &xml, QVector<QPair<QString, QString>>& properties)
{
    xml.writeStartElement("property");
    xml.writeAttribute("id", getValue2ByValue1(tr("从站ID"), properties));
    xml.writeEndElement();
}


///
/// \brief FX2N::readFromXml
/// \details 从xml节点加载属性
/// \param xml xml节点
/// \param properties 属性
///
void FX2N::readFromXml(QXmlStreamReader &xml, QVector<QPair<QString, QString>>& properties)
{
    properties.clear();
    QXmlStreamAttributes attributes = xml.attributes();
    if (attributes.hasAttribute("id")) {
        properties.append(qMakePair(tr("从站ID"), attributes.value("id").toString()));
    }
}


///
/// \brief FX2N::setDeviceProperty
/// \details 设置设备属性
/// \param properties
///
void FX2N::setDeviceProperty(QVector<QPair<QString, QString>>& properties)
{
    m_properties.clear();
    m_properties.append(properties);
}


///
/// \brief FX2N::GetDeviceSupportProtocol
/// \details 获取设备支持的所有协议
/// \return
///
QStringList FX2N::getDeviceSupportProtocol()
{
    QStringList list;
    list<<"FxProtocol";
    return list;
}

///
/// \brief FX2N::getDeviceSupportRegisterArea
/// \details 获取设备支持的所有寄存器区
/// \return 寄存器区
///
QStringList FX2N::getDeviceSupportRegisterArea()
{
    QStringList list;
    list << tr("X")
         << tr("Y")
         << tr("M")
         << tr("S")
         << tr("T")
         << tr("C")
         << tr("D")
         << tr("TN")
         << tr("CN");
    return list;
}

///
/// \brief FX2N::getDeviceSupportRegisterArea
/// \details 获取指定数据类型和读写属性设备支持的寄存器区
/// \param szDataType 数据类型
/// \param bWriteable 读写属性
/// \return 寄存器区
///
QStringList FX2N::getDeviceSupportRegisterArea(const QString &szDataType, bool bWriteable)
{
    Q_UNUSED(szDataType)
    Q_UNUSED(bWriteable)
    return QStringList();
}

///
/// \brief FX2N::GetDeviceSupportDataType
/// \details 获取设备支持的所有数据类型
/// \param szAreaName 寄存器区
/// \return
///
QStringList FX2N::getDeviceSupportDataType(const QString &szAreaName)
{
    QStringList list;

    if(szAreaName == tr("X") || szAreaName == tr("Y") || szAreaName == tr("M") ||
            szAreaName == tr("S") || szAreaName == tr("T") || szAreaName == tr("C")) {
        list << tr("bool");
    }
    else if(szAreaName == tr("D")) {
        //list << tr("bool")
        list << tr("int16")
             << tr("uint16")
             << tr("int32")
             << tr("uint32")
             << tr("float32");
    }
    else if(szAreaName == tr("TN")) {
        list << tr("int16")
             << tr("uint16");
    }
    else if(szAreaName == tr("CN")) {
        list << tr("int16")
             << tr("uint16")
             << tr("int32")
             << tr("uint32")
             << tr("float32");
    }
    return list;
}


/**
 * @brief FX2N::getRegisterAreaLimit
 * @details 获取寄存器区地址的下限和上限
 * @param szAreaName 寄存器区名称
 * @param iLowerLimit 寄存器区地址下限
 * @param iUpperLimit 寄存器区地址上限
 */
void FX2N::getRegisterAreaLimit(const QString &szAreaName,
                                quint32 &iLowerLimit,
                                quint32 &iUpperLimit)
{
    iLowerLimit = 0;
    iUpperLimit = 0;
    if(szAreaName == tr("X")) {
        iLowerLimit = 0;
        iUpperLimit = 177;
    } else if(szAreaName == tr("Y")) {
        iLowerLimit = 0;
        iUpperLimit = 177;
    } else if(szAreaName == tr("M")) {
        iLowerLimit = 0;
        iUpperLimit = 1023;
    } else if(szAreaName == tr("S")) {
        iLowerLimit = 0;
        iUpperLimit = 999;
    } else if(szAreaName == tr("D")) {
        iLowerLimit = 0;
        iUpperLimit = 0xFFFF;
    } else if(szAreaName == tr("T")) {
        iLowerLimit = 0;
        iUpperLimit = 511;
    }  else if(szAreaName == tr("TN")) {
        iLowerLimit = 0;
        iUpperLimit = 511;
    } else if(szAreaName == tr("C")) {
        iLowerLimit = 0;
        iUpperLimit = 255;
    } else if(szAreaName == tr("CN")) {
        iLowerLimit = 0;
        iUpperLimit = 255;
    }
}

///
/// \brief FX2N::getAddressTypeAlias
/// \details 获取地址类型别名
/// \param szAreaName 寄存器区名称
/// \param szDataType 数据类型
/// \param szAddrOffset 地址偏移量
/// \return 地址类型别名
///
QString FX2N::getAddressTypeAlias(const QString &szDataType,
                                  const QString &szAreaName,
                                  const QString &szAddrOffset,
                                  const QString &szAreaName2,
                                  const QString &szAddrOffset2)
{
    Q_UNUSED(szDataType)
    Q_UNUSED(szAreaName2)
    Q_UNUSED(szAddrOffset2)
    QString szAddrTypeAlias = szAreaName;
    szAddrTypeAlias += szAddrOffset;
    return szAddrTypeAlias;
}


///
/// \brief FX2N::getAddressTypeAlias
/// \details 获取地址类型别名
/// \param szAreaName 寄存器区名称
/// \return 地址类型别名
///
QString FX2N::getAddressTypeAlias(const QString &szAreaName)
{
    return szAreaName;
}


///
/// \brief FX2N::getAddressType
/// \details 获取指定地址类型别名的地址类型
/// \param szAddressTypeAlias 地址类型别名
/// \return 寄存器区名称
///
QString FX2N::getAddressType(const QString &szAddressTypeAlias)
{
    return szAddressTypeAlias;
}

///
/// \brief FX2N::getAutoAddrMapItemList
/// \details 获取寄存器地址映射列表
/// \param listAutoAddrMapItem
///
void FX2N::getAutoAddrMapItemList(QList<PAutoAddrMapItem> &listAutoAddrMapItem)
{
    Q_UNUSED(listAutoAddrMapItem)
}


///
/// \brief FX2N::getDeviceSupportRegisterAreaSubArea
/// \details 获取设备支持的地址类型所有子寄存器区名称
/// \param szAreaName 寄存器区名称
/// \return 地址类型所有子寄存器区名称
///
QStringList FX2N::getDeviceSupportRegisterAreaSubArea(const QString &szAreaName)
{
    Q_UNUSED(szAreaName)
    return QStringList();
}




