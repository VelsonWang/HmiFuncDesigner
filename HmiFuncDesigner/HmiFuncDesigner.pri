!isEmpty(HMIFUNCDESIGNER_PRI_INCLUDED):error("HmiFuncDesigner.pri already included")
HMIFUNCDESIGNER_PRI_INCLUDED = 1

HMIFUNCDESIGNER_VERSION = 0.0.1
HMIFUNCDESIGNER_COMPAT_VERSION = 0.0.1
BINARY_ARTIFACTS_BRANCH = 0.0

# turn off qDebug messages
CONFIG(release, debug|release) { #release
#    DEFINES += QT_NO_DEBUG_OUTPUT
}


# enable c++11
CONFIG += c++11

defineReplace(qtLibraryName) {
   unset(LIBRARY_NAME)
   LIBRARY_NAME = $$1
   CONFIG(debug, debug|release) {
      !debug_and_release|build_pass {
          mac:RET = $$member(LIBRARY_NAME, 0)_debug
              else:win32:RET = $$member(LIBRARY_NAME, 0)d
      }
   }
   isEmpty(RET):RET = $$LIBRARY_NAME
   return($$RET)
}

defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}

# For use in custom compilers which just copy files
defineReplace(stripSrcDir) {
    return($$relative_path($$absolute_path($$1, $$OUT_PWD), $$_PRO_FILE_PWD_))
}

#!isEmpty(BUILD_TESTS):TEST = 1
!isEmpty(BUILD_TESTS):TEST = 0

isEmpty(TEST):CONFIG(debug, debug|release) {
    !debug_and_release|build_pass {
#        TEST = 1
        TEST = 1
    }
}

isEmpty(IDE_LIBRARY_BASENAME) {
    IDE_LIBRARY_BASENAME = libs
}



IDE_SOURCE_TREE = $$PWD
#message("IDE_SOURCE_TREE is: $$IDE_SOURCE_TREE")
isEmpty(IDE_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    IDE_BUILD_TREE = $$clean_path($$OUT_PWD)
    IDE_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}
#message("IDE_BUILD_TREE is: $$IDE_BUILD_TREE")
IDE_APP_PATH = $$IDE_BUILD_TREE/..HmiFuncDesignerBin/bin
#message("IDE_APP_PATH is: $$IDE_APP_PATH")
osx {

} else {
    contains(TEMPLATE, vc.*):vcproj = 1

    # target output path if not set manually
    isEmpty(IDE_OUTPUT_PATH): IDE_OUTPUT_PATH = $$IDE_BUILD_TREE
    #message("IDE_OUTPUT_PATH is: $$IDE_OUTPUT_PATH")
    IDE_LIBRARY_PATH = $$IDE_OUTPUT_PATH/../lib
    IDE_PLUGIN_PATH  = $$IDE_LIBRARY_PATH/plugins
    IDE_DATA_PATH    = $$IDE_OUTPUT_PATH/../HmiFuncDesignerBin
    IDE_DOC_PATH     = $$IDE_OUTPUT_PATH/../HmiFuncDesignerBin/doc
    IDE_BIN_PATH     = $$IDE_OUTPUT_PATH/../HmiFuncDesignerBin/bin
    IDE_TOOL_PATH    = $$IDE_OUTPUT_PATH/../HmiFuncDesignerBin/tools
    win32: \
        IDE_LIBEXEC_PATH = $$IDE_OUTPUT_PATH/../HmiFuncDesignerBin/bin
    else: \
        IDE_LIBEXEC_PATH = $$IDE_OUTPUT_PATH/../QHmiFuncDesignerBin/libexec
    !isEqual(IDE_SOURCE_TREE, $$IDE_OUTPUT_PATH):copydata = 1

    LINK_LIBRARY_PATH = $$IDE_LIBRARY_PATH
    LINK_PLUGIN_PATH  = $$LINK_LIBRARY_PATH/plugins

    INSTALL_LIBRARY_PATH = $$IDE_BIN_PATH
    INSTALL_DEVICE_PLUGIN_PATH  = $$IDE_OUTPUT_PATH/../HmiFuncDesignerBin/deviceplugins
    INSTALL_DRAWAPPLICATION_PLUGIN_PATH  = $$IDE_OUTPUT_PATH/../HmiFuncDesignerBin/DrawApplicationPlugins

    INSTALL_PLUGIN_PATH  = $$INSTALL_LIBRARY_PATH/plugins
    win32: \
        INSTALL_LIBEXEC_PATH = $$QTC_PREFIX/../HmiFuncDesignerBin/bin
    else: \
        INSTALL_LIBEXEC_PATH = $$QTC_PREFIX/libexec/bin
    INSTALL_BIN_PATH     = $$QTC_PREFIX/exec/bin

    #message("QTC_PREFIX is: $$QTC_PREFIX")
}

