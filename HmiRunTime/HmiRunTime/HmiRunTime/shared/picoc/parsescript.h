#ifndef PARSESCRIPT_H
#define PARSESCRIPT_H

#include "sharedlibglobal.h"
#include <QString>

class SHAREDLIB_EXPORT ParseScript
{
private:
    ParseScript() {}
    ~ParseScript() {}

public:
    static ParseScript *instance()
    {
        static ParseScript instance;
        return &instance;
    }
    int run_c_main(int argc, char **argv);
    int run_c(const QString &script);
};


#endif
