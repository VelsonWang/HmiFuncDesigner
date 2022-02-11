#include "S7_200.h"



S7_200::S7_200() {

}


///
/// \brief S7_200::getBitMaxRegPacket
/// \details 位组包最大寄存器个数
/// \return 寄存器个数
///
int S7_200::getBitMaxRegPacket()
{
    return 100;
}


///
/// \brief S7_200::getWordMaxRegPacket
/// \details 字组包最大寄存器个数
/// \return 寄存器个数
///
int S7_200::getWordMaxRegPacket()
{
    return 100;
}


///
/// \brief S7_200::getCommFailRetryCount
/// \details 通信失败重试次数
/// \return 次数
///
int S7_200::getCommFailRetryTimes()
{
    return 2;
}


///
/// \brief S7_200::getCommTimeout
/// \details 通信超时时间
/// \return 时间值
///
int S7_200::getCommTimeout()
{
    return 1;
}


///
/// \brief S7_200::getCommIntervalTime
/// \details 通信间隔时间
/// \return 时间值
///
int S7_200::getCommIntervalTime()
{
    return 200;
}


///
/// \brief S7_200::getCommResumeTime
/// \details 尝试恢复通讯间隔时间
/// \return 时间值
///
int S7_200::getCommResumeTime()
{
    return 2;
}

///
/// \brief S7_200::getDefaultDeviceProperty
/// \details 获取设备默认属性
/// \param properties
///
void S7_200::getDefaultDeviceProperty(QVector<QPair<QString, QString>>& properties)
{
    properties.clear();
    properties.append(qMakePair(tr("从站ID"), QString("2")));
}


///
/// \brief S7_200::getDefaultDevicePropertyDataType
/// \details 获取设备默认属性数据类型
/// \param properties_type
///
void S7_200::getDefaultDevicePropertyDataType(QVector<QPair<QString, QString>>& properties_type)
{
    properties_type.clear();
    properties_type.append(qMakePair(tr("从站ID"), QString("int")));
}

///
/// \brief S7_200::writeAsXml
/// \details 保存属性至xml节点
/// \param xml xml节点
/// \param properties 属性
///
void S7_200::writeAsXml(QXmlStreamWriter &xml, QVector<QPair<QString, QString>>& properties)
{
    xml.writeStartElement("property");
    xml.writeAttribute("id", getValue2ByValue1(tr("从站ID"), properties));
    xml.writeEndElement();
}


///
/// \brief S7_200::readFromXml
/// \details 从xml节点加载属性
/// \param xml xml节点
/// \param properties 属性
///
void S7_200::readFromXml(QXmlStreamReader &xml, QVector<QPair<QString, QString>>& properties)
{
    properties.clear();
    QXmlStreamAttributes attributes = xml.attributes();
    if (attributes.hasAttribute("id")) {
        properties.append(qMakePair(tr("从站ID"), attributes.value("id").toString()));
    }
}


///
/// \brief S7_200::setDeviceProperty
/// \details 设置设备属性
/// \param properties
///
void S7_200::setDeviceProperty(QVector<QPair<QString, QString>>& properties)
{
    m_properties.clear();
    m_properties.append(properties);
}


///
/// \brief S7_200::GetDeviceSupportProtocol
/// \details 获取设备支持的所有协议
/// \return
///
QStringList S7_200::getDeviceSupportProtocol()
{
    QStringList list;
    list<<"PPI";
    return list;
}

///
/// \brief S7_200::getDeviceSupportRegisterArea
/// \details 获取设备支持的所有寄存器区
/// \return
///
QStringList S7_200::getDeviceSupportRegisterArea()
{
    QStringList list;
    list << tr("I")
         << tr("Q")
         << tr("M")
         << tr("MB")
         << tr("MW")
         << tr("MD")
         << tr("SMB")
         << tr("SMW")
         << tr("SMD")
         << tr("V")
         << tr("VB")
         << tr("VW")
         << tr("VD")
         << tr("T")
         << tr("C");
    return list;
}


///
/// \brief S7_200::getDeviceSupportRegisterArea
/// \details 获取指定数据类型和读写属性设备支持的寄存器区
/// \param szDataType 数据类型
/// \param bWriteable 读写属性
/// \return 寄存器区
///
QStringList S7_200::getDeviceSupportRegisterArea(const QString &szDataType, bool bWriteable)
{
    Q_UNUSED(szDataType)
    Q_UNUSED(bWriteable)
    return QStringList();
}


