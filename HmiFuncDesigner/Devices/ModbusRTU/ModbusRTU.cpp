#include "ModbusRTU.h"

#define COIL_STATUS    tr("线圈状态")
#define DISCRETE_STATUS    tr("离散量状态")
#define INPUT_REGISTER    tr("输入寄存器")
#define HOLDING_REGISTER    tr("保持寄存器")


ModbusRTU::ModbusRTU() {

}

///
/// \brief ModbusRTU::getDefaultDeviceProperty
/// \details 获取设备默认属性
/// \param properties
///
void ModbusRTU::getDefaultDeviceProperty(QVector<QPair<QString, QString>>& properties) {
    properties.clear();
    properties.append(qMakePair(tr("设备ID"), QString("1")));
    properties.append(qMakePair(tr("位组包最大寄存器个数"), QString("100")));
    properties.append(qMakePair(tr("字组包最大寄存器个数"), QString("20")));
    properties.append(qMakePair(tr("失败重试次数"), QString("2")));
    properties.append(qMakePair(tr("通讯超时时间(s)"), QString("1")));
    properties.append(qMakePair(tr("通讯间隔时间(ms)"), QString("200")));
    properties.append(qMakePair(tr("尝试恢复间隔(s)"), QString("2")));
    properties.append(qMakePair(tr("内存地址起始位为0"), QString("true")));
    properties.append(qMakePair(tr("写线圈功能码为15"), QString("false")));
    properties.append(qMakePair(tr("写寄存器功能码为16"), QString("false")));
    properties.append(qMakePair(tr("8位逆序"), QString("false")));
    properties.append(qMakePair(tr("16位低字节在前高字节在后"), QString("false")));
    properties.append(qMakePair(tr("32位低字节在前高字节在后"), QString("false")));
    properties.append(qMakePair(tr("64位低字节在前高字节在后"), QString("false")));
}


///
/// \brief ModbusRTU::getDefaultDevicePropertyDataType
/// \details 获取设备默认属性数据类型
/// \param properties_type
///
void ModbusRTU::getDefaultDevicePropertyDataType(QVector<QPair<QString, QString>>& properties_type) {
    properties_type.clear();
    properties_type.append(qMakePair(tr("设备ID"), QString("int")));
    properties_type.append(qMakePair(tr("位组包最大寄存器个数"), QString("int")));
    properties_type.append(qMakePair(tr("字组包最大寄存器个数"), QString("int")));
    properties_type.append(qMakePair(tr("失败重试次数"), QString("int")));
    properties_type.append(qMakePair(tr("通讯超时时间(s)"), QString("int")));
    properties_type.append(qMakePair(tr("通讯间隔时间(ms)"), QString("int")));
    properties_type.append(qMakePair(tr("尝试恢复间隔(s)"), QString("int")));
    properties_type.append(qMakePair(tr("内存地址起始位为0"), QString("bool")));
    properties_type.append(qMakePair(tr("写线圈功能码为15"), QString("bool")));
    properties_type.append(qMakePair(tr("写寄存器功能码为16"), QString("bool")));
    properties_type.append(qMakePair(tr("8位逆序"), QString("bool")));
    properties_type.append(qMakePair(tr("16位低字节在前高字节在后"), QString("bool")));
    properties_type.append(qMakePair(tr("32位低字节在前高字节在后"), QString("bool")));
    properties_type.append(qMakePair(tr("64位低字节在前高字节在后"), QString("bool")));
}

///
/// \brief ModbusRTU::writeProperties
/// \details 保存属性
/// \param szProperties 属性字符串
/// \param properties 属性
///
void ModbusRTU::writeProperties(QString &szProperties, QVector<QPair<QString, QString>>& properties) {
    QStringList szListProperties;
    szListProperties << QString("%1=%2").arg("id").arg(getValue2ByValue1(tr("设备ID"), properties)); 
    szListProperties << QString("%1=%2").arg("bitMaxRegPacket").arg(getValue2ByValue1(tr("位组包最大寄存器个数"), properties));
    szListProperties << QString("%1=%2").arg("wordMaxRegPacket").arg(getValue2ByValue1(tr("字组包最大寄存器个数"), properties));
    szListProperties << QString("%1=%2").arg("commFailRetryTimes").arg(getValue2ByValue1(tr("失败重试次数"), properties));
    szListProperties << QString("%1=%2").arg("commTimeout").arg(getValue2ByValue1(tr("通讯超时时间(s)"), properties));
    szListProperties << QString("%1=%2").arg("commIntervalTime").arg(getValue2ByValue1(tr("通讯间隔时间(ms)"), properties));
    szListProperties << QString("%1=%2").arg("commResumeTime").arg(getValue2ByValue1(tr("尝试恢复间隔(s)"), properties));
    szListProperties << QString("%1=%2").arg("startAddrBit").arg(getValue2ByValue1(tr("内存地址起始位为0"), properties));
    szListProperties << QString("%1=%2").arg("writeCoilFn").arg(getValue2ByValue1(tr("写线圈功能码为15"), properties));
    szListProperties << QString("%1=%2").arg("writeRegFn").arg(getValue2ByValue1(tr("写寄存器功能码为16"), properties));
    szListProperties << QString("%1=%2").arg("addr8").arg(getValue2ByValue1(tr("8位逆序"), properties));
    szListProperties << QString("%1=%2").arg("addr16").arg(getValue2ByValue1(tr("16位低字节在前高字节在后"), properties));
    szListProperties << QString("%1=%2").arg("addr32").arg(getValue2ByValue1(tr("32位低字节在前高字节在后"), properties));
    szListProperties << QString("%1=%2").arg("addr64").arg(getValue2ByValue1(tr("64位低字节在前高字节在后"), properties));
    szProperties = szListProperties.join("|");
}

