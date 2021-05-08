
QT += core
QT += gui
QT += xml
QT += widgets
QT += script

INCLUDEPATH +=  $$PWD

SOURCES += \
    $$PWD/functionedit.cpp \
    $$PWD/functioneditordialog.cpp \
    $$PWD/insertfunctiondlg.cpp \
    $$PWD/inserttagdlg.cpp \
    $$PWD/tagcolorlistedit.cpp \
    $$PWD/tagcolorlisteditordialog.cpp \
    $$PWD/tagtextlistedit.cpp \	
    $$PWD/tagtextlisteditordialog.cpp \
	$$PWD/scriptedit.cpp \
	$$PWD/scriptpropertyeditordlg.cpp \
    $$PWD/variantfactory.cpp \
    $$PWD/variantmanager.cpp 

HEADERS += \
    $$PWD/functionedit.h \
    $$PWD/functioneditordialog.h \
    $$PWD/insertfunctiondlg.h \
    $$PWD/inserttagdlg.h \
    $$PWD/tagcolorlistedit.h \
    $$PWD/tagcolorlisteditordialog.h \
    $$PWD/tagtextlistedit.h \		
	$$PWD/tagtextlisteditordialog.h \	
	$$PWD/scriptedit.h \	
	$$PWD/scriptpropertyeditordlg.h \
    $$PWD/variantfactory.h \
    $$PWD/variantmanager.h 

FORMS += \
    $$PWD/functioneditordialog.ui \
    $$PWD/insertfunctiondlg.ui \
    $$PWD/inserttagdlg.ui \
    $$PWD/tagcolorlisteditordialog.ui \
    $$PWD/tagtextlisteditordialog.ui \
    $$PWD/scriptpropertyeditordlg.ui

RESOURCES += \
    $$PWD/images.qrc 

