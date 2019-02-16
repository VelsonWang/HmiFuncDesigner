#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QtGlobal>

// 文件传输协议头
typedef struct tagMgsHeader
{
    quint32 length;
    quint32 cmd; // CmdType
}TMgsHeader;

// 文件传输命令类型
enum CmdType
{
    CMD_NONE = 0,
    CMD_DOWNLOAD_PROJECT = 1,
    CMD_DOWNLOAD_PROJECT_ACK = 2,
    CMD_UPLOAD_PROJECT = 3,
    CMD_UPLOAD_PROJECT_ACK = 4,
    CMD_DONE = 5,
};
typedef enum CmdType TCmdType;

// 数据包
typedef struct tagDataPackage
{
    quint32 total; // 数据包总数
    quint32 index; // 第几个数据包
    quint8 data[512];
}TDataPackage;

///////////////////////////////////////////////////////////////////////////////
//
// 文件传输协议
// 命令包
// |----tagMgsHeader----|
// |--length-->CmdType--|
//
// length = tagMgsHeader
//
// 数据包
// |----tagMgsHeader----|-----------tagDataPackage-----------|
// |--length-->CmdType--|--total--index--data[0]--data[511]--|
//
// length = tagMgsHeader + tagDataPackage
//
//

#endif // FILETRANSFER_H
