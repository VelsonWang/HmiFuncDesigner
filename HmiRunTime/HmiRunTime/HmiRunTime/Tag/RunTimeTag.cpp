#include "RunTimeTag.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtDebug>


RunTimeTag::RunTimeTag(QObject *parent) : QObject(parent), id(0)
{

}

RunTimeTag::~RunTimeTag()
{

}


RunTimeTag::RunTimeTag(const RunTimeTag &obj)
{
    copyObject(obj);
}


RunTimeTag &RunTimeTag::operator=(const RunTimeTag &obj)
{
    copyObject(obj);
    return *this;
}

void RunTimeTag::copyObject(const RunTimeTag &obj)
{
    m_szName = obj.m_szName;
    m_szUnit = obj.m_szUnit;
    m_szAddrType = obj.m_szAddrType;
    m_szAddrOffset = obj.m_szAddrOffset;
    m_szAddrType2 = obj.m_szAddrType2;
    m_szAddrOffset2 = obj.m_szAddrOffset2;
    m_szDataType = obj.m_szDataType;
    m_iWriteable = obj.m_iWriteable;
    m_szRemark = obj.m_szRemark;
    m_szOwnGroup = obj.m_szOwnGroup;
    m_szDevType = obj.m_szDevType;
}


void RunTimeTag::copyFromTag(RunTimeTag obj)
{
    m_szName = obj.m_szName;
    m_szUnit = obj.m_szUnit;
    m_szAddrType = obj.m_szAddrType;
    m_szAddrOffset = obj.m_szAddrOffset;
    m_szAddrType2 = obj.m_szAddrType2;
    m_szAddrOffset2 = obj.m_szAddrOffset2;
    m_szDataType = obj.m_szDataType;
    m_iWriteable = obj.m_iWriteable;
    m_szRemark = obj.m_szRemark;
    m_szOwnGroup = obj.m_szOwnGroup;
    m_szDevType = obj.m_szDevType;
}


#include "tag.h"
#include "device.h"
#include "moduletampersetting.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "log.h"
#include "cstring.h"
#include "configwarning.h"
#include "taggroup.h"
#include "util.h"
#include "collectorbox.h"

//----------------------------------------------------------------------------

// BOX对象
extern PCollectorBox g_pBox;



//
// 配置数据类型字符转数据类型枚举
// \param [in] 配置数据类型字符
// \return 数据类型枚举值
//
TDataType getDataType(PINT8S pszDataType)
{
    TDataType ret = TYPE_NONE;
    if(cstr_equals(pszDataType, "bool")) {
        ret = TYPE_BOOL;
    } else if(cstr_equals(pszDataType, "int8")) {
        ret = TYPE_INT8;
    } else if(cstr_equals(pszDataType, "uint8")) {
        ret = TYPE_UINT8;
    } else if(cstr_equals(pszDataType, "bcd8")) {
        ret = TYPE_BCD8;
    } else if(cstr_equals(pszDataType, "int16")) {
        ret = TYPE_INT16;
    } else if(cstr_equals(pszDataType, "uint16")) {
        ret = TYPE_UINT16;
    } else if(cstr_equals(pszDataType, "bcd16")) {
        ret = TYPE_BCD16;
    } else if(cstr_equals(pszDataType, "int32")) {
        ret = TYPE_INT32;
    } else if(cstr_equals(pszDataType, "uint32")) {
        ret = TYPE_UINT32;
    } else if(cstr_equals(pszDataType, "float32")) {
        ret = TYPE_FLOAT32;
    } else if(cstr_equals(pszDataType, "bcd32")) {
        ret = TYPE_BCD32;
    } else if(cstr_equals(pszDataType, "double")) {
        ret = TYPE_DOUBLE;
    } else if(cstr_equals(pszDataType, "bytes")) {
        ret = TYPE_BYTES;
    }
    return ret;
}

//----------------------------------------------------------------------------

static IPubObj IPackageRead;


// 组包读取描述对象
PVOID PackageRead_CreateObject(void)
{
    PPackageReadObj pThis = (PPackageReadObj)malloc(sizeof(TPackageReadObj));
    memset((PVOID)pThis, 0, sizeof(TPackageReadObj));
    pThis->pPubObj = &IPackageRead;
    memset(pThis->szAddrType, 0, sizeof(pThis->szAddrType) / sizeof(INT8S));
    pThis->iAddrOffsetMin = UINT_MAX;
    pThis->iAddrOffsetMax = 0;
    INIT_LIST_HEAD(&pThis->list);
    INIT_LIST_HEAD(&pThis->listTags);
    INIT_LIST_HEAD(&pThis->listPackageTags);
    return (PVOID)pThis;
}


///////////////////////////////////////////////////////////

//从XML节点获取配置的数据
static void PackageRead_GetXmlData(PVOID pObj, mxml_node_t* pNode, mxml_node_t *pTop)
{
    (void)pObj;
    (void)pNode;
    (void)pTop;
}

// 终端打印数据
static void PackageRead_PrintShow(PVOID pObj)
{
    PPackageReadObj pThis =  (PPackageReadObj)pObj;
    char *buf = calloc(1, 1024);

    sprintf(buf, "AddrType: %s, min: %d, max: %d", pThis->szAddrType, pThis->iAddrOffsetMin, pThis->iAddrOffsetMax);

    log_trace("---------------<PackageRead>--------------->>");
    log_trace("[%s]", buf);
    free(buf);

    struct list_head *pos;
    list_for_each(pos, &pThis->listPackageTags) {
        PTag pTag = list_entry(pos, TTag, list);
        pTag->pPubObj->pfPrintShow(pTag);
    }

    pos = NULL;
    list_for_each(pos, &pThis->listTags) {
        PTag pTag = list_entry(pos, TTag, listInPackage);
        pTag->pPubObj->pfPrintShow(pTag);
    }

    log_trace("---------------<PackageRead>---------------<<");

}

// 释放资源
static void PackageRead_Free(PVOID pObj)
{
    PPackageReadObj pThis = (PPackageReadObj)pObj;
    log_trace("free PackageRead");

    struct list_head *pos;
    for (pos = (&pThis->listPackageTags)->next; pos != (&pThis->listPackageTags); ) {
        PTag pTag = list_entry(pos, TTag, list);
        pos = pos->next;
        pTag->pPubObj->pfFree(pTag);
    }

    free(pThis);
}

////////////////////////////////////////////////////////////////////////////


static IPubObj IPackageRead = {
    PackageRead_GetXmlData,
    PackageRead_PrintShow,
    PackageRead_Free,
};


////////////////////////////////////////////////////////////////////////////



static IPubObj ITag;



