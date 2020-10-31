
include(../HmiFuncDesigner.pri)

TEMPLATE  = subdirs

SUBDIRS   = \
    Utils \
    edncrypt \
    Csv \
    DrawUtils \
    qtpropertybrowser \
    qscintilla \
#    Lua \
#    Ftp \
    ProjData




for(l, SUBDIRS) {
    QTC_LIB_DEPENDS =
    include($$l/$${l}_dependencies.pri)
    lv = $${l}.depends
    $$lv = $$QTC_LIB_DEPENDS
}



