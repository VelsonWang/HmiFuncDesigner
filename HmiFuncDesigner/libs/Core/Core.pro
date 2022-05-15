QT += core gui xml widgets script

TARGET = core
TEMPLATE = lib

CONFIG(debug, debug|release) { #debug
    DEFINES += BUILD_DEBUG
}

BUILD_DIR = $$clean_path($$OUT_PWD)

DESTDIR = $$BUILD_DIR/../../../lib
win32 {
    DLLDESTDIR = $$BUILD_DIR/../../../HmiFuncDesignerBin/bin
}

DEFINES += CORE_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

LIB_SUFFIX=""
CONFIG(debug, debug|release) { #debug
    win32 {
        LIB_SUFFIX=d
    }
}

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/qpropertylist
INCLUDEPATH += $$PWD/undocommand
INCLUDEPATH += $$PWD/../qscintilla

LIBS += -L$$clean_path($$DESTDIR) -lshared
LIBS += -L$$clean_path($$DESTDIR) -lgradientEditor
LIBS += -L$$clean_path($$DESTDIR) -lqscintilla2_qt$${QT_MAJOR_VERSION}$${LIB_SUFFIX}

HEADERS += \
    devicelistdialog.h \
    devicepluginloader.h \
    listviewex.h \
    newcomdevicedialog.h \
    newnetdevicedialog.h \
    qpropertylist/coloreditor.h \
    qpropertylist/function/addtagvalueform.h \
    qpropertylist/function/functioneditordialog.h \
    qpropertylist/function/gettagvalueform.h \
    qpropertylist/function/insertfunctiondlg.h \
    qpropertylist/function/qabstractfunction.h \
    qpropertylist/function/returngraphpageform.h \
    qpropertylist/function/setdatetimeform.h \
    qpropertylist/function/settagvalueform.h \
    qpropertylist/function/sleepform.h \
    qpropertylist/function/statechangeform.h \
    qpropertylist/function/subtagvalueform.h \
    qpropertylist/function/switchgraphpageform.h \
    qpropertylist/function/waitformillisecform.h \
    qpropertylist/inserttagdlg.h \
    qpropertylist/qcoloreditor.h \
    qpropertylist/qfileeditor.h \
    qpropertylist/qfunctioneditor.h \
    qpropertylist/qimageeditor.h \
    qpropertylist/qtagcolorlisteditor.h \
    qpropertylist/qtagtextlisteditor.h \
    qpropertylist/scriptpropertyeditordlg.h \
    qpropertylist/tagcolorlisteditordialog.h \
    qpropertylist/tagtextlisteditordialog.h \
    selectprotocoldialog.h \
    tageditdialog.h \
    corelibglobal.h \
    qabstractpage.h \
    qabstractpagewidget.h \
    minisplitter.h \
    manhattanstyle.h \
    stylehelper.h \
    styledbar.h \
    qtoolbarbutton.h \
    qfancylineedit.h \
    qactiontoolbar.h \
    qsoftcore.h \
    qpropertylist/qpropertylistview.h \
    qpropertylist/qpropertybaseeditor.h \
    qbaseinit.h \
    qpropertylist/qbytearrayeditor.h \
    qnamevalidator.h \
    undocommand/qbaseundocommand.h \
    undocommand/qpropertychangedundocommand.h \
    qpropertylist/qenumpropertyeditor.h \
    qrenamedialog.h \
    undocommand/qhostparentchangedundocommand.h \
    qpropertylist/qnumbereditor.h \
    qpropertylist/qcommoneditor.h \
    qpropertylist/qbooleditor.h \
    qpropertylist/qbuttoncommoneditor.h \
    qpropertylist/qfonteditor.h \
    qpropertylist/qstringeditor.h \
    qpropertylist/qstringeditdialog.h \
    undocommand/qstringchangedundocommand.h \
    undocommand/qaddhostundocommand.h \
    qpropertylist/qfloateditor.h \
    qpropertylist/script_editor/qwidgetviewtoolbar.h \
    qpropertylist/script_editor/qscripthighlight.h \
    qpropertylist/script_editor/qhostobjectmap.h \
    qpropertylist/script_editor/qfindwidget.h \
    qpropertylist/script_editor/qwidgetview.h \
    qpropertylist/script_editor/qfunctionview.h \
    qpropertylist/script_editor/qcompletertreewidget.h \
    qpropertylist/script_editor/qscripteditview.h \
    qpropertylist/script_editor/qscripteditdialog.h \
    qpropertylist/qscripteditor.h \
    qabstractupdate.h \
    qdevicesmanager.h \
    qbtnlineedit.h \
    qtexteditdialog.h \
    qfilemanager.h \
    qfilelistview.h \
    undocommand/qfilegroupaddundocommand.h \
    undocommand/qfileaddundocommand.h \
    qfilelistitemwidget.h \
    qabstracteditorplugin.h \
    qabstractresourceeditor.h \
    qpropertylist/stylesheeteditor/qstylesheeteditor.h \
    qpropertylist/stylesheeteditor/qstylesheetdialog.h \
    qpropertylist/stylesheeteditor/qsheetitemlist.h \
    qpropertylist/stylesheeteditor/qitemlistwidget.h \
    qpropertylist/stylesheeteditor/qshowwidget.h \
    qpropertylist/stylesheeteditor/qbackgrounsheeteditor.h \
    qpropertylist/stylesheeteditor/qbaseeditorwidget.h \
    qpropertylist/stylesheeteditor/qresourceview.h \
    qpropertylist/stylesheeteditor/qresourceshowwidget.h \
    qpropertylist/stylesheeteditor/qbordersheeteditor.h \
    qpropertylist/stylesheeteditor/qtextsheeteditor.h \
    qpropertylist/stylesheeteditor/qaddsheetitemdialog.h \
    qpropertylist/stylesheeteditor/qstatecheckwidget.h \
    qpropertylist/stylesheeteditor/qitemcheckbox.h \
    undocommand/qpageaddundocommand.h \
    qprojectinformation.h \
    databasesettingdialog.h \
    netsettingdialog.h \
    newprojectdialog.h \
    qipaddressedit.h  \
    userauthoritydialog.h

