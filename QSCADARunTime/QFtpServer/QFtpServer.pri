
QT       += network


DEFINES += QFTPSERVERLIB_LIBRARY

INCLUDEPATH += $$PWD
	
SOURCES += \
    $$PWD/dataconnection.cpp \
    $$PWD/ftpcommand.cpp \
    $$PWD/ftpcontrolconnection.cpp \
    $$PWD/ftplistcommand.cpp \
    $$PWD/ftpretrcommand.cpp \
    $$PWD/ftpserver.cpp \
    $$PWD/ftpstorcommand.cpp \
    $$PWD/sslserver.cpp

HEADERS +=\
    $$PWD/qftpserverlib_global.h \
    $$PWD/dataconnection.h \
    $$PWD/ftpcommand.h \
    $$PWD/ftpcontrolconnection.h \
    $$PWD/ftplistcommand.h \
    $$PWD/ftpretrcommand.h \
    $$PWD/ftpserver.h \
    $$PWD/ftpstorcommand.h \
    $$PWD/sslserver.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    $$PWD/certificates.qrc
