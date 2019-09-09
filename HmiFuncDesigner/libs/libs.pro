
include(../HmiFuncDesigner.pri)

TEMPLATE  = subdirs
CONFIG += ordered

SUBDIRS   = \
    Utils \
    edncrypt \
    Csv \
    PluginManager \
    DrawUtils \
    qtpropertybrowser \
    qscintilla \
    Lua \
    Ftp \
    TagManager \
    ProjectDataUtils



for(l, SUBDIRS) {
    QTC_LIB_DEPENDS =
    include($$l/$${l}_dependencies.pri)
    lv = $${l}.depends
    $$lv = $$QTC_LIB_DEPENDS
}



