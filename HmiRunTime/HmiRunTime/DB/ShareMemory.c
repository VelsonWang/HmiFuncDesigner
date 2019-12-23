#include "ShareMemory.h"


// 注：本函数尚未使用(预留实时数据后期将修改为共享内存)
// 打开实时数据库
int openRamRealTimeDB()
{
#ifdef Q_OS_WIN
    char chRamName[30] ; 
    HANDLE hFile;
    HANDLE hMapFile = 0;        
    LPVOID lpMapAddress = 0;
    strcpy(chRamName, "RAMRT");

    if (lpMapAddress) UnmapViewOfFile(lpMapAddress);
    if (hMapFile) CloseHandle(hMapFile);

    hFile = (HANDLE) 0xFFFFFFFF ;

    hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, STATION_SIZE, (LPCTSTR)chRamName);
    if (hMapFile == NULL) {
        lpMapAddress = NULL;
        return 0;
    }
    lpMapAddress = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    // 这里做指针转换访问
    // pDBTagObjectBase = (DBTagObject* )lpMapAddress;

    return 1;
#endif

#ifdef  Q_OS_LINUX
    int shmid ;
    void *shmptr ;

    if((shmid = shmget( SHMDBKEYRTRAM_RT , STATION_SIZE , SHM_MODE ) ) < 0 ){
        return 0 ;
    }
    shmptr = shmat( shmid, 0, 0 ) ;
    
    if ( shmptr == (void*) -1 ) {
        return 0 ;
    }

    // 这里做指针转换访问
    // pDBTagObjectBase = (DBTagObject* )shmptr;

    return 1 ;
#endif
}