INCLUDEPATH += \
    $$IDE_SOURCE_TREE/libs/Utils \
    $$IDE_SOURCE_TREE/libs/edncrypt \
    $$IDE_SOURCE_TREE/libs/DrawUtils \
    $$IDE_SOURCE_TREE/libs/Csv \
    $$IDE_SOURCE_TREE/libs/XmlUtils \
    $$IDE_SOURCE_TREE/libs/Lua \
    $$IDE_SOURCE_TREE/libs/qtsingleapplication \
    $$IDE_SOURCE_TREE/libs/qscintilla \
    $$IDE_SOURCE_TREE/libs/qscintilla/Qsci \
    $$IDE_SOURCE_TREE/libs/qscintilla/src \
    $$IDE_SOURCE_TREE/libs/qscintilla/include \
    $$IDE_SOURCE_TREE/libs/qscintilla/lexlib \
    $$IDE_SOURCE_TREE/libs/qtpropertybrowser/src \
    $$IDE_SOURCE_TREE/libs/TagManager \
    $$IDE_SOURCE_TREE/libs/ProjectDataUtils \
    $$IDE_SOURCE_TREE/Devices/IDevicePlugin \
    $$IDE_SOURCE_TREE/DrawApplication/IDrawApplicationPlugin \
    $$IDE_SOURCE_TREE/DrawApplication/DrawApplication \
    $$IDE_SOURCE_TREE/DrawApplication/QtPropertyBrowserEx \
    $$IDE_SOURCE_TREE/DrawApplication/Public
	

QTC_PLUGIN_DIRS_FROM_ENVIRONMENT = $$(QTC_PLUGIN_DIRS)
QTC_PLUGIN_DIRS += $$split(QTC_PLUGIN_DIRS_FROM_ENVIRONMENT, $$QMAKE_DIRLIST_SEP)
QTC_PLUGIN_DIRS += $$IDE_SOURCE_TREE/src/plugins
for(dir, QTC_PLUGIN_DIRS) {
    INCLUDEPATH += $$dir
}

CONFIG += \
    depend_includepath \
    no_include_pwd

LIBS *= -L$$LINK_LIBRARY_PATH  # Qt Creator libraries
exists($$IDE_LIBRARY_PATH): LIBS *= -L$$IDE_LIBRARY_PATH  # library path from output path

!isEmpty(vcproj) {
    DEFINES += IDE_LIBRARY_BASENAME=\"$$IDE_LIBRARY_BASENAME\"
} else {
    DEFINES += IDE_LIBRARY_BASENAME=\\\"$$IDE_LIBRARY_BASENAME\\\"
}

#DEFINES += QT_CREATOR QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII
!macx:DEFINES += QT_USE_FAST_OPERATOR_PLUS QT_USE_FAST_CONCATENATION

unix {
    CONFIG(debug, debug|release):OBJECTS_DIR = $${OUT_PWD}/.obj/debug-shared
    CONFIG(release, debug|release):OBJECTS_DIR = $${OUT_PWD}/.obj/release-shared

    CONFIG(debug, debug|release):MOC_DIR = $${OUT_PWD}/.moc/debug-shared
    CONFIG(release, debug|release):MOC_DIR = $${OUT_PWD}/.moc/release-shared

    RCC_DIR = $${OUT_PWD}/.rcc
    UI_DIR = $${OUT_PWD}/.uic
}

win32-msvc* {
    #Don't warn about sprintf, fopen etc being 'unsafe'
    DEFINES += _CRT_SECURE_NO_WARNINGS
    QMAKE_CXXFLAGS_WARN_ON *= -w44996
    # Speed up startup time when debugging with cdb
    QMAKE_LFLAGS_DEBUG += /INCREMENTAL:NO
}

qt {
    contains(QT, core): QT += concurrent
    contains(QT, gui): QT += widgets
    QT += xml
    QT += sql
}

QBSFILE = $$replace(_PRO_FILE_, \\.pro$, .qbs)
exists($$QBSFILE):DISTFILES += $$QBSFILE

!isEmpty(QTC_PLUGIN_DEPENDS) {
    LIBS *= -L$$IDE_PLUGIN_PATH  # plugin path from output directory
    LIBS *= -L$$LINK_PLUGIN_PATH  # when output path is different from Qt Creator build directory
}

# recursively resolve plugin deps
done_plugins =
for(ever) {
    isEmpty(QTC_PLUGIN_DEPENDS): \
        break()
    done_plugins += $$QTC_PLUGIN_DEPENDS
    for(dep, QTC_PLUGIN_DEPENDS) {
        dependencies_file =
        for(dir, QTC_PLUGIN_DIRS) {
            exists($$dir/$$dep/$${dep}_dependencies.pri) {
                dependencies_file = $$dir/$$dep/$${dep}_dependencies.pri
                break()
            }
        }
        isEmpty(dependencies_file): \
            error("Plugin dependency $$dep not found")
        include($$dependencies_file)
        LIBS += -l$$qtLibraryName($$QTC_PLUGIN_NAME)
    }
    QTC_PLUGIN_DEPENDS = $$unique(QTC_PLUGIN_DEPENDS)
    QTC_PLUGIN_DEPENDS -= $$unique(done_plugins)
}

# recursively resolve library deps
done_libs =
for(ever) {
    isEmpty(QTC_LIB_DEPENDS): \
        break()
    done_libs += $$QTC_LIB_DEPENDS
    for(dep, QTC_LIB_DEPENDS) {
        include($$PWD/libs/$$dep/$${dep}_dependencies.pri)
        LIBS += -l$$qtLibraryName($$QTC_LIB_NAME)
    }
    QTC_LIB_DEPENDS = $$unique(QTC_LIB_DEPENDS)
    QTC_LIB_DEPENDS -= $$unique(done_libs)

}


