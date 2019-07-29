
QT += network

win32 {
    LIBS += -lpthread -lwsock32 -lws2_32
} else {

}

INCLUDEPATH += \
    $$PWD/ 
	

SOURCES += \
    $$PWD/soapC.cpp \
    $$PWD/soapClient.cpp \
    $$PWD/stdsoap2.cpp 


HEADERS += \
    $$PWD/ISoap.h \
    $$PWD/soapH.h \
    $$PWD/soapStub.h \
    $$PWD/stdsoap2.h 

