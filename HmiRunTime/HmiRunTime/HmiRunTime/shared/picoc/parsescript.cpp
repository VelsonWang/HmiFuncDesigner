#include "parsescript.h"

extern "C"
{
#include "src/picoc.h"
}

int ParseScript::run_c_main(int argc, char **argv)
{
    return picoc_main(argc, argv);
}

int ParseScript::run_c(const QString &script)
{
    return picoc((char *)script.toStdString().c_str());
}


