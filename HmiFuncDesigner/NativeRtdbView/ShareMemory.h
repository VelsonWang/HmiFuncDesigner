#ifndef __SHAREMEMORY_H__
#define __SHAREMEMORY_H__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include<sys/types.h>
#include<sys/shm.h>
#endif

struct SharememoryHandle;

class Sharememory
{
public:
    Sharememory();
    Sharememory(std::string name);
    ~Sharememory();

    std::string getName();

    void setName(std::string name);

    bool create(unsigned int size);

    void* open();

    void destroy();

private:
    void init();
    void release();
    void resetHandle();

private:
    std::string name;
    SharememoryHandle *handle;
};

#endif
