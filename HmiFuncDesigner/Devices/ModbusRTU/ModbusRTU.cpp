#include "ModbusRTU.h"
#include <QDebug>

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
            if(val == "") val = "false";
            properties.append(qMakePair(tr("32位低字节在前高字节在后"), val));
        }
        if (szKeyValue.startsWith("addr64=")) {
            QString val = szKeyValue.replace("addr64=", "");
            if(val == "") val = "false";
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


void ModbusRTU::loadInfos(QXmlStreamReader *r, QMap<QString, QVector<TTagInfo>> &infos, QString &dev)
{
    QString addr;
    TTagInfo info;
    if(r->name().toString() == "tag") {
        foreach(QXmlStreamAttribute attr, r->attributes()) {
            QString attrName = attr.name().toString();
            if(attrName == "addr"){
                addr = attr.value().toString();
            } else if(attrName == "id"){
                info.id = attr.value().toInt();
            } else if(attrName == "dev"){
                dev = attr.value().toString();
            } else if(attrName == "offset"){
                info.offset = attr.value().toInt();
            } else if(attrName == "type"){
                info.length = tagLength(attr.value().toString());
            }
        }

        if(infos.count(addr) > 0) {
            QVector<TTagInfo> &vecInfo = infos[addr];
            vecInfo.append(info);
        } else {
            QVector<TTagInfo> vecInfo;
            vecInfo.append(info);
            infos[addr] = vecInfo;
        }
    }
}

/**
 * @brief ModbusRTU::buildBlockReadTags 生成块读变量
 * @param xmlDevTags 同一设备的所有寄存器变量
 * @param xmlDevBlockReadTags 同一设备的所有块读变量
 * @param idToBlockId 变量ID对应的块读变量ID
 * @return true-成功, false-失败
 */
bool ModbusRTU::buildBlockReadTags(const QString &xmlDevTags, const QString &properties, QString &xmlDevBlockReadTags, QVector<QPair<QString, QString>>& idToBlockId)
{
#if 0
    xmlDevTags -->
    <tags>
        <tag addr="4x" addr2="" dev="ModbusRTU" group="" id="7" name="4x1" offset="0" offset2="0" remark="" type="uint16" unit="" writeable="1" blockReadId="60001"/>
        <tag addr="4x" addr2="" dev="ModbusRTU" group="" id="8" name="4x2" offset="1" offset2="0" remark="" type="uint16" unit="" writeable="1" blockReadId="60001"/>
        <tag addr="0x" addr2="" dev="ModbusRTU" group="" id="2" name="0x00" offset="0" offset2="0" remark="" type="bool" unit="" writeable="1" blockReadId="60002"/>
    </tags>

    xmlDevBlockReadTags -->
    <block_tags>
        <tag addr="4x" addr2="" dev="ModbusRTU" group="" id="60001" name="4x1" offset="0" offset2="0" remark="" type="4:reg" unit="" writeable="0" blockReadId=""/>
        <tag addr="0x" addr2="" dev="ModbusRTU" group="" id="60002" name="0x00" offset="0" offset2="0" remark="" type="1:reg" unit="" writeable="0" blockReadId=""/>
    </block_tags>
#endif

    int bitMaxRegPacket = 0;
    int wordMaxRegPacket = 0;

    QStringList szListProperties = properties.split("|");
    foreach(QString szKeyValue, szListProperties) {
        if (szKeyValue.startsWith("bitMaxRegPacket=")) {
            QString val = szKeyValue.replace("bitMaxRegPacket=", "").trimmed();
            if(!val.isEmpty()) {
                bitMaxRegPacket = val.toInt();
            }
        }
        if (szKeyValue.startsWith("wordMaxRegPacket=")) {
            QString val = szKeyValue.replace("wordMaxRegPacket=", "").trimmed();
            if(!val.isEmpty()) {
                wordMaxRegPacket = val.toInt();
            }
        }
    }

    // 不需要块读
    if(bitMaxRegPacket <= 0 && wordMaxRegPacket <= 0) {
        return false;
    }

    QString dev = "";
    QMap<QString, QVector<TTagInfo>> mapInfos;

    // 解析设备变量节点
    QXmlStreamReader r(xmlDevTags);
    while(!r.atEnd() && !r.hasError()) {
        if(r.readNext() == QXmlStreamReader::StartElement) {
            if(r.name() == "tag") {
                loadInfos(&r, mapInfos, dev);
            }
        }
    }
#if 0
    {
        qDebug() << "dev: " << dev;
        QList<QString> memInfo = mapInfos.keys();
        qDebug() << "memInfo: " << memInfo;
        foreach (QString info, memInfo) {
            QVector<TTagInfo> &vecInfo = mapInfos[info];
            foreach(TTagInfo tagInfo, vecInfo) {
                qDebug() << "tagInfo: " << tagInfo.id << tagInfo.offset << tagInfo.length;
            }
        }
    }
#endif
    ///////////////////////////////////////////////////////////////////////////
    /// 生成打包变量
    ///
    quint16 min0xAddr = 0xffff;
    quint16 max0xAddr = 0;
    quint16 var0xCnt = 0;

    quint16 min1xAddr = 0xffff;
    quint16 max1xAddr = 0;
    quint16 var1xCnt = 0;

    quint16 min3xAddr = 0xffff;
    quint16 max3xAddr = 0;
    quint16 var3xCnt = 0;

    quint16 min4xAddr = 0xffff;
    quint16 max4xAddr = 0;
    quint16 var4xCnt = 0;

    QList<QString> memInfo = mapInfos.keys();
    foreach (QString info, memInfo) {
        QVector<TTagInfo> &vecInfo = mapInfos[info];
        foreach(TTagInfo tagInfo, vecInfo) {
            if(info == "0x") {
                if(min0xAddr > tagInfo.offset) {
                    min0xAddr = tagInfo.offset;
                }
                if(max0xAddr <= tagInfo.offset + tagInfo.length) {
                    max0xAddr = tagInfo.offset + tagInfo.length;
                }
                var0xCnt++;
            } else if(info == "1x") {
                if(min1xAddr > tagInfo.offset) {
                    min1xAddr = tagInfo.offset;
                }
                if(max1xAddr <= tagInfo.offset + tagInfo.length) {
                    max1xAddr = tagInfo.offset + tagInfo.length;
                }
                var1xCnt++;
            } else if(info == "3x") {
                if(min3xAddr > tagInfo.offset) {
                    min3xAddr = tagInfo.offset;
                }
                if(max3xAddr <= tagInfo.offset + tagInfo.length / 2 - 1) {
                    max3xAddr = tagInfo.offset + tagInfo.length / 2 - 1;
                }
                var3xCnt++;
            } else if(info == "4x") {
                if(min4xAddr > tagInfo.offset) {
                    min4xAddr = tagInfo.offset;
                }
                if(max4xAddr <= tagInfo.offset + tagInfo.length / 2 - 1) {
                    max4xAddr = tagInfo.offset + tagInfo.length / 2 - 1;
                }
                var4xCnt++;
            }
        }
    }
#if 0
    qDebug() << "0x number: " << var0xCnt << ", min addr: " << min0xAddr << ", max addr: " << max0xAddr;
    qDebug() << "1x number: " << var1xCnt << ", min addr: " << min1xAddr << ", max addr: " << max1xAddr;
    qDebug() << "3x number: " << var3xCnt << ", min addr: " << min3xAddr << ", max addr: " << max3xAddr;
    qDebug() << "4x number: " << var4xCnt << ", min addr: " << min4xAddr << ", max addr: " << max4xAddr;
#endif
    QMap<QString, QVector<TTagInfo *>> mapBlockInfos;
    QVector<TTagInfo *> vecInfo0x;
    mapBlockInfos["0x"] = vecInfo0x;

    QVector<TTagInfo *> vecInfo1x;
    mapBlockInfos["1x"] = vecInfo1x;

    QVector<TTagInfo *> vecInfo3x;
    mapBlockInfos["3x"] = vecInfo3x;

    QVector<TTagInfo *> vecInfo4x;
    mapBlockInfos["4x"] = vecInfo4x;

    int iNextPackageVarID = 60001;

    // 创建0x组包变量
    if(var0xCnt > 1) {
        int num = (max0xAddr - min0xAddr + 1) / bitMaxRegPacket;
        if(((max0xAddr - min0xAddr + 1) % bitMaxRegPacket) > 0) {
            num++;
        }

        //qDebug() << "0x pack variable number: " << num;

        for(int j=0; j<num; j++) {
            TTagInfo *pInfoObj = new TTagInfo;
            pInfoObj->id = iNextPackageVarID;
            pInfoObj->offset = min0xAddr + j * bitMaxRegPacket;
            pInfoObj->length = bitMaxRegPacket;
            pInfoObj->use = false;
            iNextPackageVarID++;
            mapBlockInfos["0x"].append(pInfoObj);
        }
    }

    // 创建1x组包变量
    if(var1xCnt > 1) {
        int num = (max1xAddr - min1xAddr + 1) / bitMaxRegPacket;
        if(((max1xAddr - min1xAddr + 1) % bitMaxRegPacket) > 0) {
            num++;
        }

        //qDebug() << "1x pack variable number: " << num;

        for(int j=0; j<num; j++) {
            TTagInfo *pInfoObj = new TTagInfo;
            pInfoObj->id = iNextPackageVarID;
            pInfoObj->offset = min1xAddr + j * bitMaxRegPacket;
            pInfoObj->length = bitMaxRegPacket;
            pInfoObj->use = false;
            iNextPackageVarID++;
            mapBlockInfos["1x"].append(pInfoObj);
        }
    }

    // 创建3x组包变量
    if(var3xCnt > 1) {
        int num = (max3xAddr - min3xAddr + 1) / wordMaxRegPacket;
        if(((max3xAddr - min3xAddr + 1) % wordMaxRegPacket) > 0) {
            num++;
        }

        //qDebug() << "3x pack variable number: " << num;

        for(int j=0; j<num; j++) {
            TTagInfo *pInfoObj = new TTagInfo;
            pInfoObj->id = iNextPackageVarID;
            pInfoObj->offset = min3xAddr + j * wordMaxRegPacket;
            pInfoObj->length = wordMaxRegPacket;
            pInfoObj->use = false;
            iNextPackageVarID++;
            mapBlockInfos["3x"].append(pInfoObj);
        }
    }

    // 创建4x组包变量
    if(var4xCnt > 1) {
        int num = (max4xAddr - min4xAddr + 1) / wordMaxRegPacket;
        if(((max4xAddr - min4xAddr + 1) % wordMaxRegPacket) > 0) {
            num++;
        }

        //qDebug() << "4x pack variable number: " << num;

        for(int j=0; j<num; j++) {
            TTagInfo *pInfoObj = new TTagInfo;
            pInfoObj->id = iNextPackageVarID;
            pInfoObj->offset = min4xAddr + j * wordMaxRegPacket;
            pInfoObj->length = wordMaxRegPacket;
            pInfoObj->use = false;
            iNextPackageVarID++;
            mapBlockInfos["4x"].append(pInfoObj);
        }
    }
    ///////////////////////////////////////////////////////////////////////////
    /// 变量关联组包变量
    ///
    QXmlStreamWriter writer(&xmlDevBlockReadTags);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("block_tags"); // <block_tags>

    QVector<TTagInfo *> &vecTagInfo0x = mapBlockInfos["0x"];
    quint32 dwFindMax0xAddr = 0; // 包内的最大地址
    quint32 dwFindMin0xAddr = 0xffff; // 包内的最小地址
    foreach(TTagInfo *pObj, vecTagInfo0x) {
        quint32 iMinAddrOffset = pObj->offset;
        quint32 iMaxAddrOffset = pObj->length;
        dwFindMax0xAddr = iMinAddrOffset;
        dwFindMin0xAddr = iMaxAddrOffset;
        QList<QString> memInfo = mapInfos.keys();
        foreach (QString info, memInfo) {
            if(info != "0x") {
                continue;
            }
            QVector<TTagInfo> &vecInfo = mapInfos[info];
            foreach(TTagInfo tagInfo, vecInfo) {
                if(tagInfo.offset >= iMinAddrOffset && (tagInfo.offset + tagInfo.length) <= iMaxAddrOffset) {
                    pObj->use = true;
                    QPair<QString, QString> idPair;
                    idPair.first = QString::number(tagInfo.id);
                    idPair.second = QString::number(pObj->id);
                    idToBlockId.append(idPair);
                    if((tagInfo.offset + tagInfo.length) > dwFindMax0xAddr) {
                        dwFindMax0xAddr = tagInfo.offset + tagInfo.length;
                    }
                    if(dwFindMin0xAddr > tagInfo.offset) {
                        dwFindMin0xAddr = tagInfo.offset;
                    }
                }
            }
        }
        writer.writeStartElement("tag"); // <tag>
        writer.writeAttribute("addr", "0x");
        writer.writeAttribute("addr2", "");
        writer.writeAttribute("dev", dev);
        writer.writeAttribute("group", "");
        writer.writeAttribute("id", QString::number(pObj->id));
        writer.writeAttribute("name", QString("0x_%1").arg(QString::number(pObj->id)));
        writer.writeAttribute("offset", QString::number(pObj->offset));
        writer.writeAttribute("offset2", "");
        writer.writeAttribute("remark", "");
        writer.writeAttribute("type", QString("%1:reg").arg(QString::number(dwFindMax0xAddr - dwFindMin0xAddr)));
        writer.writeAttribute("unit", "");
        writer.writeAttribute("writeable", "0");
        writer.writeAttribute("blockReadId", "");
        writer.writeEndElement(); // <tag/>
    }

    QVector<TTagInfo *> &vecTagInfo1x = mapBlockInfos["1x"];
    quint32 dwFindMax1xAddr = 0; // 包内的最大地址
    quint32 dwFindMin1xAddr = 0xffff; // 包内的最小地址
    foreach(TTagInfo *pObj, vecTagInfo1x) {
        quint32 iMinAddrOffset = pObj->offset;
        quint32 iMaxAddrOffset = pObj->length;
        dwFindMax1xAddr = iMinAddrOffset;
        dwFindMin1xAddr = iMaxAddrOffset;
        QList<QString> memInfo = mapInfos.keys();
        foreach (QString info, memInfo) {
            if(info != "1x") {
                continue;
            }
            QVector<TTagInfo> &vecInfo = mapInfos[info];
            foreach(TTagInfo tagInfo, vecInfo) {
                if(tagInfo.offset >= iMinAddrOffset && (tagInfo.offset + tagInfo.length) <= iMaxAddrOffset) {
                    pObj->use = true;
                    QPair<QString, QString> idPair;
                    idPair.first = QString::number(tagInfo.id);
                    idPair.second = QString::number(pObj->id);
                    idToBlockId.append(idPair);
                    if((tagInfo.offset + tagInfo.length) > dwFindMax1xAddr) {
                        dwFindMax1xAddr = tagInfo.offset + tagInfo.length;
                    }
                    if(dwFindMin1xAddr > tagInfo.offset) {
                        dwFindMin1xAddr = tagInfo.offset;
                    }
                }
            }
        }
        writer.writeStartElement("tag"); // <tag>
        writer.writeAttribute("addr", "1x");
        writer.writeAttribute("addr2", "");
        writer.writeAttribute("dev", dev);
        writer.writeAttribute("group", "");
        writer.writeAttribute("id", QString::number(pObj->id));
        writer.writeAttribute("name", QString("1x_%1").arg(QString::number(pObj->id)));
        writer.writeAttribute("offset", QString::number(pObj->offset));
        writer.writeAttribute("offset2", "");
        writer.writeAttribute("remark", "");
        writer.writeAttribute("type", QString("%1:reg").arg(QString::number(dwFindMax1xAddr - dwFindMin1xAddr)));
        writer.writeAttribute("unit", "");
        writer.writeAttribute("writeable", "0");
        writer.writeAttribute("blockReadId", "");
        writer.writeEndElement(); // <tag/>
    }

    QVector<TTagInfo *> &vecTagInfo3x = mapBlockInfos["3x"];
    quint32 dwFindMax3xAddr = 0; // 包内的最大地址
    quint32 dwFindMin3xAddr = 0xffff; // 包内的最小地址
    foreach(TTagInfo *pObj, vecTagInfo3x) {
        quint32 iMinAddrOffset = pObj->offset;
        quint32 iMaxAddrOffset = pObj->length;
        dwFindMax3xAddr = iMinAddrOffset;
        dwFindMin3xAddr = iMaxAddrOffset;
        QList<QString> memInfo = mapInfos.keys();
        foreach (QString info, memInfo) {
            if(info != "3x") {
                continue;
            }
            QVector<TTagInfo> &vecInfo = mapInfos[info];
            foreach(TTagInfo tagInfo, vecInfo) {
                if(tagInfo.offset >= iMinAddrOffset && (tagInfo.offset + tagInfo.length) <= iMaxAddrOffset) {
                    pObj->use = true;
                    QPair<QString, QString> idPair;
                    idPair.first = QString::number(tagInfo.id);
                    idPair.second = QString::number(pObj->id);
                    idToBlockId.append(idPair);
                    if((tagInfo.offset + tagInfo.length) > dwFindMax3xAddr) {
                        dwFindMax3xAddr = tagInfo.offset + tagInfo.length / 2;
                    }
                    if(dwFindMin3xAddr > tagInfo.offset) {
                        dwFindMin3xAddr = tagInfo.offset;
                    }
                }
            }
        }
        writer.writeStartElement("tag"); // <tag>
        writer.writeAttribute("addr", "3x");
        writer.writeAttribute("addr2", "");
        writer.writeAttribute("dev", dev);
        writer.writeAttribute("group", "");
        writer.writeAttribute("id", QString::number(pObj->id));
        writer.writeAttribute("name", QString("3x_%1").arg(QString::number(pObj->id)));
        writer.writeAttribute("offset", QString::number(pObj->offset));
        writer.writeAttribute("offset2", "");
        writer.writeAttribute("remark", "");
        writer.writeAttribute("type", QString("%1:reg").arg(QString::number(dwFindMax3xAddr - dwFindMin3xAddr)));
        writer.writeAttribute("unit", "");
        writer.writeAttribute("writeable", "0");
        writer.writeAttribute("blockReadId", "");
        writer.writeEndElement(); // <tag/>
    }

    QVector<TTagInfo *> &vecTagInfo4x = mapBlockInfos["4x"];
    quint32 dwFindMax4xAddr = 0; // 包内的最大地址
    quint32 dwFindMin4xAddr = 0xffff; // 包内的最小地址
    foreach(TTagInfo *pObj, vecTagInfo4x) {
        quint32 iMinAddrOffset = pObj->offset;
        quint32 iMaxAddrOffset = pObj->length;
        dwFindMax4xAddr = iMinAddrOffset;
        dwFindMin4xAddr = iMaxAddrOffset;
        QList<QString> memInfo = mapInfos.keys();
        foreach (QString info, memInfo) {
            if(info != "4x") {
                continue;
            }
            QVector<TTagInfo> &vecInfo = mapInfos[info];
            foreach(TTagInfo tagInfo, vecInfo) {
                if(tagInfo.offset >= iMinAddrOffset && (tagInfo.offset + tagInfo.length) <= iMaxAddrOffset) {
                    pObj->use = true;
                    QPair<QString, QString> idPair;
                    idPair.first = QString::number(tagInfo.id);
                    idPair.second = QString::number(pObj->id);
                    idToBlockId.append(idPair);
                    if((tagInfo.offset + tagInfo.length) > dwFindMax4xAddr) {
                        dwFindMax4xAddr = tagInfo.offset + tagInfo.length / 2;
                    }
                    if(dwFindMin4xAddr > tagInfo.offset) {
                        dwFindMin4xAddr = tagInfo.offset;
                    }
                }
            }
        }
        writer.writeStartElement("tag"); // <tag>
        writer.writeAttribute("addr", "4x");
        writer.writeAttribute("addr2", "");
        writer.writeAttribute("dev", dev);
        writer.writeAttribute("group", "");
        writer.writeAttribute("id", QString::number(pObj->id));
        writer.writeAttribute("name", QString("4x_%1").arg(QString::number(pObj->id)));
        writer.writeAttribute("offset", QString::number(pObj->offset));
        writer.writeAttribute("offset2", "");
        writer.writeAttribute("remark", "");
        writer.writeAttribute("type", QString("%1:reg").arg(QString::number(dwFindMax4xAddr - dwFindMin4xAddr)));
        writer.writeAttribute("unit", "");
        writer.writeAttribute("writeable", "0");
        writer.writeAttribute("blockReadId", "");
        writer.writeEndElement(); // <tag/>
    }

    writer.writeEndElement(); // <block_tags/>
    writer.writeEndDocument();

    qDeleteAll(vecTagInfo0x);
    qDeleteAll(vecTagInfo1x);
    qDeleteAll(vecTagInfo3x);
    qDeleteAll(vecTagInfo4x);
    mapBlockInfos.clear();

    return true;
}
