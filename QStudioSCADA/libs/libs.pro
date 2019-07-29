
include(../QStudioSCADA.pri)

TEMPLATE  = subdirs
CONFIG += ordered

SUBDIRS   = \
    Utils \
    edncrypt \
    Csv \
    PluginManager \
    DrawUtils \
    qtpropertybrowser \
    QScintilla \
    Lua \
    Ftp \
    PropertyEditor \
    TagManager \
    ProjectDataUtils



for(l, SUBDIRS) {
    QTC_LIB_DEPENDS =
    include($$l/$${l}_dependencies.pri)
    lv = $${l}.depends
    $$lv = $$QTC_LIB_DEPENDS
}