SOURCES += \
    devicelistdialog.cpp \
    devicepluginloader.cpp \
    listviewex.cpp \
    newcomdevicedialog.cpp \
    newnetdevicedialog.cpp \
    qpropertylist/coloreditor.cpp \
    qpropertylist/function/addtagvalueform.cpp \
    qpropertylist/function/functioneditordialog.cpp \
    qpropertylist/function/gettagvalueform.cpp \
    qpropertylist/function/insertfunctiondlg.cpp \
    qpropertylist/function/qabstractfunction.cpp \
    qpropertylist/function/returngraphpageform.cpp \
    qpropertylist/function/setdatetimeform.cpp \
    qpropertylist/function/settagvalueform.cpp \
    qpropertylist/function/sleepform.cpp \
    qpropertylist/function/statechangeform.cpp \
    qpropertylist/function/subtagvalueform.cpp \
    qpropertylist/function/switchgraphpageform.cpp \
    qpropertylist/function/waitformillisecform.cpp \
    qpropertylist/inserttagdlg.cpp \
    qpropertylist/qcoloreditor.cpp \
    qpropertylist/qfileeditor.cpp \
    qpropertylist/qfunctioneditor.cpp \
    qpropertylist/qimageeditor.cpp \
    qpropertylist/qtagcolorlisteditor.cpp \
    qpropertylist/qtagtextlisteditor.cpp \
    qpropertylist/scriptpropertyeditordlg.cpp \
    qpropertylist/tagcolorlisteditordialog.cpp \
    qpropertylist/tagtextlisteditordialog.cpp \
    selectprotocoldialog.cpp \
    tageditdialog.cpp \
    qabstractpage.cpp \
    qabstractpagewidget.cpp \
    minisplitter.cpp \
    manhattanstyle.cpp \
    stylehelper.cpp \
    styledbar.cpp \
    qtoolbarbutton.cpp \
    qfancylineedit.cpp \
    qactiontoolbar.cpp \
    qsoftcore.cpp \
    qpropertylist/qpropertylistview.cpp \
    qpropertylist/qpropertybaseeditor.cpp \
    qbaseinit.cpp \
    qpropertylist/qbytearrayeditor.cpp \
    qnamevalidator.cpp \
    undocommand/qbaseundocommand.cpp \
    undocommand/qpropertychangedundocommand.cpp \
    qpropertylist/qenumpropertyeditor.cpp \
    qrenamedialog.cpp \
    undocommand/qhostparentchangedundocommand.cpp \
    qpropertylist/qnumbereditor.cpp \
    qpropertylist/qcommoneditor.cpp \
    qpropertylist/qbooleditor.cpp \
    qpropertylist/qbuttoncommoneditor.cpp \
    qpropertylist/qfonteditor.cpp \
    qpropertylist/qstringeditor.cpp \
    qpropertylist/qstringeditdialog.cpp \
    undocommand/qstringchangedundocommand.cpp \
    undocommand/qaddhostundocommand.cpp \
    qpropertylist/qfloateditor.cpp \
    qpropertylist/script_editor/qwidgetviewtoolbar.cpp \
    qpropertylist/script_editor/qscripthighlight.cpp \
    qpropertylist/script_editor/qhostobjectmap.cpp \
    qpropertylist/script_editor/qfindwidget.cpp \
    qpropertylist/script_editor/qwidgetview.cpp \
    qpropertylist/script_editor/qfunctionview.cpp \
    qpropertylist/script_editor/qcompletertreewidget.cpp \
    qpropertylist/script_editor/qscripteditview.cpp \
    qpropertylist/script_editor/qscripteditdialog.cpp \
    qpropertylist/qscripteditor.cpp \
    qabstractupdate.cpp \
    qdevicesmanager.cpp \
    qbtnlineedit.cpp \
    qtexteditdialog.cpp \
    qfilemanager.cpp \
    qfilelistview.cpp \
    undocommand/qfilegroupaddundocommand.cpp \
    undocommand/qfileaddundocommand.cpp \
    qfilelistitemwidget.cpp \
    qabstracteditorplugin.cpp \
    qabstractresourceeditor.cpp \
    qpropertylist/stylesheeteditor/qstylesheeteditor.cpp \
    qpropertylist/stylesheeteditor/qstylesheetdialog.cpp \
    qpropertylist/stylesheeteditor/qsheetitemlist.cpp \
    qpropertylist/stylesheeteditor/qitemlistwidget.cpp \
    qpropertylist/stylesheeteditor/qshowwidget.cpp \
    qpropertylist/stylesheeteditor/qbackgrounsheeteditor.cpp \
    qpropertylist/stylesheeteditor/qbaseeditorwidget.cpp \
    qpropertylist/stylesheeteditor/qresourceview.cpp \
    qpropertylist/stylesheeteditor/qresourceshowwidget.cpp \
    qpropertylist/stylesheeteditor/qbordersheeteditor.cpp \
    qpropertylist/stylesheeteditor/qtextsheeteditor.cpp \
    qpropertylist/stylesheeteditor/qaddsheetitemdialog.cpp \
    qpropertylist/stylesheeteditor/qstatecheckwidget.cpp \
    qpropertylist/stylesheeteditor/qitemcheckbox.cpp \
    undocommand/qpageaddundocommand.cpp \
    qprojectinformation.cpp \
    databasesettingdialog.cpp \
    netsettingdialog.cpp \
    newprojectdialog.cpp \
    qipaddressedit.cpp \
    userauthoritydialog.cpp

