
#DEPENDPATH += cstdlib platform src
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/src
#define  UNIX_HOST
#define  FLYINGFOX_HOST
#define  SURVEYOR_HOST
#define  SRV1_UNIX_HOST
#define  UMON_HOST
#define  WIN_HOST



win32 {
    DEFINES += WIN_HOST

    SOURCES += $$PWD/platform/library_win.c \
               $$PWD/platform/platform_win.c
}


unix {
    DEFINES += UNIX_HOST

    SOURCES += $$PWD/platform/library_unix.c \
               $$PWD/platform/platform_unix.c
}

macx {

}

# Input
HEADERS += \
    $$PWD/src/interpreter.h \
    $$PWD/src/picoc.h \
    $$PWD/src/platform.h \
    $$PWD/parsescript.h

SOURCES += \
    $$PWD/cstdlib/ctype.c \
    $$PWD/cstdlib/errno.c \
    $$PWD/cstdlib/math.c \
    $$PWD/cstdlib/stdbool.c \
    $$PWD/cstdlib/stdio.c \
    $$PWD/cstdlib/stdlib.c \
    $$PWD/cstdlib/string.c \
    $$PWD/cstdlib/time.c \
    $$PWD/cstdlib/unistd.c \
    $$PWD/src/clibrary.c \
    $$PWD/src/expression.c \
    $$PWD/src/heap.c \
    $$PWD/src/include.c \
    $$PWD/src/lex.c \
    $$PWD/src/parse.c \
    $$PWD/src/picoc.c \
    $$PWD/src/platform.c \
    $$PWD/src/table.c \
    $$PWD/src/type.c \
    $$PWD/src/variable.c \
    $$PWD/parsescript.cpp


