
exists(config.pri):infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtpropertybrowser-uselib

TEMPLATE += fakelib
QTPROPERTYBROWSER_LIBNAME = $$qtLibraryTarget(QtPropertyBrowser)
TEMPLATE -= fakelib
QTPROPERTYBROWSER_LIBDIR = $$OUT_PWD/../../../../lib
#message(QTPROPERTYBROWSER_LIBDIR =  $$QTPROPERTYBROWSER_LIBDIR)
unix:qtpropertybrowser-uselib:!qtpropertybrowser-buildlib:QMAKE_RPATHDIR += $$QTPROPERTYBROWSER_LIBDIR