///
/// \brief ModbusRTU::readProperties
/// \details 加载属性
/// \param szProperties 属性字符串
/// \param properties 属性
///
void ModbusRTU::readProperties(QString &szProperties, QVector<QPair<QString, QString>>& properties) {
    properties.clear();
    QStringList szListProperties = szProperties.split("|");
    foreach(QString szKeyValue, szListProperties) {
        if (szKeyValue.startsWith("id=")) {
            QString val = szKeyValue.replace("id=", "");
            if(val == "") val = "1";
            properties.append(qMakePair(tr("设备ID"), val));
        }
        if (szKeyValue.startsWith("bitMaxRegPacket=")) {
            QString val = szKeyValue.replace("bitMaxRegPacket=", "");
            if(val == "") val = "100";
            properties.append(qMakePair(tr("位组包最大寄存器个数"), val));
        }
        if (szKeyValue.startsWith("wordMaxRegPacket=")) {
            QString val = szKeyValue.replace("wordMaxRegPacket=", "");
            if(val == "") val = "20";
            properties.append(qMakePair(tr("字组包最大寄存器个数"), val));
        }
        if (szKeyValue.startsWith("commFailRetryTimes=")) {
            QString val = szKeyValue.replace("commFailRetryTimes=", "");
            if(val == "") val = "2";
            properties.append(qMakePair(tr("失败重试次数"), val));
        }
        if (szKeyValue.startsWith("commTimeout=")) {
            QString val = szKeyValue.replace("commTimeout=", "");
            if(val == "") val = "1";
            properties.append(qMakePair(tr("通讯超时时间(s)"), val));
        }
        if (szKeyValue.startsWith("commIntervalTime=")) {
            QString val = szKeyValue.replace("commIntervalTime=", "");
            if(val == "") val = "200";
            properties.append(qMakePair(tr("通讯间隔时间(ms)"), val));
        }
        if (szKeyValue.startsWith("commResumeTime=")) {
            QString val = szKeyValue.replace("commResumeTime=", "");
            if(val == "") val = "2";
            properties.append(qMakePair(tr("尝试恢复间隔(s)"), val));
        }
        if (szKeyValue.startsWith("startAddrBit=")) {
            QString val = szKeyValue.replace("startAddrBit=", "");
            if(val == "") val = "true";
            properties.append(qMakePair(tr("内存地址起始位为0"), val));
        }
        if (szKeyValue.startsWith("writeCoilFn=")) {
            QString val = szKeyValue.replace("writeCoilFn=", "");
            if(val == "") val = "false";
            properties.append(qMakePair(tr("写线圈功能码为15"), val));
        }
        if (szKeyValue.startsWith("writeRegFn=")) {
            QString val = szKeyValue.replace("writeRegFn=", "");
            if(val == "") val = "false";
            properties.append(qMakePair(tr("写寄存器功能码为16"), val));
        }
        if (szKeyValue.startsWith("addr8=")) {
            QString val = szKeyValue.replace("addr8=", "");
            if(val == "") val = "false";
            properties.append(qMakePair(tr("8位逆序"), val));
        }
        if (szKeyValue.startsWith("addr16=")) {
            QString val = szKeyValue.replace("addr16=", "");
            if(val == "") val = "false";
            properties.append(qMakePair(tr("16位低字节在前高字节在后"), val));
        }
        if (szKeyValue.startsWith("addr32=")) {
            QString val = szKeyValue.replace("addr32=", "");
            if(val == "") val == "false";
            properties.append(qMakePair(tr("32位低字节在前高字节在后"), val));
        }
        if (szKeyValue.startsWith("addr64=")) {
            QString val = szKeyValue.replace("addr64=", "");
            if(val == "") val == "false";
            properties.append(qMakePair(tr("64位低字节在前高字节在后"), val));
        }
    }
}


///
/// \brief ModbusRTU::setDeviceProperty
/// \details 设置设备属性
/// \param properties
///
void ModbusRTU::setDeviceProperty(QVector<QPair<QString, QString>>& properties) {
    m_properties.clear();
    m_properties.append(properties);
    QString szVal = getValue2ByValue1(tr("内存地址起始位为0"), m_properties);
    m_bStartAddrBit0 = (szVal.toLower() == "true") ? true : false;
}