//
// 检查是否需要触发告警
//
static void Tag_CheckWarnings(PTag pTag)
{
    struct list_head *pos;
    list_for_each(pos, &pTag->listConfigWarnings) {
        PConfigWarning pWarningObj = list_entry(pos, TConfigWarning, list);
        PINT8S pszValue = Tag_GetValueString(pTag);
        if(pszValue != NULL) {
            BOOLEAN bWarnings = FALSE;
            FLOAT64 dValue = strtod(pszValue, NULL);

            switch (pWarningObj->alarmExp.iExp) {
                case EXP_EQ: // 等于 "x=1"
                    if(dValue == pWarningObj->alarmExp.dRight) {
                        bWarnings = TRUE;
                    }
                    break;
                case EXP_NE: // 不等于 "x!=1"
                    if(dValue != pWarningObj->alarmExp.dRight) {
                        bWarnings = TRUE;
                    }
                    break;
                case EXP_LT: // 小于 "x<1"
                    if(dValue < pWarningObj->alarmExp.dRight) {
                        bWarnings = TRUE;
                    }
                    break;
                case EXP_LE: // 小于等于 "x<=1"
                    if(dValue <= pWarningObj->alarmExp.dRight) {
                        bWarnings = TRUE;
                    }
                    break;
                case EXP_GT: // 大于 "x>1"
                    if(dValue > pWarningObj->alarmExp.dRight) {
                        bWarnings = TRUE;
                    }
                    break;
                case EXP_GE: // 大于等于 "x>=1"
                    if(dValue >= pWarningObj->alarmExp.dRight) {
                        bWarnings = TRUE;
                    }
                    break;
                case EXP_GT_LT: // 大于&小于 "0<x<1"
                    if(dValue > pWarningObj->alarmExp.dLeft && dValue < pWarningObj->alarmExp.dRight) {
                        bWarnings = TRUE;
                    }
                    break;
                case EXP_GE_LT: // 大于等于&小于 "0<=x<1"
                    if(dValue >= pWarningObj->alarmExp.dLeft && dValue < pWarningObj->alarmExp.dRight) {
                        bWarnings = TRUE;
                    }
                    break;
                case EXP_GT_LE: // 大于&小于等于 "0<x<=1"
                    if(dValue > pWarningObj->alarmExp.dLeft && dValue <= pWarningObj->alarmExp.dRight) {
                        bWarnings = TRUE;
                    }
                    break;
                case EXP_GE_LE: // 大于等于&小于等于 "0<=x<=1"
                    if(dValue >= pWarningObj->alarmExp.dLeft && dValue <= pWarningObj->alarmExp.dRight) {
                        bWarnings = TRUE;
                    }
                    break;
                default:
                    break;
            }

            // 执行报警动作
            if(pWarningObj->bWarningsAction == FALSE && bWarnings == TRUE) {
                struct list_head *posAlarmAction;
                list_for_each(posAlarmAction, &pWarningObj->listAlarmActions) {
                    PAlarmAction pAlarmActionObj = list_entry(posAlarmAction, TAlarmAction, list);
                    //printf("do alarm action logic!! tag[id: %d, value: %s]\n\n", pAlarmActionObj->iTagID, pAlarmActionObj->szTagValue);
                    PTag pFoundTag = CollectorBox_FindTagObjByID(g_pBox, pAlarmActionObj->iTagID);
                    if(pFoundTag != NULL) {
                        Tag_SetWriteBuffer(pFoundTag, pFoundTag->iDataType, pAlarmActionObj->szTagValue);
                    }
                }
                pWarningObj->bWarningsAction = TRUE;
            } else if(pWarningObj->bWarningsAction == TRUE && bWarnings == FALSE) {
                pWarningObj->bWarningsAction = FALSE;
            }

            if(pTag->bOldWarnings == FALSE && bWarnings == TRUE) {
                pTag->bWarnings = TRUE;
            } else if(pTag->bOldWarnings == TRUE && bWarnings == FALSE) {
                if(pWarningObj->iPushRestoreMessage == 1) { // 推送报警恢复消
                    pTag->bWarnings = FALSE;
                } else {
                    pTag->bWarnings = FALSE;
                    pTag->bOldWarnings = FALSE;
                }
            }
        }
        free(pszValue);
    }
}

//
//线性换算
// Y = kX + b
// k = (OUTmax-OUTmin) / (INmax-INmin)
// b = (INmax * OUTmin-INmin * OUTmax) / (INmax-INmin)
//
static void Tag_ScaleCalculate(PTag pTag)
{
    if(pTag->bLineScale == TRUE) { // 启用线性换算
        FLOAT64 dInputMin = strtod(pTag->szInputMin, NULL);
        FLOAT64 dInputMax = strtod(pTag->szInputMax, NULL);
        FLOAT64 dOutputMin = strtod(pTag->szOutputMin, NULL);
        FLOAT64 dOutputMax = strtod(pTag->szOutputMax, NULL);
        if(dInputMax == dInputMin) {
            log_error("dInputMax == dInputMin division zero");
            return;
        }

        //log_trace("dInputMin: %lf, dInputMax: %lf, dOutputMin: %lf, dOutputMax: %lf", dInputMin, dInputMax, dOutputMin, dOutputMax);
        PINT8S pszTagValue = Tag_ValueToString(pTag->iDataType, pTag->readBuf);
        if(pszTagValue != NULL) {
            FLOAT64 dTagValue = strtod(pszTagValue, NULL);
            FLOAT64 k = (dOutputMax - dOutputMin) / (dInputMax - dInputMin);
            FLOAT64 b = (dInputMax * dOutputMin - dInputMin * dOutputMax) / (dInputMax - dInputMin);
            FLOAT64 dNewTagValue = dTagValue * k + b;
            //log_trace("k: %lf, b: %lf, tag value: %lf", k, b, dTagValue);
            memset(pTag->readBuf, 0, TAG_BUFFER_SIZE);
            if(pTag->iOutputDataType == TYPE_BOOL) {
                pTag->readBuf[0] = (dNewTagValue != 0) ? 1 : 0;
            } else if(pTag->iOutputDataType == TYPE_INT8) {
                INT8 iValue = (INT8)(dNewTagValue + 0.5);
                memcpy(pTag->readBuf, (PVOID)&iValue, 1);
            } else if(pTag->iOutputDataType == TYPE_UINT8) {
                INT8U iValue = (INT8U)(dNewTagValue + 0.5);
                memcpy(pTag->readBuf, (PVOID)&iValue, 1);
            } else if(pTag->iOutputDataType == TYPE_INT16) {
                INT16 iValue = (INT16)(dNewTagValue + 0.5);
                memcpy(pTag->readBuf, (PVOID)&iValue, 2);
            } else if(pTag->iOutputDataType == TYPE_UINT16) {
                INT16U iValue = (INT16U)(dNewTagValue + 0.5);
                memcpy(pTag->readBuf, (PVOID)&iValue, 2);
            } else if(pTag->iOutputDataType == TYPE_INT32) {
                INT32 iValue = (INT32)(dNewTagValue + 0.5);
                memcpy(pTag->readBuf, (PVOID)&iValue, 4);
            } else if(pTag->iOutputDataType == TYPE_UINT32) {
                INT32U iValue = (INT32U)(dNewTagValue + 0.5);
                memcpy(pTag->readBuf, (PVOID)&iValue, 4);
            } else if(pTag->iOutputDataType == TYPE_FLOAT32) {
                FLOAT32 iValue = (FLOAT32)dNewTagValue;
                memcpy(pTag->readBuf, (PVOID)&iValue, 4);
            } else if(pTag->iOutputDataType == TYPE_DOUBLE) {
                memcpy(pTag->readBuf, (PVOID)&dNewTagValue, 8);
            }
        }
        free(pszTagValue);
    }
}


