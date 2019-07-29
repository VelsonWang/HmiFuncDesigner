#ifndef THREADHANDLE_H
#define THREADHANDLE_H

#include <QThread>
#include <QQueue>
#include <QList>
#include <QHash>
//线程管理类，类似于一个线程池，单例类
//两种初始化方式，一种是每个线程处理的连接数，一个是一共多少个线程
class ThreadHandle
{
public:
    ~ThreadHandle();


    enum ThreadType
    {
        THREADSIZE, //固定线程数
        HANDLESIZE //固定每个线程处理多少连接
    };

    static ThreadHandle & getClass(); //返回对象引用，是单例类

    QThread * getThread(); //取出应该移入的线程
    void initThreadType(ThreadType type = HANDLESIZE,unsigned int max = 1000);//初始化线程管理的方式
    void removeThread(QThread *);//连接断开，线程计数减一
    void clear();//清空计数

protected:
    void initThreadSize();//新建固定线程和启动
    QThread * findThreadSize();//固定线程数的查找
    QThread *findHandleSize();//固定连接数查找
private:
    ThreadHandle();
    ThreadType type;//线程类型
    unsigned int size;//最大值
    QHash<QThread *, unsigned int> threadSize;//保存每个线程的数目
    bool initfist;//是否是第一次初始化，只允许初始化一次。

};

#endif // THREADHANDLE_H
