QT += core gui xml widgets script

TARGET = core
TEMPLATE = lib

CONFIG(debug, debug|release) { #debug
    DEFINES += BUILD_DEBUG
}

BUILD_DIR = $$clean_path($$OUT_PWD)

DESTDIR = $$BUILD_DIR/../../../lib_rt
win32 {
    DLLDESTDIR = $$BUILD_DIR/../../../HmiRunTimeBin
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

LIBS += -L$$clean_path($$DESTDIR) -lshared
LIBS += -L$$clean_path($$DESTDIR) -lgradientEditor

HEADERS += \
    listviewex.h \
    qpropertylist/coloreditor.h \
#    qpropertylist/functioneditordialog.h \
    qpropertylist/functioneditordialog.h \
    qpropertylist/insertfunctiondlg.h \
    qpropertylist/inserttagdlg.h \
    qpropertylist/qcoloreditor.h \
    qpropertylist/qfileeditor.h \
    qpropertylist/qfunctioneditor.h \
    qpropertylist/qimageeditor.h \
    qpropertylist/qtagcolorlisteditor.h \
    qpropertylist/qtagtextlisteditor.h \
    qpropertylist/tagcolorlisteditordialog.h \
    qpropertylist/tagtextlisteditordialog.h \
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
    qipaddressedit.h

SOURCES += \
    listviewex.cpp \
    qpropertylist/coloreditor.cpp \
#    qpropertylist/functioneditordialog.cpp \
    qpropertylist/functioneditordialog.cpp \
    qpropertylist/insertfunctiondlg.cpp \
    qpropertylist/inserttagdlg.cpp \
    qpropertylist/qcoloreditor.cpp \
    qpropertylist/qfileeditor.cpp \
    qpropertylist/qfunctioneditor.cpp \
    qpropertylist/qimageeditor.cpp \
    qpropertylist/qtagcolorlisteditor.cpp \
    qpropertylist/qtagtextlisteditor.cpp \
    qpropertylist/tagcolorlisteditordialog.cpp \
    qpropertylist/tagtextlisteditordialog.cpp \
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
    qipaddressedit.cpp

FORMS += \
#    qpropertylist/functioneditordialog.ui \
    qpropertylist/functioneditordialog.ui \
    qpropertylist/insertfunctiondlg.ui \
    qpropertylist/inserttagdlg.ui \
    qpropertylist/tagcolorlisteditordialog.ui \
    qpropertylist/tagtextlisteditordialog.ui \
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
    qipaddressedit.ui


RESOURCES += \
    images.qrc