//
// 读取变量缓冲区更新
//
void Tag_ValueBufferUpdated(PTag pTag)
{
    if(pTag->iDataType == TYPE_BYTES) {
        return;
    }

    //线性换算
    Tag_ScaleCalculate(pTag);

    for(int i = 0; i < TAG_BUFFER_SIZE; i++) {
        if(pTag->readBuf[i] != pTag->readBufOld[i]) {
            pTag->bValueChanged = TRUE;
            // 触发报警
            Tag_CheckWarnings(pTag);
            memset((PVOID)pTag->readBufOld, 0, TAG_BUFFER_SIZE);
            memcpy((PVOID)pTag->readBufOld, (PVOID)pTag->readBuf, TAG_BUFFER_SIZE);
            if(g_bNeedPushDataToCloud == TRUE) {
                sem_post(&g_semPushDataToCloud);
            }
            break;
        }
    }
}


PVOID Tag_CreateObject(void)
{
    PTag pThis = (PTag)malloc(sizeof(TTag));
    memset((PVOID)pThis, 0, sizeof(TTag));
    pThis->pPubObj = &ITag;
    pThis->pfValueBufferUpdated = Tag_ValueBufferUpdated;
    pThis->pfCheckWarnings = Tag_CheckWarnings;

    pThis->iID = 0;
    memset(pThis->szName, 0, sizeof(pThis->szName) / sizeof(INT8S));
    memset(pThis->szUnit, 0, sizeof(pThis->szUnit) / sizeof(INT8S));
    memset(pThis->szAddrType, 0, sizeof(pThis->szAddrType) / sizeof(INT8S));
    pThis->iAddrOffset = 0;
    pThis->iAddrByteOffset = 0;
    pThis->iAddrBitOffset = 0;
    memset(pThis->szAddrType2, 0, sizeof(pThis->szAddrType2) / sizeof(INT8S));
    pThis->iAddrOffset2 = 0;
    pThis->iAddrByteOffset2 = 0;
    pThis->iAddrBitOffset2 = 0;
    pThis->iDataType = TYPE_NONE;
    pThis->iWriteable = 0;
    memset(pThis->szRemark, 0, sizeof(pThis->szRemark) / sizeof(INT8S));
    memset(pThis->szOwnGroup, 0, sizeof(pThis->szOwnGroup) / sizeof(INT8S));

    pThis->bLineScale = FALSE;
    memset(pThis->szInputMin, 0, sizeof(pThis->szInputMin) / sizeof(INT8S));
    memset(pThis->szInputMax, 0, sizeof(pThis->szInputMax) / sizeof(INT8S));
    memset(pThis->szOutputMin, 0, sizeof(pThis->szOutputMin) / sizeof(INT8S));
    memset(pThis->szOutputMax, 0, sizeof(pThis->szOutputMax) / sizeof(INT8S));
    pThis->iOutputDataType = TYPE_NONE;

    INIT_LIST_HEAD(&pThis->listConfigWarnings);

    pThis->iSaveType = 0;
    memset(pThis->szSaveValue, 0, sizeof(pThis->szSaveValue) / sizeof(INT8S));

    pThis->bEnSafeLevel = FALSE;
    pThis->iSafeLevel = 0;

    memset(pThis->readBuf, 0, TAG_BUFFER_SIZE);
    memset(pThis->readBufOld, 0, TAG_BUFFER_SIZE);
    memset(pThis->writeBuf, 0, TAG_BUFFER_SIZE);

    pThis->bWriteToDevice = FALSE;
    pThis->iPackageReadID = -1;
    pThis->iAddrOffsetInPackage = 0;
    pThis->bValueChanged = FALSE;
    pThis->bWarnings = FALSE;
    pThis->bOldWarnings = FALSE;

    INIT_LIST_HEAD(&pThis->list);
    INIT_LIST_HEAD(&pThis->listInPackage);

    return (PVOID)pThis;
}

///////////////////////////////////////////////////////////

