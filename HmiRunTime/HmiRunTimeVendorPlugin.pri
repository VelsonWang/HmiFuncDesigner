
depfile = $$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri)

exists($$depfile) {
    include($$depfile)
    isEmpty(QTC_PLUGIN_NAME): \
        error("$$basename(depfile) does not define QTC_PLUGIN_NAME.")
} else {
    isEmpty(QTC_PLUGIN_NAME): \
        error("QTC_PLUGIN_NAME is empty. Maybe you meant to create $$basename(depfile)?")
}
TARGET = $$QTC_PLUGIN_NAME

plugin_deps = $$QTC_PLUGIN_DEPENDS
plugin_test_deps = $$QTC_TEST_DEPENDS
plugin_recmds = $$QTC_PLUGIN_RECOMMENDS

#include(QStudioSCADA.pri)

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

defineReplace(dependencyName) {
    dependencies_file =
    for(dir, QTC_PLUGIN_DIRS) {
        exists($$dir/$$1/$${1}_dependencies.pri) {
            dependencies_file = $$dir/$$1/$${1}_dependencies.pri
            break()
        }
    }
    isEmpty(dependencies_file): \
        error("Plugin dependency $$dep not found")
    include($$dependencies_file)
    return($$QTC_PLUGIN_NAME)
}

# for substitution in the .json
dependencyList =
for(dep, plugin_deps) {
    dependencyList += "        { \"Name\" : \"$$dependencyName($$dep)\", \"Version\" : \"$$QTCREATOR_VERSION\" }"
}
for(dep, plugin_recmds) {
    dependencyList += "        { \"Name\" : \"$$dependencyName($$dep)\", \"Version\" : \"$$QTCREATOR_VERSION\", \"Type\" : \"optional\" }"
}
for(dep, plugin_test_deps) {
    dependencyList += "        { \"Name\" : \"$$dependencyName($$dep)\", \"Version\" : \"$$QTCREATOR_VERSION\", \"Type\" : \"test\" }"
}
dependencyList = $$join(dependencyList, ",$$escape_expand(\\n)")

dependencyList = "\"Dependencies\" : [$$escape_expand(\\n)$$dependencyList$$escape_expand(\\n)    ]"


DESTDIR = $$_PRO_FILE_PWD_/../../../RuntimeBin/Vendor

LIBS += -L$$DESTDIR
INCLUDEPATH += $$OUT_PWD

# copy the plugin spec
isEmpty(TARGET) {
    error("qtcreatorplugin.pri: You must provide a TARGET")
}

PLUGINJSON = $$_PRO_FILE_PWD_/$${TARGET}.json
PLUGINJSON_IN = $${PLUGINJSON}.in
exists($$PLUGINJSON_IN) {
    DISTFILES += $$PLUGINJSON_IN
    QMAKE_SUBSTITUTES += $$PLUGINJSON_IN
    PLUGINJSON = $$OUT_PWD/$${TARGET}.json
} else {
    # need to support that for external plugins
    DISTFILES += $$PLUGINJSON
}


TEMPLATE = lib
CONFIG += plugin plugin_with_soname
linux*:QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF

!macx {
    target.path = $$INSTALL_DEVICE_PLUGIN_PATH
    INSTALLS += target
}

MIMETYPES = $$_PRO_FILE_PWD_/$${TARGET}.mimetypes.xml
exists($$MIMETYPES):DISTFILES += $$MIMETYPES

TARGET = $$qtLibraryName($$TARGET)


