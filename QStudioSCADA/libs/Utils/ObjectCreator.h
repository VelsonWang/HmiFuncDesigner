#ifndef OBJECTCREATOR_H
#define OBJECTCREATOR_H

#include <QString>
#include <QMap>

typedef void* (*CreateObjFunc)();
typedef QMap<QString, CreateObjFunc> QMapdata;


class ObjectCreator
{

public:
    static void  regclass(QString classname, CreateObjFunc createobjfunc)
    {
        getmap()[classname] = createobjfunc;
    }

    static void* getobj(const QString& classname)
    {
        CreateObjFunc creatobjfunc = nullptr;
        QMap<QString, CreateObjFunc>::iterator it;

        it = getmap().find(classname);
        if(it != getmap().end())
            creatobjfunc = it.value();

        if(creatobjfunc != nullptr)
            return (*creatobjfunc)();

        return nullptr;
    }

private:
    inline static QMapdata& getmap()
    {
      static QMapdata instance;
      return instance;
    }

private:
    ObjectCreator(){}
    ~ObjectCreator(){}
    ObjectCreator(const ObjectCreator &) = delete;
    ObjectCreator &operator=(const ObjectCreator &) = delete;
};


///////////////////////////////////////////////////////////////////////
// 类注册宏
// 注意只能在cpp文件中使用
// 严禁在头文件中使用 目前只支持无参的类型构造
///////////////////////////////////////////////////////////////////////

#define REGISTER_CLASS(class_name) \
class C##class_name##Helper { \
public: \
    C##class_name##Helper() \
    { \
        ObjectCreator::regclass(#class_name, C##class_name##Helper::creatObjFunc); \
    } \
    static void* creatObjFunc() \
    { \
        return new class_name; \
    } \
}; \
C##class_name##Helper class_name##Helper;




#endif // OBJECTCREATOR_H