///
/// \brief ModbusRTU::getDeviceDescInfo
/// @details 获取设备描述信息
/// \return 设备描述信息
///
QString ModbusRTU::getDeviceDescInfo() {
#if 0
    <?xml version="1.0"/>
    <Device Name="ModbusRTU" AllDataType="bool|int16|uint16|int32|uint32|float32|double|bcd16|bcd32" SupportProtocol="ModbusRTU">
        <RegAreas>
            <RegArea Name="线圈状态" Alias="0x" Min="0x0000" Max="0xFFFF" DataType="bool" SubArea=""/>
            <RegArea Name="离散量状态" Alias="1x" Min="0x0000" Max="0xFFFF" DataType="bool" SubArea=""/>
            <RegArea Name="输入寄存器" Alias="3x" Min="0x0000" Max="0xFFFF" DataType="int16|uint16|int32|uint32|float32|double|bcd16|bcd32" SubArea=""/>
            <RegArea Name="保持寄存器" Alias="4x" Min="0x0000" Max="0xFFFF" DataType="int16|uint16|int32|uint32|float32|double|bcd16|bcd32" SubArea=""/>
        </RegAreas>
    </Device>
#endif
    QString szDeviceDescInfo;
    QStringList szListDataType;
    QStringList szListSubArea;

    QXmlStreamWriter writer(&szDeviceDescInfo);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("Device"); // <Device>
    writer.writeAttribute("Name", "ModbusRTU");

    szListDataType.clear();
    szListDataType << tr("bool")
                   << tr("int16")
                   << tr("uint16")
                   << tr("int32")
                   << tr("uint32")
                   << tr("float32")
                   << tr("double")
                   << tr("bcd16")
                   << tr("bcd32");
    writer.writeAttribute("AllDataType", szListDataType.join("|"));

    // 设备支持的所有协议
    QStringList szListSupportProtocol;
    szListSupportProtocol << "ModbusRTU";
    writer.writeAttribute("SupportProtocol", szListSupportProtocol.join("|"));

    // 设备支持的所有寄存器区
    writer.writeStartElement("RegAreas"); // <RegAreas>

    writer.writeStartElement("RegArea"); // <RegArea>
    writer.writeAttribute("Name", COIL_STATUS);
    writer.writeAttribute("Alias", "0x");
    writer.writeAttribute("Min", "0x0000");
    writer.writeAttribute("Max", "0xFFFF");
    szListDataType.clear();
    szListDataType << tr("bool");
    writer.writeAttribute("DataType", szListDataType.join("|"));
    szListSubArea.clear();
    writer.writeAttribute("SubArea", szListSubArea.join("|"));
    writer.writeEndElement(); // <RegArea/>

    writer.writeStartElement("RegArea"); // <RegArea>
    writer.writeAttribute("Name", DISCRETE_STATUS);
    writer.writeAttribute("Alias", "1x");
    writer.writeAttribute("Min", "0x0000");
    writer.writeAttribute("Max", "0xFFFF");
    szListDataType.clear();
    szListDataType << tr("bool");
    writer.writeAttribute("DataType", szListDataType.join("|"));
    szListSubArea.clear();
    writer.writeAttribute("SubArea", szListSubArea.join("|"));
    writer.writeEndElement(); // <RegArea/>

    writer.writeStartElement("RegArea"); // <RegArea>
    writer.writeAttribute("Name", INPUT_REGISTER);
    writer.writeAttribute("Alias", "3x");
    writer.writeAttribute("Min", "0x0000");
    writer.writeAttribute("Max", "0xFFFF");
    szListDataType.clear();
    szListDataType << tr("int16")
                   << tr("uint16")
                   << tr("int32")
                   << tr("uint32")
                   << tr("float32")
                   << tr("double")
                   << tr("bcd16")
                   << tr("bcd32");
    writer.writeAttribute("DataType", szListDataType.join("|"));
    szListSubArea.clear();
    writer.writeAttribute("SubArea", szListSubArea.join("|"));
    writer.writeEndElement(); // <RegArea/>

    writer.writeStartElement("RegArea"); // <RegArea>
    writer.writeAttribute("Name", HOLDING_REGISTER);
    writer.writeAttribute("Alias", "4x");
    writer.writeAttribute("Min", "0x0000");
    writer.writeAttribute("Max", "0xFFFF");
    szListDataType.clear();
    szListDataType << tr("int16")
                   << tr("uint16")
                   << tr("int32")
                   << tr("uint32")
                   << tr("float32")
                   << tr("double")
                   << tr("bcd16")
                   << tr("bcd32");
    writer.writeAttribute("DataType", szListDataType.join("|"));
    szListSubArea.clear();
    writer.writeAttribute("SubArea", szListSubArea.join("|"));

    writer.writeEndElement(); // <RegArea/>
    writer.writeEndElement(); // <RegAreas/>
    writer.writeEndElement(); // <Device/>
    writer.writeEndDocument();

    return szDeviceDescInfo;
}