//从XML节点获取配置的数据
static void Tag_GetXmlData(PVOID pObj, mxml_node_t* pNode, mxml_node_t *pTop)
{
    PTag pThis =  (PTag)pObj;
    log_trace("---------------<Tag>--------------->>");

    const char *szTmp = mxmlElementGetAttr(pNode, "id");
    pThis->iID = atoi(szTmp);
    log_trace("id: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "name");
    strcpy(pThis->szName, szTmp);
    log_trace("name: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "unit");
    strcpy(pThis->szUnit, szTmp);
    log_trace("unit: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "addr_type");
    strcpy(pThis->szAddrType, szTmp);
    log_trace("addr_type: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "addr_offset");
    char *pszDot = strchr(szTmp, '.');
    if(pszDot != NULL) { // Q1.2 处理偏移地址形如[1.2]
        char **tokens = NULL;
        size_t num = 0;
        if (!cstr_split(szTmp, ".", &tokens, &num)) {
            log_warn("couldn't handle a memory request. token list incomplete");
        }
        if(num == 2) {
            INT32U iByte = atoi(tokens[0]);
            INT32U iBit = atoi(tokens[1]);
            pThis->iAddrOffset = iByte * 8 + iBit;
            //------------------------------------->>>>----
            // 因有些PLC[如Omron]的位地址范围为[bit0-bit15]以上计算有问题，在不影响以前协议的基础上
            // 新增字节地址偏移和位地址偏移两个属性
            pThis->iAddrByteOffset = iByte;
            pThis->iAddrBitOffset = iBit;
            //-------------------------------------<<<<----
        }
        for (size_t i = 0; i < num; ++i) {
            free(tokens[i]);
        }
        free(tokens);
        log_trace("addr_offset: %d", pThis->iAddrOffset);
    } else {
        pThis->iAddrOffset = atoi(szTmp);
        pThis->iAddrByteOffset = pThis->iAddrOffset;
        pThis->iAddrBitOffset = 0;
        log_trace("addr_offset: %s", szTmp);
    }

    szTmp = mxmlElementGetAttr(pNode, "addr_type2");
    strcpy(pThis->szAddrType2, szTmp);
    log_trace("addr_type2: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "addr_offset2");
    pszDot = strchr(szTmp, '.');
    if(pszDot != NULL) { // Q1.2 处理偏移地址形如[1.2]
        char **tokens = NULL;
        size_t num = 0;
        if (!cstr_split(szTmp, ".", &tokens, &num)) {
            log_warn("couldn't handle a memory request. token list incomplete");
        }
        if(num == 2) {
            INT32U iByte = atoi(tokens[0]);
            INT32U iBit = atoi(tokens[1]);
            pThis->iAddrOffset2 = iByte * 8 + iBit;
            pThis->iAddrByteOffset2 = iByte;
            pThis->iAddrBitOffset2 = iBit;
        }
        for (size_t i = 0; i < num; ++i) {
            free(tokens[i]);
        }
        free(tokens);
        log_trace("addr_offset2: %d", pThis->iAddrOffset2);
    } else {
        pThis->iAddrOffset2 = atoi(szTmp);
        pThis->iAddrByteOffset2 = pThis->iAddrOffset2;
        pThis->iAddrBitOffset2 = 0;
        log_trace("addr_offset2: %s", szTmp);
    }

    szTmp = mxmlElementGetAttr(pNode, "data_type");
    pThis->iDataType = getDataType((PINT8S)szTmp);
    log_trace("data_type: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "writeable");
    pThis->iWriteable = atoi(szTmp);
    log_trace("writeable: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "remark");
    strcpy(pThis->szRemark, szTmp);
    log_trace("remark: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "group");
    strcpy(pThis->szOwnGroup, szTmp);
    log_trace("group: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "is_scale");
    pThis->bLineScale = (cstr_equals(szTmp, "true") ? TRUE : FALSE);
    log_trace("is_scale: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "input_min");
    strcpy(pThis->szInputMin, szTmp);
    log_trace("input_min: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "input_max");
    strcpy(pThis->szInputMax, szTmp);
    log_trace("input_max: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "output_min");
    strcpy(pThis->szOutputMin, szTmp);
    log_trace("output_min: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "output_max");
    strcpy(pThis->szOutputMax, szTmp);
    log_trace("output_max: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "output_data_type");
    pThis->iOutputDataType = getDataType((PINT8S)szTmp);
    log_trace("output_data_type: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "save_type");
    pThis->iSaveType = atoi(szTmp);
    log_trace("save_type: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "save_value");
    strcpy(pThis->szSaveValue, szTmp);
    log_trace("save_value: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "is_enable_safe");
    pThis->bEnSafeLevel = (cstr_equals(szTmp, "true") ? TRUE : FALSE);
    log_trace("is_enable_safe: %s", szTmp);

    szTmp = mxmlElementGetAttr(pNode, "safe_level");
    pThis->iSafeLevel = atoi(szTmp);
    log_trace("safe_level: %s", szTmp);

    //----------------------------<解析warning_config节点>------------------------------------
    mxml_node_t* nodeConfigWarning = mxmlFindElementInChild(pNode, pNode, "warning_config", NULL, NULL, MXML_DESCEND);
    while(nodeConfigWarning) {
        //log_trace("warning_config: %s", mxmlGetElement(nodeConfigWarning));
        const char *szTmp = mxmlElementGetAttr(nodeConfigWarning, "name");
        //log_trace("name: %s", szTmp);

        PConfigWarning pConfigWarning = (PConfigWarning)ConfigWarning_CreateObject();
        pConfigWarning->pPubObj->pfGetXmlData(pConfigWarning, nodeConfigWarning, pTop);
        list_add_tail(&pConfigWarning->list, &pThis->listConfigWarnings);

        nodeConfigWarning = mxmlFindElement(nodeConfigWarning, pNode, "warning_config", NULL, NULL, MXML_NO_DESCEND);
    }

    log_trace("---------------<Tag>---------------<<");

}

// 终端打印数据
static void Tag_PrintShow(PVOID pObj)
{
    PTag pThis =  (PTag)pObj;
    char *buf = calloc(1, 1024);

    sprintf(buf, "id: %d, name: %s, unit: %s, addr_type: %s, addr_offset: %d, addr_type2: %s, addr_offset2: %d, data_type: %d, writeable: %d, remark: %s, group: %s, is_scale: %d, input_min: %s, input_max: %s, output_min: %s, output_max: %s, output_data_type: %d, save_type: %d, save_value: %s, is_enable_safe: %d, safe_level: %d",
            pThis->iID, \
            pThis->szName, \
            pThis->szUnit, \
            pThis->szAddrType, \
            pThis->iAddrOffset, \
            pThis->szAddrType2, \
            pThis->iAddrOffset2, \
            pThis->iDataType, \
            pThis->iWriteable, \
            pThis->szRemark, \
            pThis->szOwnGroup, \
            pThis->bLineScale, \
            pThis->szInputMin, \
            pThis->szInputMax, \
            pThis->szOutputMin, \
            pThis->szOutputMax, \
            pThis->iOutputDataType, \
            pThis->iSaveType, \
            pThis->szSaveValue, \
            pThis->bEnSafeLevel, \
            pThis->iSafeLevel
           );

    log_trace("---------------<Tag>--------------->>");
    log_trace("[%s]", buf);

    memset(buf, 0, 1024);
    sprintf(buf, "id: %d, offset: %d", pThis->iPackageReadID, pThis->iAddrOffsetInPackage);
    log_trace("package read info[%s]", buf);

    free(buf);

    struct list_head *pos;
    list_for_each(pos, &pThis->listConfigWarnings) {
        PConfigWarning pConfigWarning = list_entry(pos, TConfigWarning, list);
        pConfigWarning->pPubObj->pfPrintShow(pConfigWarning);
    }

    log_trace("---------------<Tag>---------------<<");
}

// 释放资源
static void Tag_Free(PVOID pObj)
{
    PTag pThis = (PTag)pObj;
    log_trace("free Tag [name: %s, id: %d]", pThis->szName, pThis->iID);

    struct list_head *pos;
    for (pos = (&pThis->listConfigWarnings)->next; pos != (&pThis->listConfigWarnings); ) {
        PConfigWarning pConfigWarning = list_entry(pos, TConfigWarning, list);
        pos = pos->next;
        pConfigWarning->pPubObj->pfFree(pConfigWarning);
    }

    free(pThis);
}

////////////////////////////////////////////////////////////////////////////

static IPubObj ITag = {
    Tag_GetXmlData,
    Tag_PrintShow,
    Tag_Free,
};

// 获取变量数据长度
INT32 Tag_GetDataTypeLength(PTag pTag)
{
    INT32 ret = 0;
    switch(pTag->iDataType) {
        case TYPE_BOOL:
        case TYPE_INT8:
        case TYPE_UINT8:
        case TYPE_BCD8:
            ret = 1;
            break;
        case TYPE_INT16:
        case TYPE_UINT16:
        case TYPE_BCD16:
            ret = 2;
            break;
        case TYPE_INT32:
        case TYPE_UINT32:
        case TYPE_FLOAT32:
        case TYPE_BCD32:
            ret = 4;
            break;
        case TYPE_DOUBLE:
            ret = 8;
            break;
        case TYPE_BYTES: {
            PPackageReadTag pPackageReadTag = (PPackageReadTag)pTag;
            ret = pPackageReadTag->iPackageSize;
        }
        break;
        default:
            ret = 0;
            break;
    }

    return ret;
}



// 获取变量数据缓存长度
INT32 Tag_GetDataTypeBufferLength(PTag pTag)
{
    INT32 ret = 0;
    switch(pTag->iDataType) {
        case TYPE_BOOL:
        case TYPE_INT8:
        case TYPE_UINT8:
        case TYPE_BCD8:
            ret = 1;
            break;
        case TYPE_INT16:
        case TYPE_UINT16:
        case TYPE_BCD16:
            ret = 2;
            break;
        case TYPE_INT32:
        case TYPE_UINT32:
        case TYPE_FLOAT32:
        case TYPE_BCD32:
            ret = 4;
            break;
        case TYPE_DOUBLE:
            ret = 8;
            break;
        case TYPE_BYTES: {
            PPackageReadTag pPackageReadTag = (PPackageReadTag)pTag;
            ret = pPackageReadTag->iPackageBufSize;
        }
        break;
        default:
            ret = 0;
            break;
    }

    return ret;
}

//
// 获取变量线性变换输出数据类型长度
//
INT32 Tag_GetScaleOutputDataTypeLength(PTag pTag)
{
    if(pTag->bLineScale == FALSE) {
        return 0;
    }

    INT32 ret = 0;
    switch(pTag->iOutputDataType) {
        case TYPE_BOOL:
        case TYPE_INT8:
        case TYPE_UINT8:
        case TYPE_BCD8:
            ret = 1;
            break;
        case TYPE_INT16:
        case TYPE_UINT16:
        case TYPE_BCD16:
            ret = 2;
            break;
        case TYPE_INT32:
        case TYPE_UINT32:
        case TYPE_FLOAT32:
        case TYPE_BCD32:
            ret = 4;
            break;
        case TYPE_DOUBLE:
            ret = 8;
            break;
        default:
            ret = 0;
            break;
    }

    return ret;
}

//
// 获取指定数据长度
//
INT32 getDataTypeLength(TDataType iDataType)
{
    INT32 ret = 0;
    switch(iDataType) {
        case TYPE_BOOL:
        case TYPE_INT8:
        case TYPE_UINT8:
        case TYPE_BCD8:
            ret = 1;
            break;
        case TYPE_INT16:
        case TYPE_UINT16:
        case TYPE_BCD16:
            ret = 2;
            break;
        case TYPE_INT32:
        case TYPE_UINT32:
        case TYPE_FLOAT32:
        case TYPE_BCD32:
            ret = 4;
            break;
        case TYPE_DOUBLE:
            ret = 8;
            break;
        default:
            ret = 0;
            break;
    }

    return ret;
}


//------------------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////

static IPubObj IPackageReadTag;

PVOID PackageReadTag_CreateObject(void)
{
    PPackageReadTag pThis = (PPackageReadTag)malloc(sizeof(TPackageReadTag));
    memset((PVOID)pThis, 0, sizeof(TPackageReadTag));

    pThis->tag.pPubObj = &IPackageReadTag;
    pThis->tag.pfValueBufferUpdated = Tag_ValueBufferUpdated;
    pThis->tag.pfCheckWarnings = Tag_CheckWarnings;

    pThis->tag.iID = 0;
    memset(pThis->tag.szName, 0, sizeof(pThis->tag.szName) / sizeof(INT8S));
    memset(pThis->tag.szUnit, 0, sizeof(pThis->tag.szUnit) / sizeof(INT8S));
    memset(pThis->tag.szAddrType, 0, sizeof(pThis->tag.szAddrType) / sizeof(INT8S));
    pThis->tag.iAddrOffset = 0;
    memset(pThis->tag.szAddrType2, 0, sizeof(pThis->tag.szAddrType2) / sizeof(INT8S));
    pThis->tag.iAddrOffset2 = 0;
    pThis->tag.iDataType = TYPE_BYTES;
    pThis->tag.iWriteable = 0;
    memset(pThis->tag.szRemark, 0, sizeof(pThis->tag.szRemark) / sizeof(INT8S));
    memset(pThis->tag.szOwnGroup, 0, sizeof(pThis->tag.szOwnGroup) / sizeof(INT8S));

    pThis->tag.bLineScale = FALSE;
    memset(pThis->tag.szInputMin, 0, sizeof(pThis->tag.szInputMin) / sizeof(INT8S));
    memset(pThis->tag.szInputMax, 0, sizeof(pThis->tag.szInputMax) / sizeof(INT8S));
    memset(pThis->tag.szOutputMin, 0, sizeof(pThis->tag.szOutputMin) / sizeof(INT8S));
    memset(pThis->tag.szOutputMax, 0, sizeof(pThis->tag.szOutputMax) / sizeof(INT8S));
    pThis->tag.iOutputDataType = TYPE_NONE;

    INIT_LIST_HEAD(&pThis->tag.listConfigWarnings);

    pThis->tag.iSaveType = 0;
    memset(pThis->tag.szSaveValue, 0, sizeof(pThis->tag.szSaveValue) / sizeof(INT8S));

    pThis->tag.bEnSafeLevel = FALSE;
    pThis->tag.iSafeLevel = 0;

    memset(pThis->tag.readBuf, 0, TAG_BUFFER_SIZE);
    memset(pThis->tag.readBufOld, 0, TAG_BUFFER_SIZE);
    memset(pThis->tag.writeBuf, 0, TAG_BUFFER_SIZE);

    pThis->tag.bWriteToDevice = FALSE;
    pThis->tag.iPackageReadID = -1;
    pThis->tag.iAddrOffsetInPackage = 0;

    INIT_LIST_HEAD(&pThis->tag.list);
    INIT_LIST_HEAD(&pThis->tag.listInPackage);

    pThis->bPackageReadOk = FALSE;
    pThis->bPackageInUsed = FALSE;
    memset(pThis->pPackageReadBuf, 0, sizeof(pThis->pPackageReadBuf) / sizeof(INT8S));
    pThis->iPackageBufSize = 0;
    pThis->iPackageSize = 0;
    pThis->bLittleEndian = TRUE;
    pThis->tag.bValueChanged = FALSE;
    pThis->tag.bWarnings = FALSE;
    pThis->tag.bOldWarnings = FALSE;

    return (PVOID)pThis;
}

///////////////////////////////////////////////////////////

//从XML节点获取配置的数据
static void PackageReadTag_GetXmlData(PVOID pObj, mxml_node_t* pNode, mxml_node_t *pTop)
{
    Tag_GetXmlData(pObj, pNode, pTop);
}

// 终端打印数据
static void PackageReadTag_PrintShow(PVOID pObj)
{
    PPackageReadTag pThis =  (PPackageReadTag)pObj;
    char *buf = calloc(1, 1024);

    sprintf(buf, "id: %d, name: %s, unit: %s, addr_type: %s, addr_offset: %d, addr_type2: %s, addr_offset2: %d, data_type: %d, writeable: %d, remark: %s, group: %s, is_scale: %d, input_min: %s, input_max: %s, output_min: %s, output_max: %s, output_data_type: %d, save_type: %d, save_value: %s, is_enable_safe: %d, safe_level: %d",
            pThis->tag.iID, \
            pThis->tag.szName, \
            pThis->tag.szUnit, \
            pThis->tag.szAddrType, \
            pThis->tag.iAddrOffset, \
            pThis->tag.szAddrType2, \
            pThis->tag.iAddrOffset2, \
            pThis->tag.iDataType, \
            pThis->tag.iWriteable, \
            pThis->tag.szRemark, \
            pThis->tag.szOwnGroup, \
            pThis->tag.bLineScale, \
            pThis->tag.szInputMin, \
            pThis->tag.szInputMax, \
            pThis->tag.szOutputMin, \
            pThis->tag.szOutputMax, \
            pThis->tag.iOutputDataType, \
            pThis->tag.iSaveType, \
            pThis->tag.szSaveValue, \
            pThis->tag.bEnSafeLevel, \
            pThis->tag.iSafeLevel
           );

    log_trace("---------------<Tag>--------------->>");
    log_trace("[%s]", buf);

    memset(buf, 0, 1024);
    sprintf(buf, "id: %d, size: %d, buffer size: %d", pThis->tag.iPackageReadID, pThis->iPackageSize, pThis->iPackageBufSize);
    log_trace("package read info[%s]", buf);

    free(buf);

    struct list_head *pos;
    list_for_each(pos, &pThis->tag.listConfigWarnings) {
        PConfigWarning pConfigWarning = list_entry(pos, TConfigWarning, list);
        pConfigWarning->pPubObj->pfPrintShow(pConfigWarning);
    }

    log_trace("---------------<Tag>---------------<<");
}

// 释放资源
static void PackageReadTag_Free(PVOID pObj)
{
    PPackageReadTag pThis = (PPackageReadTag)pObj;
    log_trace("free Tag [name: %s, id: %d]", pThis->tag.szName, pThis->tag.iID);

    struct list_head *pos;
    for (pos = (&pThis->tag.listConfigWarnings)->next; pos != (&pThis->tag.listConfigWarnings); ) {
        PConfigWarning pConfigWarning = list_entry(pos, TConfigWarning, list);
        pos = pos->next;
        pConfigWarning->pPubObj->pfFree(pConfigWarning);
    }

    free(pThis);
}

////////////////////////////////////////////////////////////////////////////

static IPubObj IPackageReadTag = {
    PackageReadTag_GetXmlData,
    PackageReadTag_PrintShow,
    PackageReadTag_Free,
};

//------------------------------------------------------------------------------------------------

// 查找组包读变量列表是否存在地址类型
// @param listFind 组包读变量列表
// @param szAddrType 地址类型
// @return FALSE-不存在, TRUE-存在
BOOLEAN isAddrTypeExistInPackageReadObj(struct list_head *listFind, PINT8S szAddrType)
{
    struct list_head *pos;
    list_for_each(pos, listFind) {
        PPackageReadObj pPackageReadObj = list_entry(pos, TPackageReadObj, list);
        if(cstr_equals(pPackageReadObj->szAddrType, szAddrType)) {
            return TRUE;
        }
    }
    return FALSE;
}


// 查找指定地址类型的组包读取描述对象
// @param listFind 组包读变量列表
// @param szAddrType 地址类型
// @return 指定地址类型的组包读取描述对象
PPackageReadObj getPackageReadObjByAddrType(struct list_head *listFind, PINT8S szAddrType)
{
    struct list_head *pos;
    list_for_each(pos, listFind) {
        PPackageReadObj pPackageReadObj = list_entry(pos, TPackageReadObj, list);
        if(cstr_equals(pPackageReadObj->szAddrType, szAddrType)) {
            return pPackageReadObj;
        }
    }
    return NULL;
}


// 设备描述对象组包读变量列表增加组包读变量列表对象
// @param pObj 设备描述对象
void packageReadAddAddrType(PVOID pObj)
{
    PDevice pThis =  (PDevice)pObj;
    if(pThis->iPortType == PORT_SERIAL) {
        struct list_head *pos;
        list_for_each(pos, &pThis->listTags) {
            PTag pTag = list_entry(pos, TTag, list);
            if(!isAddrTypeExistInPackageReadObj(&pThis->listPackageReadTags, pTag->szAddrType)) {
                PPackageReadObj pPackageReadObj = (PPackageReadObj)PackageRead_CreateObject();
                strcpy(pPackageReadObj->szAddrType, pTag->szAddrType);
                list_add_tail(&pTag->listInPackage, &pPackageReadObj->listTags);
                list_add_tail(&pPackageReadObj->list, &pThis->listPackageReadTags);
            } else {
                PPackageReadObj pPackageReadObj = getPackageReadObjByAddrType(&pThis->listPackageReadTags, pTag->szAddrType);
                list_add_tail(&pTag->listInPackage, &pPackageReadObj->listTags);
            }
        }

        list_for_each(pos, &pThis->listTagGroup) {
            PTagGroup pTagGroup = list_entry(pos, TTagGroup, list);
            struct list_head *pos1;
            list_for_each(pos1, &pTagGroup->listTags) {
                PTag pTag = list_entry(pos1, TTag, list);
                if(!isAddrTypeExistInPackageReadObj(&pThis->listPackageReadTags, pTag->szAddrType)) {
                    PPackageReadObj pPackageReadObj = (PPackageReadObj)PackageRead_CreateObject();
                    strcpy(pPackageReadObj->szAddrType, pTag->szAddrType);
                    list_add_tail(&pTag->listInPackage, &pPackageReadObj->listTags);
                    list_add_tail(&pPackageReadObj->list, &pThis->listPackageReadTags);
                } else {
                    PPackageReadObj pPackageReadObj = getPackageReadObjByAddrType(&pThis->listPackageReadTags, pTag->szAddrType);
                    list_add_tail(&pTag->listInPackage, &pPackageReadObj->listTags);
                }
            }
        }
    } else if(pThis->iPortType == PORT_NET) {
        PNetDevice pNetThis =  (PNetDevice)pObj;
        struct list_head *pos;
        list_for_each(pos, &pNetThis->device.listTags) {
            PTag pTag = list_entry(pos, TTag, list);
            if(!isAddrTypeExistInPackageReadObj(&pNetThis->device.listPackageReadTags, pTag->szAddrType)) {
                PPackageReadObj pPackageReadObj = (PPackageReadObj)PackageRead_CreateObject();
                strcpy(pPackageReadObj->szAddrType, pTag->szAddrType);
                list_add_tail(&pTag->listInPackage, &pPackageReadObj->listTags);
                list_add_tail(&pPackageReadObj->list, &pNetThis->device.listPackageReadTags);
            } else {
                PPackageReadObj pPackageReadObj = getPackageReadObjByAddrType(&pNetThis->device.listPackageReadTags, pTag->szAddrType);
                list_add_tail(&pTag->listInPackage, &pPackageReadObj->listTags);
            }
        }

        list_for_each(pos, &pNetThis->device.listTagGroup) {
            PTagGroup pTagGroup = list_entry(pos, TTagGroup, list);
            struct list_head *pos1;
            list_for_each(pos1, &pTagGroup->listTags) {
                PTag pTag = list_entry(pos1, TTag, list);
                if(!isAddrTypeExistInPackageReadObj(&pNetThis->device.listPackageReadTags, pTag->szAddrType)) {
                    PPackageReadObj pPackageReadObj = (PPackageReadObj)PackageRead_CreateObject();
                    strcpy(pPackageReadObj->szAddrType, pTag->szAddrType);
                    list_add_tail(&pTag->listInPackage, &pPackageReadObj->listTags);
                    list_add_tail(&pPackageReadObj->list, &pNetThis->device.listPackageReadTags);
                } else {
                    PPackageReadObj pPackageReadObj = getPackageReadObjByAddrType(&pNetThis->device.listPackageReadTags, pTag->szAddrType);
                    list_add_tail(&pTag->listInPackage, &pPackageReadObj->listTags);
                }
            }
        }
    }
}



//
//线性换算
// X = (Y - b) / k
// k = (OUTmax-OUTmin) / (INmax-INmin)
// b = (INmax * OUTmin-INmin * OUTmax) / (INmax-INmin)
//
static void Tag_WriteScaleCalculate(PTag pTag, FLOAT64 dTagValue)
{
    if(pTag->bLineScale == TRUE) { // 启用线性换算
        FLOAT64 dInputMin = strtod(pTag->szInputMin, NULL);
        FLOAT64 dInputMax = strtod(pTag->szInputMax, NULL);
        FLOAT64 dOutputMin = strtod(pTag->szOutputMin, NULL);
        FLOAT64 dOutputMax = strtod(pTag->szOutputMax, NULL);
        if(dInputMax == dInputMin) {
            log_error("dInputMax == dInputMin division zero");
            return;
        }

        //log_trace("dInputMin: %lf, dInputMax: %lf, dOutputMin: %lf, dOutputMax: %lf, iOutputDataType: %d", dInputMin, dInputMax, dOutputMin, dOutputMax, pTag->iOutputDataType);
        FLOAT64 k = (dOutputMax - dOutputMin) / (dInputMax - dInputMin);
        FLOAT64 b = (dInputMax * dOutputMin - dInputMin * dOutputMax) / (dInputMax - dInputMin);
        FLOAT64 dNewTagValue = (dTagValue - b) / k;
        //log_trace("k: %lf, b: %lf, tag value: %lf", k, b, dTagValue);
        memset(pTag->writeBuf, 0, TAG_BUFFER_SIZE);
        if(pTag->iDataType == TYPE_BOOL) {
            pTag->writeBuf[0] = (dNewTagValue != 0) ? 1 : 0;
        } else if(pTag->iDataType == TYPE_INT8) {
            INT8 iValue = (INT8)dNewTagValue;
            memcpy(pTag->writeBuf, (PVOID)&iValue, 1);
        } else if(pTag->iDataType == TYPE_UINT8) {
            INT8U iValue = (INT8U)dNewTagValue;
            memcpy(pTag->writeBuf, (PVOID)&iValue, 1);
        } else if(pTag->iDataType == TYPE_INT16) {
            INT16 iValue = (INT16)dNewTagValue;
            memcpy(pTag->writeBuf, (PVOID)&iValue, 2);
        } else if(pTag->iDataType == TYPE_UINT16) {
            INT16U iValue = (INT16U)dNewTagValue;
            memcpy(pTag->writeBuf, (PVOID)&iValue, 2);
        } else if(pTag->iDataType == TYPE_INT32) {
            INT32 iValue = (INT32)dNewTagValue;
            memcpy(pTag->writeBuf, (PVOID)&iValue, 4);
        } else if(pTag->iDataType == TYPE_UINT32) {
            INT32U iValue = (INT32U)dNewTagValue;
            memcpy(pTag->writeBuf, (PVOID)&iValue, 4);
        } else if(pTag->iDataType == TYPE_FLOAT32) {
            FLOAT32 iValue = (FLOAT32)dNewTagValue;
            memcpy(pTag->writeBuf, (PVOID)&iValue, 4);
        } else if(pTag->iDataType == TYPE_DOUBLE) {
            memcpy(pTag->writeBuf, (PVOID)&dNewTagValue, 8);
        }
    }
}



// 字符串转指定类型值
void Tag_StringToValue(TDataType iDataType, PINT8U pBuf, PINT8S pData)
{
    if(iDataType == TYPE_BOOL) {
        INT8U iValue = atoi(pData);
        memcpy(pBuf, (PVOID)&iValue, 1);
    } else if(iDataType == TYPE_INT8) {
        INT8 iValue = atoi(pData);
        memcpy(pBuf, (PVOID)&iValue, 1);
    } else if(iDataType == TYPE_UINT8) {
        INT8U iValue = atoi(pData);
        memcpy(pBuf, (PVOID)&iValue, 1);
    } else if(iDataType == TYPE_BCD8) {
        INT8U iValue = (INT8U)strtoul(pData, NULL, 16);
        memcpy(pBuf, (PVOID)&iValue, 1);
    } else if(iDataType == TYPE_INT16) {
        INT16 iValue = atoi(pData);
        memcpy(pBuf, (PVOID)&iValue, 2);
    } else if(iDataType == TYPE_UINT16) {
        INT16U iValue = atoi(pData);
        memcpy(pBuf, (PVOID)&iValue, 2);
    } else if(iDataType == TYPE_BCD16) {
        INT16U iValue = (INT16U)strtoul(pData, NULL, 16);
        memcpy(pBuf, (PVOID)&iValue, 2);
    } else if(iDataType == TYPE_INT32) {
        INT32 iValue = (INT32)strtol(pData, NULL, 10);
        memcpy(pBuf, (PVOID)&iValue, 4);
    } else if(iDataType == TYPE_UINT32) {
        INT32U iValue = (INT32U)strtoul(pData, NULL, 10);
        memcpy(pBuf, (PVOID)&iValue, 4);
    } else if(iDataType == TYPE_BCD32) {
        INT32U iValue = (INT32U)strtoul(pData, NULL, 16);
        memcpy(pBuf, (PVOID)&iValue, 4);
    } else if(iDataType == TYPE_FLOAT32) {
        FLOAT32 fValue = atof(pData);
        memcpy(pBuf, (PVOID)&fValue, 4);
    } else if(iDataType == TYPE_DOUBLE) {
        FLOAT64 dValue = strtod(pData, NULL);
        memcpy(pBuf, (PVOID)&dValue, 8);
    } else {
    }
}



// 设置变量写入值(数值字符数据)
void Tag_SetWriteBuffer(PTag pTag, TDataType iDataType, PINT8S pData)
{
    if(pTag->iDataType != iDataType) {
        return;
    }

    if(pTag->bLineScale == TRUE) {
        FLOAT64 dTagValue = strtod(pData, NULL);
        Tag_WriteScaleCalculate(pTag, dTagValue);
    } else {
        Tag_StringToValue(pTag->iDataType, pTag->writeBuf, pData);
    }

    pTag->bWriteToDevice = TRUE;
}



// 设置变量写入值(指定数据类型的字节数据)
void Tag_SetDataToWriteBuffer(PTag pTag, TDataType iDataType, PINT8U pData)
{
    FLOAT64 dTagValue = 0;
    if((pTag->bLineScale == FALSE && pTag->iDataType != iDataType) ||
            (pTag->bLineScale == TRUE && pTag->iOutputDataType != iDataType)) {
        return;
    }
    INT32 iLength = getDataTypeLength(iDataType);
    memcpy((PVOID)pTag->writeBuf, (PVOID)pData, iLength);

    if(pTag->bLineScale == FALSE) {
        pTag->bWriteToDevice = TRUE;
        return;
    }

    if(pTag->iOutputDataType == TYPE_BOOL) {
        INT8U iValue = 0;
        memcpy((PVOID)&iValue, pData, 1);
        dTagValue = (FLOAT64)iValue;
    } else if(pTag->iOutputDataType == TYPE_INT8) {
        INT8 iValue = 0;
        memcpy((PVOID)&iValue, pData, 1);
        dTagValue = (FLOAT64)iValue;
    } else if(pTag->iOutputDataType == TYPE_UINT8) {
        INT8U iValue = 0;
        memcpy((PVOID)&iValue, pData, 1);
        dTagValue = (FLOAT64)iValue;
    } else if(pTag->iOutputDataType == TYPE_BCD8) {
        INT8U iValue = 0;
        memcpy((PVOID)&iValue, pData, 1);
        dTagValue = (FLOAT64)iValue;
    } else if(pTag->iOutputDataType == TYPE_INT16) {
        INT16 iValue = 0;
        memcpy((PVOID)&iValue, pData, 2);
        dTagValue = (FLOAT64)iValue;
        memcpy(pTag->writeBuf, (PVOID)&iValue, 2);
    } else if(pTag->iOutputDataType == TYPE_UINT16) {
        INT16U iValue = 0;
        memcpy((PVOID)&iValue, pData, 2);
        dTagValue = (FLOAT64)iValue;
    } else if(pTag->iOutputDataType == TYPE_BCD16) {
        INT16U iValue = 0;
        memcpy((PVOID)&iValue, pData, 2);
        dTagValue = (FLOAT64)iValue;
    } else if(pTag->iOutputDataType == TYPE_INT32) {
        INT32 iValue = 0;
        memcpy((PVOID)&iValue, pData, 4);
        dTagValue = (FLOAT64)iValue;
    } else if(pTag->iOutputDataType == TYPE_UINT32) {
        INT32U iValue = 0;
        memcpy((PVOID)&iValue, pData, 4);
        dTagValue = (FLOAT64)iValue;
    } else if(pTag->iOutputDataType == TYPE_BCD32) {
        INT32U iValue = 0;
        memcpy((PVOID)&iValue, pData, 4);
        dTagValue = (FLOAT64)iValue;
    } else if(pTag->iOutputDataType == TYPE_FLOAT32) {
        FLOAT32 fValue = 0;
        memcpy((PVOID)&fValue, pData, 4);
        dTagValue = (FLOAT64)fValue;
    } else if(pTag->iOutputDataType == TYPE_DOUBLE) {
        FLOAT64 dValue = 0;
        memcpy((PVOID)&dValue, pData, 8);
        dTagValue = (FLOAT64)dValue;
    } else {
    }

    Tag_WriteScaleCalculate(pTag, dTagValue);
    pTag->bWriteToDevice = TRUE;
}

// 指定类型值转字符串
PINT8S Tag_ValueToString(TDataType iDataType, PINT8U pBuf)
{
    INT8S buf[256] = {0};

    if(iDataType == TYPE_BOOL) {
        INT8U iValue = 0;
        memcpy((PVOID)&iValue, pBuf, 1);
        sprintf((PINT8S)buf, "%u", iValue);
        return strdup((const char *)buf);
    } else if(iDataType == TYPE_INT8) {
        INT8 iValue = 0;
        memcpy((PVOID)&iValue, pBuf, 1);
        sprintf((PINT8S)buf, "%d", iValue);
        return strdup((const char *)buf);
    } else if(iDataType == TYPE_UINT8) {
        INT8U iValue = 0;
        memcpy((PVOID)&iValue, pBuf, 1);
        sprintf((PINT8S)buf, "%u", iValue);
        return strdup((const char *)buf);
    } else if(iDataType == TYPE_BCD8) {
        INT8U iValue = 0;
        memcpy((PVOID)&iValue, pBuf, 1);
        sprintf((PINT8S)buf, "%-2X", iValue);
        return strdup((const char *)buf);
    } else if(iDataType == TYPE_INT16) {
        INT16 iValue = 0;
        memcpy((PVOID)&iValue, pBuf, 2);
        sprintf((PINT8S)buf, "%d", iValue);
        return strdup((const char *)buf);
    } else if(iDataType == TYPE_UINT16) {
        INT16U iValue = 0;
        memcpy((PVOID)&iValue, pBuf, 2);
        sprintf((PINT8S)buf, "%u", iValue);
        return strdup((const char *)buf);
    } else if(iDataType == TYPE_BCD16) {
        INT16U iValue = 0;
        memcpy((PVOID)&iValue, pBuf, 2);
        sprintf((PINT8S)buf, "%-4X", iValue);
        return strdup((const char *)buf);
    } else if(iDataType == TYPE_INT32) {
        INT32 iValue = 0;
        memcpy((PVOID)&iValue, pBuf, 4);
        sprintf((PINT8S)buf, "%d", iValue);
        return strdup((const char *)buf);
    } else if(iDataType == TYPE_UINT32) {
        INT32U iValue = 0;
        memcpy((PVOID)&iValue, pBuf, 4);
        sprintf((PINT8S)buf, "%u", iValue);
        return strdup((const char *)buf);
    } else if(iDataType == TYPE_BCD32) {
        INT32U iValue = 0;
        memcpy((PVOID)&iValue, pBuf, 4);
        sprintf((PINT8S)buf, "%-8X", iValue);
        return strdup((const char *)buf);
    } else if(iDataType == TYPE_FLOAT32) {
        FLOAT32 fValue = 0;
        memcpy((PVOID)&fValue, pBuf, 4);
        sprintf((PINT8S)buf, "%f", fValue);
        return strdup((const char *)buf);
    } else if(iDataType == TYPE_DOUBLE) {
        FLOAT64 dValue = 0;
        memcpy((PVOID)&dValue, pBuf, 8);
        gcvt(dValue, 32, buf);
        return strdup((const char *)buf);
    } else {
    }

    return NULL;
}


// 获取变量值字符串
PINT8S Tag_GetValueString(PTag pTag)
{
    TDataType iDataType = TYPE_NONE;

    if(pTag->bLineScale == TRUE) {
        iDataType = pTag->iOutputDataType;
    } else {
        iDataType = pTag->iDataType;
    }

    return Tag_ValueToString(iDataType, pTag->readBuf);
}

//
// 获取上一采集周期变量值字符串
//
PINT8S Tag_GetOldValueString(PTag pTag)
{
    TDataType iDataType = TYPE_NONE;

    if(pTag->bLineScale == TRUE) {
        iDataType = pTag->iOutputDataType;
    } else {
        iDataType = pTag->iDataType;
    }

    return Tag_ValueToString(iDataType, pTag->readBufOld);
}

