
QT += core
QT += gui
QT += xml
QT += widgets


INCLUDEPATH +=  $$PWD

SOURCES += \
    $$PWD/functionedit.cpp \
    $$PWD/functioneditordialog.cpp \
    $$PWD/tagcolorlistedit.cpp \
    $$PWD/tagcolorlisteditordialog.cpp \
    $$PWD/tagtextlistedit.cpp \	
    $$PWD/tagtextlisteditordialog.cpp \
	$$PWD/coloreditor.cpp \
    $$PWD/variantfactory.cpp \
    $$PWD/variantmanager.cpp 

HEADERS += \
    $$PWD/functionedit.h \
    $$PWD/functioneditordialog.h \
    $$PWD/tagcolorlistedit.h \
    $$PWD/tagcolorlisteditordialog.h \
    $$PWD/tagtextlistedit.h \		
	$$PWD/tagtextlisteditordialog.h \
	$$PWD/coloreditor.h \
    $$PWD/variantfactory.h \
    $$PWD/variantmanager.h 

FORMS += \
    $$PWD/functioneditordialog.ui \
    $$PWD/tagcolorlisteditordialog.ui \
    $$PWD/tagtextlisteditordialog.ui



