HEADERS += \
        $$PWD/eventdispatcher_libev.h \
        $$PWD/eventdispatcher_libev_p.h
SOURCES += \
        $$PWD/eventdispatcher_libev.cpp \
        $$PWD/eventdispatcher_libev_p.cpp \
        $$PWD/timers_p.cpp \
        $$PWD/socknot_p.cpp

LIBS += -lev