///
/// \brief S7_200::GetDeviceSupportDataType
/// \details 获取设备支持的所有数据类型
/// \param szAreaName 寄存器区
/// \return
///
QStringList S7_200::getDeviceSupportDataType(const QString &szAreaName)
{
    QStringList list;

    if(szAreaName == tr("I") || szAreaName == tr("Q") || szAreaName == tr("M") ||
            szAreaName == tr("V")) {
        list << tr("bool");
    }
    else if(szAreaName == tr("MB") || szAreaName == tr("SMB") || szAreaName == tr("VB")) {
        list << tr("int8")
             << tr("uint8")
             << tr("bcd8");
    }
    else if(szAreaName == tr("MW") || szAreaName == tr("SMW") || szAreaName == tr("VW")) {
        list << tr("int16")
             << tr("uint16")
             << tr("bcd16");
    }
    else if(szAreaName == tr("MD") || szAreaName == tr("SMD") || szAreaName == tr("VD")) {
        list << tr("int32")
             << tr("uint32")
             << tr("float32")
             << tr("bcd32");
    }
    else if(szAreaName == tr("T")) {
        list << tr("bool")
             << tr("uint16")
             << tr("bcd16");
    }
    else if(szAreaName == tr("C")) {
        list << tr("bool")
             << tr("int16")
             << tr("uint16")
             << tr("bcd16");
    }

    return list;
}


/**
 * @brief S7_200::getRegisterAreaLimit
 * @details 获取寄存器区地址的下限和上限
 * @param szAreaName 寄存器区名称
 * @param iLowerLimit 寄存器区地址下限
 * @param iUpperLimit 寄存器区地址上限
 */
void S7_200::getRegisterAreaLimit(const QString &szAreaName,
                                  quint32 &iLowerLimit,
                                  quint32 &iUpperLimit)
{
    iLowerLimit = 0;
    iUpperLimit = 0;
    if(szAreaName == tr("V") || szAreaName == tr("VB")) {
        iLowerLimit = 0;
        iUpperLimit = 10239;
    } else if(szAreaName == tr("VW")) {
        iLowerLimit = 0;
        iUpperLimit = 10238;
    } else if(szAreaName == tr("VD")) {
        iLowerLimit = 0;
        iUpperLimit = 10236;
    } else if(szAreaName == tr("I")) {
        iLowerLimit = 0;
        iUpperLimit = 15;
    } else if(szAreaName == tr("Q")) {
        iLowerLimit = 0;
        iUpperLimit = 15;
    } else if(szAreaName == tr("M") || szAreaName == tr("MB")) {
        iLowerLimit = 0;
        iUpperLimit = 31;
    } else if(szAreaName == tr("MW")) {
        iLowerLimit = 0;
        iUpperLimit = 30;
    } else if(szAreaName == tr("MD")) {
        iLowerLimit = 0;
        iUpperLimit = 28;
    } else if(szAreaName == tr("T")) {
        iLowerLimit = 0;
        iUpperLimit = 255;
    } else if(szAreaName == tr("C")) {
        iLowerLimit = 0;
        iUpperLimit = 255;
    }else if(szAreaName == tr("SMB")) {
        iLowerLimit = 0;
        iUpperLimit = 299;
    } else if(szAreaName == tr("SMW")) {
        iLowerLimit = 0;
        iUpperLimit = 297;
    } else if(szAreaName == tr("SMD")) {
        iLowerLimit = 0;
        iUpperLimit = 295;
    }
}


///
/// \brief S7_200::getAddressTypeAlias
/// \details 获取地址类型别名
/// \param szAreaName 寄存器区名称
/// \param szDataType 数据类型
/// \param szAddrOffset 地址偏移量
/// \return 地址类型别名
///
QString S7_200::getAddressTypeAlias(const QString &szDataType,
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
/// \brief S7_200::getAddressTypeAlias
/// \details 获取地址类型别名
/// \param szAreaName 寄存器区名称
/// \return 地址类型别名
///
QString S7_200::getAddressTypeAlias(const QString &szAreaName)
{
    return szAreaName;
}


///
/// \brief FX2N::getAddressType
/// \details 获取指定地址类型别名的地址类型
/// \param szAddressTypeAlias 地址类型别名
/// \return 寄存器区名称
///
QString S7_200::getAddressType(const QString &szAddressTypeAlias)
{
    return szAddressTypeAlias;
}


///
/// \brief S7_200::getAutoAddrMapItemList
/// \details 获取寄存器地址映射列表
/// \param listAutoAddrMapItem
///
void S7_200::getAutoAddrMapItemList(QList<PAutoAddrMapItem> &listAutoAddrMapItem)
{
    Q_UNUSED(listAutoAddrMapItem)
}



///
/// \brief S7_200::getDeviceSupportRegisterAreaSubArea
/// \details 获取设备支持的地址类型所有子寄存器区名称
/// \param szAreaName 寄存器区名称
/// \return 地址类型所有子寄存器区名称
///
QStringList S7_200::getDeviceSupportRegisterAreaSubArea(const QString &szAreaName)
{
    Q_UNUSED(szAreaName)
    return QStringList();
}


