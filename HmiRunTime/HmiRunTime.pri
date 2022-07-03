!isEmpty(HMIRUNTIME_PRI_INCLUDED):error("HmiRunTime.pri already included")
HMIRUNTIME_PRI_INCLUDED = 1

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

LIB_SUFFIX=""
CONFIG(debug, debug|release) { #debug
    win32 {
        LIB_SUFFIX=d
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
IDE_APP_PATH = $$clean_path($$IDE_BUILD_TREE/../HmiRunTimeBin)
#message("IDE_APP_PATH is: $$IDE_APP_PATH")
osx {

} else {
    contains(TEMPLATE, vc.*):vcproj = 1

    isEmpty(IDE_OUTPUT_PATH): IDE_OUTPUT_PATH = $$clean_path($$IDE_BUILD_TREE)
    #message("IDE_OUTPUT_PATH is: $$IDE_OUTPUT_PATH")
    IDE_LIBRARY_PATH = $$clean_path($$IDE_BUILD_TREE/../lib_rt)
    #message("IDE_LIBRARY_PATH is: $$IDE_LIBRARY_PATH")
    IDE_PLUGIN_PATH  = $$clean_path($$IDE_LIBRARY_PATH/plugins)
    #message("IDE_PLUGIN_PATH is: $$IDE_PLUGIN_PATH")
    LINK_LIBRARY_PATH = $$clean_path($$IDE_LIBRARY_PATH)
    #message("LINK_LIBRARY_PATH is: $$LINK_LIBRARY_PATH")
    LINK_DEVICE_PLUGIN_PATH  = $$clean_path($$LINK_LIBRARY_PATH/Vendors)
    #message("LINK_DEVICE_PLUGIN_PATH is: $$LINK_DEVICE_PLUGIN_PATH")
    INSTALL_DEVICE_PLUGIN_PATH  = $$clean_path($$IDE_APP_PATH/Vendors)
    #message("INSTALL_DEVICE_PLUGIN_PATH is: $$INSTALL_DEVICE_PLUGIN_PATH")
    LINK_VIEW_ELEMENT_PLUGIN_PATH  = $$clean_path($$LINK_LIBRARY_PATH/ViewElements)
    #message("LINK_VIEW_ELEMENT_PLUGIN_PATH is: $$LINK_VIEW_ELEMENT_PLUGIN_PATH")
    INSTALL_VIEW_ELEMENT_PLUGIN_PATH  = $$clean_path($$IDE_APP_PATH/ViewElements)
    #message("INSTALL_VIEW_ELEMENT_PLUGIN_PATH is: $$INSTALL_VIEW_ELEMENT_PLUGIN_PATH")
}

INCLUDEPATH += \
    $$IDE_SOURCE_TREE/libs/Utils \
    $$IDE_SOURCE_TREE/libs/edncrypt \
    $$IDE_SOURCE_TREE/libs/qtsingleapplication


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