FORMS += \
    devicelistdialog.ui \
    newcomdevicedialog.ui \
    newnetdevicedialog.ui \
    qpropertylist/function/addtagvalueform.ui \
    qpropertylist/function/functioneditordialog.ui \
    qpropertylist/function/gettagvalueform.ui \
    qpropertylist/function/insertfunctiondlg.ui \
    qpropertylist/function/returngraphpageform.ui \
    qpropertylist/function/setdatetimeform.ui \
    qpropertylist/function/settagvalueform.ui \
    qpropertylist/function/sleepform.ui \
    qpropertylist/function/statechangeform.ui \
    qpropertylist/function/subtagvalueform.ui \
    qpropertylist/function/switchgraphpageform.ui \
    qpropertylist/function/waitformillisecform.ui \
    qpropertylist/inserttagdlg.ui \
    qpropertylist/scriptpropertyeditordlg.ui \
    qpropertylist/tagcolorlisteditordialog.ui \
    qpropertylist/tagtextlisteditordialog.ui \
    selectprotocoldialog.ui \
    tageditdialog.ui \
    qrenamedialog.ui \
    qpropertylist/qstringeditdialog.ui \
    qpropertylist/script_editor/qwidgetviewtoolbar.ui \
    qpropertylist/script_editor/qfindwidget.ui \
    qtexteditdialog.ui \
    qfilelistitemwidget.ui \
    qpropertylist/stylesheeteditor/qitemlistwidget.ui \
    qpropertylist/stylesheeteditor/qshowwidget.ui \
    qpropertylist/stylesheeteditor/qbackgrounsheeteditor.ui \
    qpropertylist/stylesheeteditor/qresourceshowwidget.ui \
    qpropertylist/stylesheeteditor/qbordersheeteditor.ui \
    qpropertylist/stylesheeteditor/qtextsheeteditor.ui \
    qpropertylist/stylesheeteditor/qaddsheetitemdialog.ui \
    qpropertylist/stylesheeteditor/qitemcheckbox.ui \
    databasesettingdialog.ui \
    netsettingdialog.ui \
    newprojectdialog.ui \
    qipaddressedit.ui \
    userauthoritydialog.ui


RESOURCES += \
    images.qrc
