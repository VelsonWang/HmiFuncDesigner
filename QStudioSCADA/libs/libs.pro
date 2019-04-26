
include(../QStudioSCADA.pri)

TEMPLATE  = subdirs

SUBDIRS   = \
    Utils \
    edncrypt \
    ConfigUtils \
    XmlUtils \
    Csv \
    Helper \
    PluginManager \
    DrawUtils \
    qtpropertybrowser \
    qscintilla \
    Lua \
    Ftp \
    PropertyEditor \
    TagManager



for(l, SUBDIRS) {
    QTC_LIB_DEPENDS =
    include($$l/$${l}_dependencies.pri)
    lv = $${l}.depends
    $$lv = $$QTC_LIB_DEPENDS
}



