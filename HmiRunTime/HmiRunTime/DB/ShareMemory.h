#ifndef SHAREMEMORY_H
#define SHAREMEMORY_H

#include <QtGlobal>

#ifdef Q_OS_LINUX
#include <sys/mman.h>
#include <stdlib.h>  
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <asm/ioctls.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#include <stdio.h>
#endif

#define SHM_MODE (SHM_R | SHM_W | IPC_CREAT) 

#ifdef Q_OS_WIN
#define MAX_NUM             12000
#endif

#ifdef Q_OS_LINUX
#define MAX_NUM             4000
#endif

#define MAX_SYSITEM         200
#define MAX_STRUCT          5
#define MAX_STRUCT_DOWN     5
#define MAX_STRUCT_UP       5

#ifdef Q_OS_WIN
#define STATION_SIZE        200000      // 一个站点所占用的大小200000 bytes 未使用的空间作为备用
#endif

#ifdef Q_OS_LINUX
#define SHMDBKEYRTRAM_RT    1829503351  //811047
#define STATION_SIZE        70000
#endif



int openRamRealTimeDB(); // 打开实时数据库



#endif
